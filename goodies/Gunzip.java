import java.io.IOException;
import java.io.EOFException;
import java.util.zip.DataFormatException;
import java.io.ByteArrayOutputStream;
import java.util.Arrays;
import java.util.zip.DataFormatException;
import java.util.List;

public class Gunzip
{
    public static void main(String[] args)
    {
        Gunzip g = new Gunzip();

        try
        {
            g.run(args);
        }
        catch (Exception e)
        {
            System.err.println(e.getMessage());
            System.exit(1);
        }
    }

    void run(String[] args) throws Exception
    {
        if (args.length != 2)
            throw new Exception("Usage: java GzipDecompress InputFile OutputFile");

        java.io.File inFile = new java.io.File(args[0]);
        java.io.FileInputStream fis = new java.io.FileInputStream(inFile);
        java.io.DataInputStream in = new java.io.DataInputStream(fis);
        byte[] decomp, x = new byte[10];
        int flags;
        in.readFully(x);
        flags = x[3] & 0xFF;

        if ((flags & 0xE0) != 0)
            throw new Exception("Reserved flags are set");
					
        int m = x[4] & 0xFF | (x[5] & 0xFF) << 8 | (x[6] & 0xFF) << 16 | x[7] << 24;

        if (m != 0)
            System.out.println("Last modified: " + new java.util.Date(m * 1000L));
        else
            System.out.println("Last modified: N/A");
					
        switch (x[8] & 0xFF)
        {
            case 2:
                System.out.println("Extra flags: Maximum compression");
                break;
            case 4:
                System.out.println("Extra flags: Fastest compression");
                break;
            default:
                System.out.println("Extra flags: Unknown (" + (x[8] & 0xFF) + ")");
                break;
        }

        if ((flags & 0x01) != 0)
            System.out.println("Flag: Text");

        if ((flags & 0x04) != 0)
        {
            System.out.println("Flag: Extra");
            byte[] b = new byte[2];
            in.readFully(b);
            int len = b[0] & 0xFF | (b[1] & 0xFF) << 8;
            in.readFully(new byte[len]);
        }

        if ((flags & 0x08) != 0)
            System.out.println("File name: " + readNullTerminatedString(in));

        if ((flags & 0x02) != 0)
        {
            byte[] b = new byte[2];
            in.readFully(b);
            System.out.printf("Header CRC-16: %04X%n", b[0] & 0xFF | (b[1] & 0xFF) << 8);
        }

        if ((flags & 0x10) != 0)
            System.out.println("Comment: " + readNullTerminatedString(in));

        Decompressor d = new Decompressor(new ByteBitInputStream(in));
        decomp = d.output.toByteArray();
        java.io.File outFile = new java.io.File(args[1]);
        java.io.OutputStream out = new java.io.FileOutputStream(outFile);
        out.write(decomp);
        out.close();
	}

    String readNullTerminatedString(java.io.DataInput in) throws IOException
    {
        StringBuilder sb = new StringBuilder();

        while (true)
        {
            byte c = in.readByte();
            if (c == 0) break; else sb.append((char)(c & 0xFF));
        }
        return sb.toString();
    }

    int getCrc32(byte[] data)
    {
        java.util.zip.CRC32 crc = new java.util.zip.CRC32();
        crc.update(data);
        return (int)crc.getValue();
    }	
}

class Decompressor
{
    ByteBitInputStream input;
    ByteArrayOutputStream output;
    CircularDictionary dictionary;
    CodeTree fixedLiteralLengthCode, fixedDistanceCode;
    int[] llcodelens = new int[288];

    Decompressor(ByteBitInputStream in) throws IOException, DataFormatException
    {
        Arrays.fill(llcodelens,   0, 144, 8);
        Arrays.fill(llcodelens, 144, 256, 9);
        Arrays.fill(llcodelens, 256, 280, 7);
        Arrays.fill(llcodelens, 280, 288, 8);
        fixedLiteralLengthCode = new CanonicalCode(llcodelens).toCodeTree();
        int[] distcodelens = new int[32];
        Arrays.fill(distcodelens, 5);
        fixedDistanceCode = new CanonicalCode(distcodelens).toCodeTree();
        input = in;
        output = new ByteArrayOutputStream();
        dictionary = new CircularDictionary(32 * 1024);

        while (true)
        {
            boolean isFinal = in.readNoEof() == 1;
            int type = readInt(2);

            if (type == 0)
            {
                decompressUncompressedBlock();
            }
            else if (type == 1 || type == 2)
            {
                CodeTree litLenCode, distCode;

                if (type == 1)
                {
                    litLenCode = fixedLiteralLengthCode;
                    distCode = fixedDistanceCode;
                }
                else
                {
                    CodeTree[] temp = decodeHuffmanCodes(in);
                    litLenCode = temp[0];
                    distCode = temp[1];
                }
                decompressHuffmanBlock(litLenCode, distCode);
            }
            else if (type == 3)
            {
                throw new DataFormatException("Invalid block type");
            }
            else
            {
                throw new AssertionError();
            }

            if (isFinal)
                break;
        }
    }

    CodeTree[] decodeHuffmanCodes(ByteBitInputStream in) throws IOException, DataFormatException
    {
        int numLitLenCodes = readInt(5) + 257;
        int numDistCodes = readInt(5) + 1;
        int numCodeLenCodes = readInt(4) + 4;
        int[] codeLenCodeLen = new int[19];
        codeLenCodeLen[16] = readInt(3);
        codeLenCodeLen[17] = readInt(3);
        codeLenCodeLen[18] = readInt(3);
        codeLenCodeLen[ 0] = readInt(3);

        for (int i = 0; i < numCodeLenCodes - 4; i++)
        {
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
        int runVal = -1, runLen = 0;

        for (int i = 0; i < codeLens.length; i++)
        {
            if (runLen > 0)
            {
                codeLens[i] = runVal;
                runLen--;	
			}
            else
            {
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
        {
            distCode = null;
        }
        else
        {
            int oneCount = 0, otherPositiveCount = 0;

            for (int x : distCodeLen)
            {
				if (x == 1)
					oneCount++;
				else if (x > 1)
					otherPositiveCount++;
            }

			if (oneCount == 1 && otherPositiveCount == 0)
            {
				distCodeLen = Arrays.copyOf(distCodeLen, 32);
				distCodeLen[31] = 1;
			}
			
			try
            {
				distCode = new CanonicalCode(distCodeLen).toCodeTree();
			}
            catch (IllegalStateException e)
            {
				throw new DataFormatException(e.getMessage());
			}
		}
		
		return new CodeTree[]{litLenCode, distCode};
	}
	
    void decompressUncompressedBlock() throws IOException, DataFormatException
    {
        while (input.getBitPosition() != 0)
            input.readNoEof();
		
        int len = readInt(16), nlen = readInt(16);

        if ((len ^ 0xFFFF) != nlen)
            throw new DataFormatException("Invalid length in uncompressed block");
		
        for (int i = 0; i < len; i++)
        {
            int temp = input.readByte();

            if (temp == -1)
                throw new EOFException();

            output.write(temp);
            dictionary.append(temp);
        }
    }

    void decompressHuffmanBlock(CodeTree litLenCode, CodeTree distCode)
            throws IOException, DataFormatException
    {
        if (litLenCode == null)
            throw new NullPointerException();
		
        while (true)
        {
            int sym = decodeSymbol(litLenCode);

            if (sym == 256)
                break;

            if (sym < 256)
            {
                output.write(sym);
                dictionary.append(sym);
            }
            else
            {
                int len = decodeRunLength(sym);

                if (distCode == null)
                    throw new DataFormatException("Length sym encountered with empty dist code");

                int distSym = decodeSymbol(distCode), dist = decodeDistance(distSym);
                dictionary.copy(dist, len, output);
            }
        }
    }
	
    int decodeSymbol(CodeTree code) throws IOException
    {
        InternalNode currentNode = code.root;

        while (true)
        {
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

    int decodeRunLength(int sym) throws IOException, DataFormatException
    {
        if (sym < 257 || sym > 285)
            throw new DataFormatException("Invalid run length symbol: " + sym);
        else if (sym <= 264)
            return sym - 254;
        else if (sym <= 284)
        {
            int i = (sym - 261) / 4;
            return (((sym - 265) % 4 + 4) << i) + 3 + readInt(i);
        }
        else
            return 258;
    }

    int decodeDistance(int sym) throws IOException, DataFormatException
    {
        if (sym <= 3)
            return sym + 1;
        else if (sym <= 29)
        {
            int i = sym / 2 - 1;
            return ((sym % 2 + 2) << i) + 1 + readInt(i);
        }
        else
            throw new DataFormatException("Invalid distance symbol: " + sym);
    }

    int readInt(int numBits) throws IOException
    {
        if (numBits < 0 || numBits >= 32)
            throw new IllegalArgumentException();

        int result = 0;
        for (int i = 0; i < numBits; i++)
            result |= input.readNoEof() << i;
        return result;
    }
}

class CanonicalCode
{	
    int[] codeLengths;

    public CanonicalCode(int[] codeLengths)
    {
        if (codeLengths == null)
            throw new NullPointerException("Argument is null");

        this.codeLengths = codeLengths.clone();

        for (int x : codeLengths)
            if (x < 0)
                throw new IllegalArgumentException("Illegal code length");
    }
	
    public CanonicalCode(CodeTree tree, int symbolLimit)
    {
        codeLengths = new int[symbolLimit];
        buildCodeLengths(tree.root, 0);
    }

    void buildCodeLengths(Node node, int depth)
    {
        if (node instanceof InternalNode)
        {
            InternalNode internalNode = (InternalNode)node;
            buildCodeLengths(internalNode.leftChild , depth + 1);
            buildCodeLengths(internalNode.rightChild, depth + 1);
        }
        else if (node instanceof Leaf)
        {
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

    public int getSymbolLimit()
    {
        return codeLengths.length;
    }

    public int getCodeLength(int symbol)
    {
        if (symbol < 0 || symbol >= codeLengths.length)
            throw new IllegalArgumentException("Symbol out of range");
        return codeLengths[symbol];
    }

    public CodeTree toCodeTree()
    {
        List<Node> nodes = new java.util.ArrayList<Node>();

        for (int i = max(codeLengths); i >= 1; i--)
        {
            List<Node> newNodes = new java.util.ArrayList<Node>();

            for (int j = 0; j < codeLengths.length; j++)
                if (codeLengths[j] == i)
                    newNodes.add(new Leaf(j));

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

    int max(int[] array)
    {
        int result = array[0];

        for (int x : array)
            result = Math.max(x, result);

        return result;
    }
}

class CircularDictionary
{
    byte[] data;
    int index, mask;
	
    public CircularDictionary(int size)
    {
        data = new byte[size];
        index = 0;
        mask = size > 0 && (size & (size - 1)) == 0 ? size - 1 : 0;
    }
    
    public void append(int b)
    {
        data[index] = (byte)b;
        index = mask != 0 ? (index + 1) & mask : (index + 1) % data.length;
	}
	
    public void copy(int dist, int len, java.io.OutputStream out) throws IOException
    {
        if (len < 0 || dist < 1 || dist > data.length)
            throw new IllegalArgumentException();

        if (mask != 0)
        {
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

class CodeTree
{
    public final InternalNode root;
    List<List<Integer>> codes;

    public CodeTree(InternalNode root, int symbolLimit)
    {
        if (root == null)
            throw new NullPointerException("Argument is null");

        this.root = root;
        codes = new java.util.ArrayList<List<Integer>>();

        for (int i = 0; i < symbolLimit; i++)
            codes.add(null);

        buildCodeList(root, new java.util.ArrayList<Integer>());
    }

    void buildCodeList(Node node, List<Integer> prefix)
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

            codes.set(leaf.symbol, new java.util.ArrayList<Integer>(prefix));
        }
        else
        {
            throw new AssertionError("Illegal node type");
        }
    }

    public List<Integer> getCode(int symbol)
    {
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
	
    void toString(String prefix, Node node, StringBuilder sb)
    {
        if (node instanceof InternalNode)
        {
            InternalNode internalNode = (InternalNode)node;
            toString(prefix + "0", internalNode.leftChild , sb);
            toString(prefix + "1", internalNode.rightChild, sb);
        }
        else if (node instanceof Leaf)
        {
            sb.append(String.format("Code %s: Symbol %d%n", prefix, ((Leaf)node).symbol));
        }
        else
        {
            throw new AssertionError("Illegal node type");
        }
    }
}

abstract class Node
{
    public Node() {}
}

class InternalNode extends Node
{
    public final Node leftChild;
    public final Node rightChild;

    public InternalNode(Node leftChild, Node rightChild)
    {
        if (leftChild == null || rightChild == null)
            throw new NullPointerException("Argument is null");

        this.leftChild = leftChild;
        this.rightChild = rightChild;
    }
}

class Leaf extends Node
{	
    public final int symbol;

    public Leaf(int symbol)
    {
        if (symbol < 0)
            throw new IllegalArgumentException("Illegal symbol value");
        this.symbol = symbol;
    }
}

class ByteBitInputStream
{
    java.io.InputStream input;
    int nextBits, bitPosition;
    boolean isEndOfStream;

    public ByteBitInputStream(java.io.InputStream in)
    {
        if (in == null)
            throw new NullPointerException("Argument is null");
        input = in;
        bitPosition = 8;
        isEndOfStream = false;
    }
	
    public int read() throws IOException
    {
        if (isEndOfStream)
            return -1;

        if (bitPosition == 8)
        {
            nextBits = input.read();

            if (nextBits == -1)
            {
				isEndOfStream = true;
				return -1;
            }
            bitPosition = 0;
        }

        int result = (nextBits >>> bitPosition) & 1;
        bitPosition++;
        return result;
    }

    public int readNoEof() throws IOException
    {
        int result = read();

        if (result != -1)
            return result;
        else
            throw new EOFException("End of stream reached");
    }

    public int getBitPosition()
    {
        return bitPosition % 8;
    }

    public int readByte() throws IOException
    {
        bitPosition = 8;
        return input.read();
    }

    public void close() throws IOException
    {
        input.close();
    }
}


