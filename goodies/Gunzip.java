import java.io.IOException;
import java.io.EOFException;
import java.io.InputStream;
import java.util.zip.DataFormatException;
import java.util.Arrays;

class GzipStream
{
    BitInput _is;
    public GzipStream(java.io.InputStream is) { this(new BitInput(is)); }
    public GzipStream(BitInput is) { _is = is; }

    public void extractTo(java.io.OutputStream os) throws IOException, DataFormatException
    {
        byte[] x = new byte[10];
        _is.read(x);
        byte flags = x[3];

        if ((flags & 0x01) != 0)
            System.out.println("Flag: Text");

        if ((flags & 0x04) != 0)
        {
            System.out.println("Flag: Extra");
            byte[] b = new byte[2];
            _is.read(b);
            int len = b[0] & 0xFF | (b[1] & 0xFF) << 8;
            _is.read(new byte[len]);
        }

        if ((flags & 0x08) != 0)
            System.out.println("File name: " + readNullTerminatedString());

        if ((flags & 0x02) != 0)
        {
            byte[] b = new byte[2];
            _is.read(b);
            System.out.printf("Header CRC-16: %04X%n", b[0] & 0xFF | (b[1] & 0xFF) << 8);
        }

        if ((flags & 0x10) != 0)
            System.out.println("Comment: " + readNullTerminatedString());

        Decompressor d = new Decompressor(_is);
        d.extractTo(os);
    }

    String readNullTerminatedString() throws IOException
    {
        StringBuilder sb = new StringBuilder();

        for (int c = _is.readByte(); c != 0; c = _is.readByte())
            sb.append((char)c);

        return sb.toString();
    }
}

class Decompressor
{
    BitInput _bi;
    java.io.ByteArrayOutputStream _output;
    CircularDictionary _dictionary;
    CodeTree _lit, _dist;
    int[] llcodelens = new int[288];

    void extractTo(java.io.OutputStream o) throws IOException, DataFormatException
    {
        for (boolean isFinal = false; !isFinal;)
        {
            isFinal = _bi.readBool();
            int type = readInt(2);
            CodeTree litLenCode, distCode;

            switch (type)
            {
            case 0:
                decompressUncompressedBlock();
                break;
            case 1:
                decompressHuffmanBlock(_lit, _dist);
                break;
            case 2:
                CodeTree[] temp = decodeHuffmanCodes();
                litLenCode = temp[0];
                distCode = temp[1];
                decompressHuffmanBlock(litLenCode, distCode);
                break;
            case 3:
                throw new DataFormatException("Invalid block type");
            default:
                throw new AssertionError();
            }
        }

        byte[] decomp = _output.toByteArray();
        o.write(decomp);
    }

    Decompressor(BitInput in) throws IOException, DataFormatException
    {
        Arrays.fill(llcodelens,   0, 144, 8);
        Arrays.fill(llcodelens, 144, 256, 9);
        Arrays.fill(llcodelens, 256, 280, 7);
        Arrays.fill(llcodelens, 280, 288, 8);
        _lit = new CanonicalCode(llcodelens).toCodeTree();
        int[] distcodelens = new int[32];
        Arrays.fill(distcodelens, 5);
        _dist = new CanonicalCode(distcodelens).toCodeTree();
        _bi = in;
        _output = new java.io.ByteArrayOutputStream();
        _dictionary = new CircularDictionary(32 * 1024);
    }

    CodeTree[] decodeHuffmanCodes() throws IOException, DataFormatException
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

        CodeTree codeLenCode = new CanonicalCode(codeLenCodeLen).toCodeTree();
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
        CodeTree litLenCode = new CanonicalCode(litLenCodeLen).toCodeTree();
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

            distCode = new CanonicalCode(distCodeLen).toCodeTree();
        }

        return new CodeTree[]{litLenCode, distCode};
    }

    void decompressUncompressedBlock() throws IOException, DataFormatException
    {
        _bi.ignoreBuf();
        int len = readInt(16), nlen = readInt(16);

        if ((len ^ 0xFFFF) != nlen)
            throw new DataFormatException("Invalid length in uncompressed block");
		
        for (int i = 0; i < len; i++)
        {
            int temp = _bi.readByte();

            if (temp == -1)
                throw new EOFException();

            _output.write(temp);
            _dictionary.append(temp);
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
                _output.write(sym);
                _dictionary.append(sym);
            }
            else
            {
                int len = decodeRunLength(sym);

                if (distCode == null)
                    throw new DataFormatException("Length sym encountered with empty dist code");

                int distSym = decodeSymbol(distCode), dist = decodeDistance(distSym);
                _dictionary.copy(dist, len, _output);
            }
        }
    }
	
    int decodeSymbol(CodeTree code) throws IOException
    {
        InternalNode currentNode = code.root;

        while (true)
        {
			Node nextNode = _bi.readBool() ? currentNode.rightChild : currentNode.leftChild;

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
        {
            return sym + 1;
        }
        else if (sym <= 29)
        {
            int i = sym / 2 - 1;
            return ((sym % 2 + 2) << i) + 1 + readInt(i);
        }
        else
        {
            throw new DataFormatException("Invalid distance symbol: " + sym);
        }
    }

    int readInt(int numBits) throws IOException
    {
        if (numBits < 0 || numBits >= 32)
            throw new IllegalArgumentException();

        int result = 0;

        for (int i = 0; i < numBits; i++)
            result |= _bi.readBit() << i;

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
        java.util.List<Node> nodes = new java.util.ArrayList<Node>();

        for (int i = max(codeLengths); i >= 1; i--)
        {
            java.util.List<Node> newNodes = new java.util.ArrayList<Node>();

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
            for (int i = 0, readIndex = (index - dist + data.length) & mask; i < len; i++)
            {
                out.write(data[readIndex]);
                data[index] = data[readIndex];
                readIndex = (readIndex + 1) & mask;
                index = (index + 1) & mask;
            }
        }
        else
        {
            for (int i = 0, readIndex = (index - dist + data.length) % data.length; i < len; i++)
            {
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
    java.util.List<java.util.List<Integer>> codes;

    public CodeTree(InternalNode root, int symbolLimit)
    {
        if (root == null)
            throw new NullPointerException("Argument is null");

        this.root = root;
        codes = new java.util.ArrayList<java.util.List<Integer>>();

        for (int i = 0; i < symbolLimit; i++)
            codes.add(null);

        buildCodeList(root, new java.util.ArrayList<Integer>());
    }

    void buildCodeList(Node node, java.util.List<Integer> prefix)
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

    public java.util.List<Integer> getCode(int symbol)
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
    public final Node leftChild, rightChild;

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

class BitInput
{
    java.io.InputStream _is;
    int _nextBits, _bitPos = 8;
    int _getBitPos() { return _bitPos % 8; }
    public BitInput(java.io.InputStream is) { _is = is; }
    public boolean readBool() throws IOException { return readBit() == 1; }
    public void ignore(int n) throws IOException { while (n-- > 0) readBool(); }
    public void ignoreBuf() throws IOException { ignore(_getBitPos()); }
    public int readByte() throws IOException { return _is.read(); }
    public int read(byte[] b) throws IOException { return _is.read(b); }
    	
    public int readBit() throws IOException
    {
        if (_bitPos == 8)
        {
            _nextBits = _is.read();
            _bitPos = 0;
        }

        return _nextBits >>> _bitPos++ & 1;
    }
}

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

        java.io.FileInputStream ifs = new java.io.FileInputStream(args[0]);
        java.io.OutputStream ofs = new java.io.FileOutputStream(args[1]);
        GzipStream gz = new GzipStream(ifs);
        gz.extractTo(ofs);
        ofs.close();
        ifs.close();
	}
}


