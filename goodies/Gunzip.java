import java.io.BufferedInputStream;
import java.io.DataInput;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.EOFException;
import java.io.OutputStream;
import java.util.Date;
import java.util.zip.CRC32;
import java.util.zip.DataFormatException;
import java.io.ByteArrayOutputStream;
import java.util.Arrays;
import java.util.zip.DataFormatException;
import java.util.ArrayList;
import java.util.List;
import java.io.InputStream;

public class Gunzip
{
	public static void main(String[] args)
    {
		String msg = submain(args);
		if (msg != null) {
			System.err.println(msg);
			System.exit(1);
		}
	}
	
	
	private static String submain(String[] args)
    {
		if (args.length != 2)
			return "Usage: java GzipDecompress InputFile OutputFile";
		
		File inFile = new File(args[0]);
		if (!inFile.exists())
			return "Input file does not exist: " + inFile;
		if (inFile.isDirectory())
			return "Input file is a directory: " + inFile;
		
		try {
			DataInputStream in = new DataInputStream(new BufferedInputStream(new FileInputStream(inFile), 16 * 1024));
			byte[] decomp;
			int crc, size;
			try {
				// Header
				int flags;
				{
					byte[] b = new byte[10];
					in.readFully(b);
					if (b[0] != 0x1F || b[1] != (byte)0x8B)
						return "Invalid GZIP magic number";
					if (b[2] != 8)
						return "Unsupported compression method: " + (b[2] & 0xFF);
					flags = b[3] & 0xFF;
					
					// Reserved flags
					if ((flags & 0xE0) != 0)
						return "Reserved flags are set";
					
					// Modification time
					int mtime = (b[4] & 0xFF) | (b[5] & 0xFF) << 8 | (b[6] & 0xFF) << 16 | b[7] << 24;
					if (mtime != 0)
						System.out.println("Last modified: " + new Date(mtime * 1000L));
					else
						System.out.println("Last modified: N/A");
					
					// Extra flags
					switch (b[8] & 0xFF) {
						case 2:   System.out.println("Extra flags: Maximum compression");  break;
						case 4:   System.out.println("Extra flags: Fastest compression");  break;
						default:  System.out.println("Extra flags: Unknown (" + (b[8] & 0xFF) + ")");  break;
					}
					
					// Operating system
					String os;
					switch (b[9] & 0xFF) {
						case   0:  os = "FAT";             break;
						case   1:  os = "Amiga";           break;
						case   2:  os = "VMS";             break;
						case   3:  os = "Unix";            break;
						case   4:  os = "VM/CMS";          break;
						case   5:  os = "Atari TOS";       break;
						case   6:  os = "HPFS";            break;
						case   7:  os = "Macintosh";       break;
						case   8:  os = "Z-System";        break;
						case   9:  os = "CP/M";            break;
						case  10:  os = "TOPS-20";         break;
						case  11:  os = "NTFS";            break;
						case  12:  os = "QDOS";            break;
						case  13:  os = "Acorn RISCOS";    break;
						case 255:  os = "Unknown";         break;
						default :  os = "Really unknown";  break;
					}
					System.out.println("Operating system: " + os);
				}
				
				// Handle assorted flags
				if ((flags & 0x01) != 0)
					System.out.println("Flag: Text");
				if ((flags & 0x04) != 0) {
					System.out.println("Flag: Extra");
					byte[] b = new byte[2];
					in.readFully(b);
					int len = (b[0] & 0xFF) | (b[1] & 0xFF) << 8;
					in.readFully(new byte[len]);  // Skip extra data
				}
				if ((flags & 0x08) != 0)
					System.out.println("File name: " + readNullTerminatedString(in));
				if ((flags & 0x02) != 0) {
					byte[] b = new byte[2];
					in.readFully(b);
					System.out.printf("Header CRC-16: %04X%n", (b[0] & 0xFF) | (b[1] & 0xFF) << 8);
				}
				if ((flags & 0x10) != 0)
					System.out.println("Comment: " + readNullTerminatedString(in));
				
				// Decompress
				try {
					decomp = Decompressor.decompress(new ByteBitInputStream(in));
				} catch (DataFormatException e) {
					return "Invalid or corrupt compressed data: " + e.getMessage();
				}
				
				// Footer
				{
					byte[] b = new byte[8];
					in.readFully(b);
					crc  = (b[0] & 0xFF) | (b[1] & 0xFF) << 8 | (b[2] & 0xFF) << 16 | b[3] << 24;
					size = (b[4] & 0xFF) | (b[5] & 0xFF) << 8 | (b[6] & 0xFF) << 16 | b[7] << 24;
				}
			} finally {
				in.close();
			}
			
			// Check
			if (size != decomp.length)
				return String.format("Size mismatch: expected=%d, actual=%d", size, decomp.length);
			if (crc != getCrc32(decomp))
				return String.format("CRC-32 mismatch: expected=%08X, actual=%08X", crc, getCrc32(decomp));
			
			// Write decompressed data to output file
			File outFile = new File(args[1]);
			OutputStream out = new FileOutputStream(outFile);
			try {
				out.write(decomp);
			} finally {
				out.close();
			}
		} catch (IOException e) {
			return "I/O exception: " + e.getMessage();
		}
		
		return null;
	}
	
	
	private static String readNullTerminatedString(DataInput in) throws IOException {
		StringBuilder sb = new StringBuilder();
		while (true) {
			byte c = in.readByte();
			if (c == 0)
				break;
			else
				sb.append((char)(c & 0xFF));
		}
		return sb.toString();
	}
	
	
	private static int getCrc32(byte[] data) {
		CRC32 crc = new CRC32();
		crc.update(data);
		return (int)crc.getValue();
	}
	
}



final class Decompressor
{
	public static byte[] decompress(BitInputStream in) throws IOException, DataFormatException
    {
		Decompressor decomp = new Decompressor(in);
		return decomp.output.toByteArray();
	}
	
	private BitInputStream input;
	private ByteArrayOutputStream output;
	private CircularDictionary dictionary;
	
	private Decompressor(BitInputStream in) throws IOException, DataFormatException
    {
		input = in;
		output = new ByteArrayOutputStream();
		dictionary = new CircularDictionary(32 * 1024);
		
		while (true)
        {
			boolean isFinal = in.readNoEof() == 1;
			int type = readInt(2);
			
			if (type == 0)
				decompressUncompressedBlock();
			else if (type == 1 || type == 2) {
				CodeTree litLenCode, distCode;
				if (type == 1) {
					litLenCode = fixedLiteralLengthCode;
					distCode = fixedDistanceCode;
				} else {
					CodeTree[] temp = decodeHuffmanCodes(in);
					litLenCode = temp[0];
					distCode = temp[1];
				}
				decompressHuffmanBlock(litLenCode, distCode);
				
			} else if (type == 3)
				throw new DataFormatException("Invalid block type");
			else
				throw new AssertionError();
			
			if (isFinal)
				break;
		}
	}
	
	private static CodeTree fixedLiteralLengthCode;
	private static CodeTree fixedDistanceCode;
	
	static {
		int[] llcodelens = new int[288];
		Arrays.fill(llcodelens,   0, 144, 8);
		Arrays.fill(llcodelens, 144, 256, 9);
		Arrays.fill(llcodelens, 256, 280, 7);
		Arrays.fill(llcodelens, 280, 288, 8);
		fixedLiteralLengthCode = new CanonicalCode(llcodelens).toCodeTree();
		
		int[] distcodelens = new int[32];
		Arrays.fill(distcodelens, 5);
		fixedDistanceCode = new CanonicalCode(distcodelens).toCodeTree();
	}
	
	private CodeTree[]
    decodeHuffmanCodes(BitInputStream in) throws IOException, DataFormatException
    {
		int numLitLenCodes = readInt(5) + 257;
		int numDistCodes = readInt(5) + 1;
		int numCodeLenCodes = readInt(4) + 4;
		int[] codeLenCodeLen = new int[19];
		codeLenCodeLen[16] = readInt(3);
		codeLenCodeLen[17] = readInt(3);
		codeLenCodeLen[18] = readInt(3);
		codeLenCodeLen[ 0] = readInt(3);
		for (int i = 0; i < numCodeLenCodes - 4; i++) {
			if (i % 2 == 0)
				codeLenCodeLen[8 + i / 2] = readInt(3);
			else
				codeLenCodeLen[7 - i / 2] = readInt(3);
		}
		CodeTree codeLenCode;
		try {
			codeLenCode = new CanonicalCode(codeLenCodeLen).toCodeTree();
		} catch (IllegalStateException e) {
			throw new DataFormatException(e.getMessage());
		}
		
		int[] codeLens = new int[numLitLenCodes + numDistCodes];
		int runVal = -1;
		int runLen = 0;
		for (int i = 0; i < codeLens.length; i++) {
			if (runLen > 0) {
				codeLens[i] = runVal;
				runLen--;
				
			} else {
				int sym = decodeSymbol(codeLenCode);
				if (sym < 16) {
					codeLens[i] = sym;
					runVal = sym;
				} else {
					if (sym == 16) {
						if (runVal == -1)
							throw new DataFormatException("No code length value to copy");
						runLen = readInt(2) + 3;
					} else if (sym == 17) {
						runVal = 0;
						runLen = readInt(3) + 3;
					} else if (sym == 18) {
						runVal = 0;
						runLen = readInt(7) + 11;
					} else
						throw new AssertionError();
					i--;
				}
			}
		}
		if (runLen > 0)
			throw new DataFormatException("Run exceeds number of codes");
		
		// Create code trees
		int[] litLenCodeLen = Arrays.copyOf(codeLens, numLitLenCodes);
		CodeTree litLenCode;
		try {
			litLenCode = new CanonicalCode(litLenCodeLen).toCodeTree();
		} catch (IllegalStateException e) {
			throw new DataFormatException(e.getMessage());
		}
		
		int[] distCodeLen = Arrays.copyOfRange(codeLens, numLitLenCodes, codeLens.length);
		CodeTree distCode;
		if (distCodeLen.length == 1 && distCodeLen[0] == 0)
			distCode = null;  // Empty distance code; the block shall be all literal symbols
		else {
			// Get statistics for upcoming logic
			int oneCount = 0;
			int otherPositiveCount = 0;
			for (int x : distCodeLen) {
				if (x == 1)
					oneCount++;
				else if (x > 1)
					otherPositiveCount++;
			}
			
			if (oneCount == 1 && otherPositiveCount == 0) {
				distCodeLen = Arrays.copyOf(distCodeLen, 32);
				distCodeLen[31] = 1;
			}
			
			try {
				distCode = new CanonicalCode(distCodeLen).toCodeTree();
			} catch (IllegalStateException e) {
				throw new DataFormatException(e.getMessage());
			}
		}
		
		return new CodeTree[]{litLenCode, distCode};
	}
	
	
	/* Block decompression methods */
	
	private void decompressUncompressedBlock() throws IOException, DataFormatException {
		// Discard bits to align to byte boundary
		while (input.getBitPosition() != 0)
			input.readNoEof();
		
		// Read length
		int len  = readInt(16);
		int nlen = readInt(16);
		if ((len ^ 0xFFFF) != nlen)
			throw new DataFormatException("Invalid length in uncompressed block");
		
		// Copy bytes
		for (int i = 0; i < len; i++) {
			int temp = input.readByte();
			if (temp == -1)
				throw new EOFException();
			output.write(temp);
			dictionary.append(temp);
		}
	}
	
	
	private void decompressHuffmanBlock(CodeTree litLenCode, CodeTree distCode) throws IOException, DataFormatException {
		if (litLenCode == null)
			throw new NullPointerException();
		
		while (true) {
			int sym = decodeSymbol(litLenCode);
			if (sym == 256)  // End of block
				break;
			
			if (sym < 256) {  // Literal byte
				output.write(sym);
				dictionary.append(sym);
			} else {  // Length and distance for copying
				int len = decodeRunLength(sym);
				if (distCode == null)
					throw new DataFormatException("Length symbol encountered with empty distance code");
				int distSym = decodeSymbol(distCode);
				int dist = decodeDistance(distSym);
				dictionary.copy(dist, len, output);
			}
		}
	}
	
	
	/* Symbol decoding methods */
	
	private int decodeSymbol(CodeTree code) throws IOException {
		InternalNode currentNode = code.root;
		while (true) {
			int temp = input.readNoEof();
			Node nextNode;
			if      (temp == 0) nextNode = currentNode.leftChild;
			else if (temp == 1) nextNode = currentNode.rightChild;
			else throw new AssertionError();
			
			if (nextNode instanceof Leaf)
				return ((Leaf)nextNode).symbol;
			else if (nextNode instanceof InternalNode)
				currentNode = (InternalNode)nextNode;
			else
				throw new AssertionError();
		}
	}
	
	private int decodeRunLength(int sym) throws IOException, DataFormatException
    {
		if (sym < 257 || sym > 285)
			throw new DataFormatException("Invalid run length symbol: " + sym);
		else if (sym <= 264)
			return sym - 254;
        else if (sym <= 284)
        {
            int i = (sym - 261) / 4;
            return (((sym - 265) % 4 + 4) << i) + 3 + readInt(i);
		} else
			return 258;
	}
	
	
	private int decodeDistance(int sym) throws IOException, DataFormatException {
		if (sym <= 3)
			return sym + 1;
		else if (sym <= 29) {
            int i = sym / 2 - 1;
			return ((sym % 2 + 2) << i) + 1 + readInt(i);
		} else
			throw new DataFormatException("Invalid distance symbol: " + sym);
	}
	
	private int readInt(int numBits) throws IOException {
		if (numBits < 0 || numBits >= 32)
			throw new IllegalArgumentException();
		
		int result = 0;
		for (int i = 0; i < numBits; i++)
			result |= input.readNoEof() << i;
		return result;
	}
	
}

final class CanonicalCode
{	
	private int[] codeLengths;
	
	public CanonicalCode(int[] codeLengths) {
		if (codeLengths == null)
			throw new NullPointerException("Argument is null");
		this.codeLengths = codeLengths.clone();
		for (int x : codeLengths) {
			if (x < 0)
				throw new IllegalArgumentException("Illegal code length");
		}
	}
	
	public CanonicalCode(CodeTree tree, int symbolLimit) {
		codeLengths = new int[symbolLimit];
		buildCodeLengths(tree.root, 0);
	}
	
	
	private void buildCodeLengths(Node node, int depth) {
		if (node instanceof InternalNode) {
			InternalNode internalNode = (InternalNode)node;
			buildCodeLengths(internalNode.leftChild , depth + 1);
			buildCodeLengths(internalNode.rightChild, depth + 1);
		} else if (node instanceof Leaf) {
			int symbol = ((Leaf)node).symbol;
			if (codeLengths[symbol] != 0)
				throw new AssertionError("Symbol has more than one code");
			if (symbol >= codeLengths.length)
				throw new IllegalArgumentException("Symbol exceeds symbol limit");
			codeLengths[symbol] = depth;
		} else {
			throw new AssertionError("Illegal node type");
		}
	}
	
	
	
	public int getSymbolLimit() {
		return codeLengths.length;
	}
	
	
	public int getCodeLength(int symbol) {
		if (symbol < 0 || symbol >= codeLengths.length)
			throw new IllegalArgumentException("Symbol out of range");
		return codeLengths[symbol];
	}
	
	
	public CodeTree toCodeTree()
    {
		List<Node> nodes = new ArrayList<Node>();
		for (int i = max(codeLengths); i >= 1; i--)
        {
			List<Node> newNodes = new ArrayList<Node>();
			
			for (int j = 0; j < codeLengths.length; j++) {
				if (codeLengths[j] == i)
					newNodes.add(new Leaf(j));
			}
			
			for (int j = 0; j < nodes.size(); j += 2)
				newNodes.add(new InternalNode(nodes.get(j), nodes.get(j + 1)));
			
			nodes = newNodes;
			if (nodes.size() % 2 != 0)
				throw new IllegalStateException("This cano code doesnt repr a Huffman code tree");
		}
		
		if (nodes.size() != 2)
			throw new IllegalStateException("This canonical code doesnt repr a Huffman code tree");
		return new CodeTree(new InternalNode(nodes.get(0), nodes.get(1)), codeLengths.length);
	}
	
	
	private static int max(int[] array)
    {
		int result = array[0];
		for (int x : array)
			result = Math.max(x, result);
		return result;
	}
	
}



final class CircularDictionary {
	
	private byte[] data;
	
	private int index;
	
	private int mask;
	
	
	
	public CircularDictionary(int size) {
		data = new byte[size];
		index = 0;
		
		if (size > 0 && (size & (size - 1)) == 0)  // Test if size is a power of 2
			mask = size - 1;
		else
			mask = 0;
	}
	
	
	
	public void append(int b) {
		data[index] = (byte)b;
		if (mask != 0)
			index = (index + 1) & mask;
		else
			index = (index + 1) % data.length;
	}
	
	
	public void copy(int dist, int len, OutputStream out) throws IOException {
		if (len < 0 || dist < 1 || dist > data.length)
			throw new IllegalArgumentException();
		
		if (mask != 0) {
			int readIndex = (index - dist + data.length) & mask;
			for (int i = 0; i < len; i++) {
				out.write(data[readIndex]);
				data[index] = data[readIndex];
				readIndex = (readIndex + 1) & mask;
				index = (index + 1) & mask;
			}
		} else {
			int readIndex = (index - dist + data.length) % data.length;
			for (int i = 0; i < len; i++) {
				out.write(data[readIndex]);
				data[index] = data[readIndex];
				readIndex = (readIndex + 1) % data.length;
				index = (index + 1) % data.length;
			}
		}
	}
	
}

final class CodeTree
{
	public final InternalNode root;
	private List<List<Integer>> codes;
	
	public CodeTree(InternalNode root, int symbolLimit)
    {
		if (root == null)
			throw new NullPointerException("Argument is null");
		this.root = root;
		
		codes = new ArrayList<List<Integer>>();
		for (int i = 0; i < symbolLimit; i++)
			codes.add(null);
		buildCodeList(root, new ArrayList<Integer>());
	}
	
	
	private void buildCodeList(Node node, List<Integer> prefix)
    {
		if (node instanceof InternalNode)
        {
			InternalNode internalNode = (InternalNode)node;
			prefix.add(0);
			buildCodeList(internalNode.leftChild , prefix);
			prefix.remove(prefix.size() - 1);
			prefix.add(1);
			buildCodeList(internalNode.rightChild, prefix);
			prefix.remove(prefix.size() - 1);
			
		}
        else if (node instanceof Leaf)
        {
			Leaf leaf = (Leaf)node;
			if (leaf.symbol >= codes.size())
				throw new IllegalArgumentException("Symbol exceeds symbol limit");
			if (codes.get(leaf.symbol) != null)
				throw new IllegalArgumentException("Symbol has more than one code");
			codes.set(leaf.symbol, new ArrayList<Integer>(prefix));
		}
        else
        {
			throw new AssertionError("Illegal node type");
		}
	}
	
	
	
	public List<Integer> getCode(int symbol) {
		if (symbol < 0)
			throw new IllegalArgumentException("Illegal symbol");
		else if (codes.get(symbol) == null)
			throw new IllegalArgumentException("No code for given symbol");
		else
			return codes.get(symbol);
	}
	
	public String toString()
    {
		StringBuilder sb = new StringBuilder();
		toString("", root, sb);
		return sb.toString();
	}
	
	private static void toString(String prefix, Node node, StringBuilder sb) {
		if (node instanceof InternalNode) {
			InternalNode internalNode = (InternalNode)node;
			toString(prefix + "0", internalNode.leftChild , sb);
			toString(prefix + "1", internalNode.rightChild, sb);
		} else if (node instanceof Leaf) {
			sb.append(String.format("Code %s: Symbol %d%n", prefix, ((Leaf)node).symbol));
		} else {
			throw new AssertionError("Illegal node type");
		}
	}
}

abstract class Node {
	
	public Node() {}
}

final class InternalNode extends Node {
	
	public final Node leftChild;  // Not null	
	public final Node rightChild;  // Not null
	
	public InternalNode(Node leftChild, Node rightChild) {
		if (leftChild == null || rightChild == null)
			throw new NullPointerException("Argument is null");
		this.leftChild = leftChild;
		this.rightChild = rightChild;
	}
}

final class Leaf extends Node
{	
	public final int symbol;
	
	public Leaf(int symbol)
    {
		if (symbol < 0)
			throw new IllegalArgumentException("Illegal symbol value");
		this.symbol = symbol;
	}
}





/**
 * A stream of bits that can be read.
 */
final class ByteBitInputStream implements BitInputStream {
	
	private InputStream input;  // Underlying byte stream to read from
	
	private int nextBits;  // Either in the range 0x00 to 0xFF, or -1 if the end of stream is reached
	
	private int bitPosition;  // Always between 1 and 8, inclusive
	
	private boolean isEndOfStream;
	
	
	
	public ByteBitInputStream(InputStream in) {
		if (in == null)
			throw new NullPointerException("Argument is null");
		input = in;
		bitPosition = 8;
		isEndOfStream = false;
	}
	
	
	
	// Reads a bit from the stream. Returns 0 or 1 if a bit is available, or -1 if the end of stream is reached. The end of stream always occurs on a byte boundary.
	public int read() throws IOException {
		if (isEndOfStream)
			return -1;
		if (bitPosition == 8) {
			nextBits = input.read();
			if (nextBits == -1) {
				isEndOfStream = true;
				return -1;
			}
			bitPosition = 0;
		}
		int result = (nextBits >>> bitPosition) & 1;
		bitPosition++;
		return result;
	}
	
	
	// Reads a bit from the stream. Returns 0 or 1 if a bit is available, or throws an EOFException if the end of stream is reached.
	public int readNoEof() throws IOException {
		int result = read();
		if (result != -1)
			return result;
		else
			throw new EOFException("End of stream reached");
	}
	
	
	// Returns the current bit position, which is between 0 and 7 inclusive. The number of bits remaining in the current byte is 8 minus this number.
	public int getBitPosition() {
		return bitPosition % 8;
	}
	
	
	// Discards the remainder of the current byte and reads the next byte from the stream.
	public int readByte() throws IOException {
		bitPosition = 8;
		return input.read();
	}
	
	
	// Closes this stream and the underlying InputStream.
	public void close() throws IOException {
		input.close();
	}
	
}

interface BitInputStream {
	
	// Reads a bit from the stream. Returns 0 or 1 if a bit is available, or -1 if the end of stream is reached. The end of stream always occurs on a byte boundary.
	public int read() throws IOException;
	
	
	// Reads a bit from the stream. Returns 0 or 1 if a bit is available, or throws an EOFException if the end of stream is reached.
	public int readNoEof() throws IOException;
	
	
	// Closes this stream and the underlying InputStream.
	public void close() throws IOException;
	
	
	// Returns the current bit position, which is between 0 and 7 inclusive. The number of bits remaining in the current byte is 8 minus this number.
	public int getBitPosition();
	
	
	// Discards the remainder of the current byte and reads the next byte from the stream.
	public int readByte() throws IOException;
	
}
