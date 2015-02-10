import java.io.IOException;


/* Bzcat zonder de random */
class Bzcat2
{
    class BitInput
    {
        private final java.io.InputStream _is;
        private int _bitBuffer, _bitCount;
        int readInt() throws IOException { return readBits(16) << 16 | readBits(16); }
        BitInput(final java.io.InputStream is) { _is = is; }
        boolean readBool() throws IOException { return readBits(1) == 0 ? false : true; }
        int readUnary() throws IOException { int u = 0; while (readBool()) u++; return u; }
        void ignore(int n) throws IOException { while (n-- > 0) readBool(); }
        
        int readBits(int n) throws IOException
        {
            for (; _bitCount < n; _bitCount += 8) _bitBuffer = _bitBuffer << 8 | _is.read();
            _bitCount -= n;
            return _bitBuffer >>> _bitCount & (1 << n) - 1;
        }
    }

    class MoveToFront
    {
        private byte[] _buf = new byte[256];
        MoveToFront() { for (int i = 0; i < 256; i++) _buf[i] = (byte)i; }
        void dump(java.io.PrintStream os) { for (int i = 0; i < 256; i++) os.format("%d ", i); }

        private byte indexToFront(int index)
        {
            byte value = _buf[index];
            for (int i = index; i > 0; i--) _buf[i] = _buf[i - 1];
            return _buf[0] = value;
        }
    }

    class Fint
    {
        private int _size;
        private int[] _buf;
        Fint(int size) { _size = size; _buf = new int[size]; }
        int at(int i) { return _buf[i]; }
        int set(int i, int val) { return _buf[i] = val; }
    }

    class Fugt
    {
        private int _size;
        private byte[] _buf;
        private int _pos = 0;
        Fugt(int size) { _size = size; _buf = new byte[size]; }
        byte at(int i) { return _buf[i]; }
        byte set(int i, byte val) { return _buf[i] = val; }
        byte min() { return min(_size); }
        byte max() { return max(_size); }
        byte add(byte val) { return _buf[_pos++] = val; }

        byte min(int range)
        {
            byte a = 0;
            for (int i = 0; i < range; i++) a = (byte)Math.min(_buf[i], a);
            return a;
        }

        byte max(int range)
        {
            byte a = 0;
            for (int i = 0; i < range; i++) a = (byte)Math.max(_buf[i], a);
            return a;
        }
    }

    class Table
    {
        private Fugt _codeLengths2 = new Fugt(258);
        private int _pos = 0;
        private int _symbolCount;
        private Fint _bases2 = new Fint(25);
        private int[] _bases = new int[25], _limits = new int[24], _symbols = new int[258];
        int minLength() { return _minLength(_symbolCount + 2); }
        private int _maxLength() { return _maxLength(_symbolCount + 2); }
        Table(int symbolCount) { _symbolCount = symbolCount; }
        int limit(int i) { return _limits[i]; }
        int symbol(int i) { return _symbols[i]; }
        int base(int i) { return _bases[i]; }
        void add(byte v) { _codeLengths2.set(_pos++, v); }
        private int _minLength(int n) { return _codeLengths2.min(n); }
        private int _maxLength(int n) { return _codeLengths2.max(n); }

        void calc()
        {
            for (int i = 0; i < _symbolCount + 2; i++) _bases[_codeLengths2.at(i) + 1]++;
            for (int i = 1; i < 25; i++) _bases[i] += _bases[i - 1];
            int minLength = minLength();
            int maxLength = _maxLength();

            for (int i = minLength, code = 0; i <= maxLength; i++)
            {
                int base = code;
                code += _bases[i + 1] - _bases[i];
                _bases[i] = base - _bases[i];
                _limits[i] = code - 1;
                code <<= 1;
            }

            for (int n = minLength, i = 0; n <= maxLength; n++)
                for (int symbol = 0; symbol < _symbolCount + 2; symbol++)
                    if (_codeLengths2.at(symbol) == n)
                        _symbols[i++] = symbol;
        }

    }

    class Tables extends java.util.ArrayList<Table>
    {
    }

    class Block2
    {
        private int _repeat, _last, _length, _dec = 0, _curp;
        private int[] _buf;
        Block2() { }
        Block2(int length) { _length = length; _buf = new int[_length]; }
        void set(int i, int val) { _buf[i] = val; }

        private int _nextByte()
        {
            int next = _curp & 0xff;
            _curp = _buf[_curp >>> 8];
            _dec++;
            return next;
        }

        int read()
        {
            return -1;
        }
    }

    class Block
    {
        private int[] _bwtByteCounts, _merged;
        private int _curTbl, _grpIdx, _grpPos, _last, _acc, _repeat, _curp, _length = 0;
        private byte[] _symbolMap, _bwtBlock;
        private int _bwtBytesDecoded = 0;

        private int _nextSymbol(BitInput bi, Tables t, byte[] selectors) throws IOException
        {
            if (++_grpPos % 50 == 0) _curTbl = selectors[++_grpIdx];
            Table table = t.get(_curTbl);

            int n = table.minLength();
            int codeBits = bi.readBits(n);

            for (; n <= 23; n++)
            {
                System.err.println(table.limit(n));

                if (codeBits <= table.limit(n))
                    return table.symbol(codeBits - table.base(n));
                
                codeBits = codeBits << 1 | bi.readBits(1);
            }

            throw new IOException("Error decoding BZip2 block");
        }
       
        private int _nextByte()
        {
            int next = _curp & 0xff;
            _curp = _merged[_curp >>> 8];
            _bwtBytesDecoded++;
            return next;
	    }
        
	    int read()
        {
            while (_repeat < 1)
            {
                if (_bwtBytesDecoded == _length)
                    return -1;
                
                int nextByte = _nextByte();
                
                if (nextByte != _last)
                {
                    _last = nextByte;
                    _repeat = 1;
                    _acc = 1;
                }
                else if (++_acc == 4)
                {
                    _repeat = _nextByte() + 1;
                    _acc = 0;
                }
                else
                {
                    _repeat = 1;
                }
            }
            
            _repeat--;
            return _last;
        }
        
        void init(BitInput bi) throws IOException
        {
            _curTbl = 0;
            _grpIdx = -1;
            _grpPos = -1;
            _bwtBlock = new byte[900000];
            _symbolMap = new byte[256];
            _bwtByteCounts = new int[256];
            _last = -1;
            int crc = bi.readInt();
            bi.readBool();
            _acc = 0;
            _repeat = 0;
            _length = 0;
            _curp = 0;
            _bwtBytesDecoded = 0;
            int bwtStartPointer = bi.readBits(24), symbolCount = 0;
            int ranges = bi.readBits(16);
    
            for (int i = 0; i < 16; i++)
                if ((ranges & 1 << 15 >>> i) != 0)
                    for (int j = 0, k = i << 4; j < 16; j++, k++)
                        if (bi.readBool())
                            _symbolMap[symbolCount++] = (byte)k;
            
            int eob = symbolCount + 1, tables = bi.readBits(3), selectors_n = bi.readBits(15);
            byte[] selectors = new byte[selectors_n];
            MoveToFront tableMTF2 = new MoveToFront();
            
            for (int i = 0; i < selectors_n; i++)
                selectors[i] = tableMTF2.indexToFront(bi.readUnary());

            Tables _tables = new Tables();

            for (int t = 0; t < tables; t++)
            {
                Table table = new Table(symbolCount);

                for (int i = 0, c = bi.readBits(5); i <= eob; i++)
                {
				    while (bi.readBool()) c += bi.readBool() ? -1 : 1;
                    table.add((byte)c);
			    }
                
                table.calc();
                _tables.add(table);
		    }
            
            _curTbl = selectors[0];
            MoveToFront symbolMTF2 = new MoveToFront();
            _length = 0;
            
            for (int n = 0, inc = 1, mtfValue = 0;;)
            {
                int nextSymbol = _nextSymbol(bi, _tables, selectors);

                if (nextSymbol == 0)
                {
                    n += inc;
                    inc <<= 1;
                }
                else if (nextSymbol == 1)
                {
                    n += inc << 1;
                    inc <<= 1;
                }
                else
                {
                    if (n > 0)
                    {
                        byte nextByte = _symbolMap[mtfValue];
                        _bwtByteCounts[nextByte & 0xff] += n;
                        while (--n >= 0) _bwtBlock[_length++] = nextByte;
                        n = 0;
                        inc = 1;
                    }
                    
                    if (nextSymbol == eob)
                        break;
                
                    mtfValue = symbolMTF2.indexToFront(nextSymbol - 1) & 0xff;
                    byte next = _symbolMap[mtfValue];
                    _bwtByteCounts[next & 0xff]++;
                    _bwtBlock[_length++] = next;
			    }
            }
            
            _merged = new int[_length];
            int[] characterBase = new int[256];
            
            for (int i = 0; i < 255; i++)
                characterBase[i + 1] = _bwtByteCounts[i];
            
            for (int i = 2; i <= 255; i++)
                characterBase[i] += characterBase[i - 1];
            
            for (int i = 0; i < _length; i++)
            {
                int value = _bwtBlock[i] & 0xff;
                _merged[characterBase[value]] = (i << 8) + value;
                characterBase[value]++;
            }
            
            _bwtBlock = null;
            _curp = _merged[bwtStartPointer];
        }
    }

    class DecStream
    {
        private BitInput _bi;
        private boolean _streamComplete = false;
        private Block _bd;
        private Block2 _block;
        
        void extractTo(java.io.OutputStream o) throws IOException
        {
            for (int b = _read(); b != -1; b = _read())
                o.write(b);

            o.flush();
        }

        private int _read() throws IOException
        {
            int next = _bd.read();
            return next = next == -1 && _initNextBlock() ? _bd.read() : next;
        }
        
        private boolean _initNextBlock() throws IOException
        {
            if (_streamComplete)
                return false;

            final int marker1 = _bi.readBits(24), marker2 = _bi.readBits(24);

            if (marker1 == 0x314159 && marker2 == 0x265359)
            {
                _bd.init(_bi);
                return true;
            }
            else if (marker1 == 0x177245 && marker2 == 0x385090)
            {
                _streamComplete = true;
                int crc = _bi.readInt();
                return false;
            }
            
            _streamComplete = true;
            throw new IOException("BZip2 stream format error");
        }
        
        DecStream(BitInput bi) throws IOException
        {
            _bi = bi;
            _bi.ignore(32);
            _bd = new Block();
        }
    }

    public void run(String[] args) throws IOException
    {
        if (args.length == 0)
            throw new Error("Usage:\n  java demo.Decompress <filename>\n");
        
        java.io.File inputFile = new java.io.File(args[0]);
        System.err.println(args[0]);
        
        if (!inputFile.exists() || !inputFile.canRead() || !args[0].endsWith(".bz2"))
            throw new Error("Cannot read file " + inputFile.getPath());
        
        java.io.FileInputStream fis = new java.io.FileInputStream(inputFile);
        BitInput bis = new BitInput(fis);
        DecStream is = new DecStream(bis);
        is.extractTo(System.out);
        fis.close();
    }
    
	public static void main(String[] args) throws IOException { new Bzcat2().run(args); }
}


