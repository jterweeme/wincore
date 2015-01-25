import java.io.IOException;

class GzipStream
{
    private BitInput _bi;
    GzipStream(BitInput bi) { _bi = bi; }

    void extractTo(java.io.OutputStream os) throws IOException
    {
        byte[] x = new byte[10];
        _bi.read(x);
        byte flags = x[3];

        if ((flags & 0x01) != 0)
            System.out.println("Flag: Text");

        if ((flags & 0x04) != 0)
        {
            System.out.println("Flag: Extra");
            byte[] b = new byte[2];
            _bi.read(b);
            int len = b[0] & 0xFF | (b[1] & 0xFF) << 8;
            _bi.read(new byte[len]);
        }

        if ((flags & 0x08) != 0)
            System.out.println("File name: " + _readString());

        if ((flags & 0x02) != 0)
        {
            byte[] b = new byte[2];
            _bi.read(b);
            System.out.printf("Header CRC-16: %04X%n", b[0] & 0xFF | (b[1] & 0xFF) << 8);
        }

        if ((flags & 0x10) != 0)
            System.out.println("Comment: " + _readString());

        Decompressor d = new Decompressor(_bi);
        d.extractTo(os);
    }

    private String _readString() throws IOException
    {
        StringBuilder sb = new StringBuilder();
        for (int c = _bi.readByte(); c != 0; c = _bi.readByte()) sb.append((char)c);
        return sb.toString();
    }
}

class Pair
{
    CodeTree a, b;
    Pair(CodeTree a, CodeTree b) { this.a = a; this.b = b; }
}

class Decompressor
{
    private BitInput _bi;
    private CircularDict _dict;
    private final CodeTree _lit, _dist;

    private CodeTree _toct(int[] cl)
    {
        java.util.List<Node> nodes = new java.util.ArrayList<Node>();

        for (int i = _max(cl); i >= 1; i--)
        {
            java.util.List<Node> newNodes = new java.util.ArrayList<Node>();

            for (int j = 0; j < cl.length; j++)
                if (cl[j] == i)
                    newNodes.add(new Node(j));

            for (int j = 0; j < nodes.size(); j += 2)
                newNodes.add(new Node(nodes.get(j), nodes.get(j + 1)));

            nodes = newNodes;
        }

        return new CodeTree(new Node(nodes.get(0), nodes.get(1)), cl.length);
    }

    private int _max(int[] array)
    {
        int result = array[0];
        for (int x : array) result = Math.max(x, result);
        return result;
    }

    void extractTo(java.io.OutputStream os) throws IOException
    {
        for (boolean isFinal = false; !isFinal;)
        {
            isFinal = _bi.readBool();
            int type = _bi.readInt(2);

            switch (type)
            {
            case 0:
                _decRaw(os);
                break;
            case 1:
                _decHuff(_lit.root, _dist.root, os);
                break;
            case 2:
                Pair temp = _makePair();
                _decHuff(temp.a.root, temp.b.root, os);
                break;
            default:
                throw new AssertionError();
            }
        }
    }

    Decompressor(BitInput in) throws IOException
    {
        _dict = new CircularDict(32 * 1024);
        int[] llcodelens = new int[288];
        java.util.Arrays.fill(llcodelens,   0, 144, 8);
        java.util.Arrays.fill(llcodelens, 144, 256, 9);
        java.util.Arrays.fill(llcodelens, 256, 280, 7);
        java.util.Arrays.fill(llcodelens, 280, 288, 8);
        _lit = _toct(llcodelens);
        int[] distcodelens = new int[32];
        java.util.Arrays.fill(distcodelens, 5);
        _dist = _toct(distcodelens);
        _bi = in;
    }

    private Pair _makePair() throws IOException
    {
        int nlit = _bi.readInt(5) + 257, ndist = _bi.readInt(5) + 1, ncode = _bi.readInt(4) + 4;
        int[] codeLenCodeLen = new int[19];
        codeLenCodeLen[16] = _bi.readInt(3);
        codeLenCodeLen[17] = _bi.readInt(3);
        codeLenCodeLen[18] = _bi.readInt(3);
        codeLenCodeLen[ 0] = _bi.readInt(3);

        for (int i = 0; i < ncode - 4; i++)
        {
            int j = i % 2 == 0 ? 8 + i / 2 : 7 - i / 2;
            codeLenCodeLen[j] = _bi.readInt(3);
        }

        CodeTree codeLenCode = _toct(codeLenCodeLen);
        int[] codeLens = new int[nlit + ndist];

        for (int i = 0, runVal = -1, runLen = 0; i < codeLens.length; i++)
        {
            if (runLen > 0)
            {
                codeLens[i] = runVal;
                runLen--;	
            }
            else
            {
                int sym = _decSym(codeLenCode.root);

                switch (sym)
                {
                case 16:
                    runLen = _bi.readInt(2) + 3;
                    i--;
                    break;
                case 17:
                    runVal = 0;
                    runLen = _bi.readInt(3) + 3;
                    i--;
                    break;
                case 18:
                    runVal = 0;
                    runLen = _bi.readInt(7) + 11;
                    i--;
                    break;
                default:
                    codeLens[i] = sym;
                    runVal = sym;
                }
            }
        }

        int[] litLenCodeLen = java.util.Arrays.copyOf(codeLens, nlit);
        CodeTree litLenCode = _toct(litLenCodeLen);
        int[] distCodeLen = java.util.Arrays.copyOfRange(codeLens, nlit, codeLens.length);
        CodeTree distCode;

        if (distCodeLen.length == 1 && distCodeLen[0] == 0)
        {
            distCode = null;
        }
        else
        {
            int oneCount = 0, otherPositiveCount = 0;

            for (int x : distCodeLen)
                if (x == 1) oneCount++; else if (x > 1) otherPositiveCount++;

            if (oneCount == 1 && otherPositiveCount == 0)
            {
                distCodeLen = java.util.Arrays.copyOf(distCodeLen, 32);
                distCodeLen[31] = 1;
            }

            distCode = _toct(distCodeLen);
        }

        return new Pair(litLenCode, distCode);
    }

    private void _decRaw(java.io.OutputStream os) throws IOException
    {
        _bi.ignoreBuf();
        int len = _bi.readInt(16);
        _bi.ignore(16);
		
        for (int i = 0; i < len; i++)
        {
            int temp = _bi.readByte();
            os.write(temp);
            _dict.append(temp);
        }
    }

    private void _decHuff(Node lit, Node dist, java.io.OutputStream os) throws IOException
    {
        while (true)
        {
            int sym = _decSym(lit);

            if (sym == 256)
                break;

            if (sym < 256)
            {
                os.write(sym);
                _dict.append(sym);
            }
            else
            {
                int len = _decRll(sym), distSym = _decSym(dist);
                _dict.copy(_decDist(distSym), len, os);
            }
        }
    }

    private int _decSym(Node n) throws IOException
    {
        Node next = _bi.readBool() ? n.right : n.left;

        for (n = next; next.type == 1; n = next)
            next = _bi.readBool() ? n.right : n.left;

        return next.symbol;
    }

    private int _decRll(int sym) throws IOException
    {
        int i = (sym - 261) / 4;
        if (sym <= 264) return sym - 254;
        if (sym <= 284) return ((sym - 265) % 4 + 4 << i) + 3 + _bi.readInt(i);
        return 258;
    }

    private int _decDist(int sym) throws IOException
    {
        int i = sym / 2 - 1;
        return sym <= 3 ? sym + 1 : (sym % 2 + 2 << i) + 1 + _bi.readInt(i);
    }
}

class CircularDict
{
    private byte[] _data;
    private int _index, _mask;

    CircularDict(int size)
    {
        _data = new byte[size];
        _index = 0;
        _mask = size > 0 && (size & (size - 1)) == 0 ? size - 1 : 0;
    }
    
    void append(int b)
    {
        _data[_index] = (byte)b;
        _index = _mask != 0 ? (_index + 1) & _mask : (_index + 1) % _data.length;
	}

    void copy(int dist, int len, java.io.OutputStream out) throws IOException
    {
        if (_mask != 0)
        {
            for (int i = 0, readIndex = (_index - dist + _data.length) & _mask; i < len; i++)
            {
                out.write(_data[readIndex]);
                _data[_index] = _data[readIndex];
                readIndex = (readIndex + 1) & _mask;
                _index = (_index + 1) & _mask;
            }
        }
        else
        {
            for (int i = 0, j = (_index - dist + _data.length) % _data.length; i < len; i++)
            {
                out.write(_data[j]);
                _data[_index] = _data[j];
                j = (j + 1) % _data.length;
                _index = (_index + 1) % _data.length;
            }
        }
    }
}

class CodeTree
{
    final Node root;
    private java.util.List<java.util.List<Integer>> _codes;

    CodeTree(Node root, int symbolLimit)
    {
        this.root = root;
        _codes = new java.util.ArrayList<java.util.List<Integer>>();
        for (int i = 0; i < symbolLimit; i++) _codes.add(null);
        _buildCodeList(root, new java.util.ArrayList<Integer>());
    }

    private void _buildCodeList(Node node, java.util.List<Integer> prefix)
    {
        if (node.type == 1)
        {
            prefix.add(0);
            _buildCodeList(node.left , prefix);
            prefix.remove(prefix.size() - 1);
            prefix.add(1);
            _buildCodeList(node.right, prefix);
            prefix.remove(prefix.size() - 1);
        }
        else if (node.type == 2)
            _codes.set(node.symbol, new java.util.ArrayList<Integer>(prefix));
    }
}

class Node
{
    Node left, right;
    final int symbol, type;
    Node(Node l, Node r) { symbol = 0; left = l; right = r; type = 1; }
    Node(int s) { symbol = s; type = 2; }
}

class BitInput
{
    java.io.InputStream _is;
    int _nextBits, _bitPos = 8;
    int _getBitPos() { return _bitPos % 8; }
    BitInput(java.io.InputStream is) { _is = is; }
    boolean readBool() throws IOException { return readBit() == 1; }
    void ignore(int n) throws IOException { while (n-- > 0) readBool(); }
    void ignoreBuf() throws IOException { ignore(_getBitPos()); }
    int readByte() throws IOException { return _is.read(); }
    int read(byte[] b) throws IOException { return _is.read(b); }
    	
    int readBit() throws IOException
    {
        if (_bitPos == 8)
        {
            _nextBits = _is.read();
            _bitPos = 0;
        }

        return _nextBits >>> _bitPos++ & 1;
    }

    int readInt(int numBits) throws IOException
    {
        int result = 0;
        for (int i = 0; i < numBits; i++) result |= readBit() << i;
        return result;
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
        java.io.BufferedInputStream bis = new java.io.BufferedInputStream(ifs, 16 * 1024);
        java.io.OutputStream ofs = new java.io.FileOutputStream(args[1]);
        java.io.BufferedOutputStream bos = new java.io.BufferedOutputStream(ofs, 16 * 1024);
        BitInput bi = new BitInput(bis);
        GzipStream gz = new GzipStream(bi);
        gz.extractTo(bos);
        bos.close();
        bis.close();
	}
}


