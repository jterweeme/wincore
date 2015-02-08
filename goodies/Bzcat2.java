import java.io.IOException;


/* Bzcat zonder de random */
class Bzcat2
{
    class BitInput
    {
        final java.io.InputStream _is;
        int _bitBuffer, _bitCount;
        public int readInt() throws IOException { return readBits(16) << 16 | readBits(16); }
        public BitInput(final java.io.InputStream is) { _is = is; }
        public boolean readBool() throws IOException { return readBits(1) == 0 ? false : true; }
        public int readUnary() throws IOException { int u = 0; while (readBool()) u++; return u; }
        public void ignore(int n) throws IOException { while (n-- > 0) readBool(); }
        
        public int readBits(int n) throws IOException
        {
            for (; _bitCount < n; _bitCount += 8) _bitBuffer = _bitBuffer << 8 | _is.read();
            _bitCount -= n;
            return _bitBuffer >>> _bitCount & (1 << n) - 1;
        }
    }

    class Table
    {
        private byte[] _buf = new byte[256];
        Table() { for (int i = 0; i < 256; i++) _buf[i] = (byte)i; }
        void dump(java.io.PrintStream os) { for (int i = 0; i < 256; i++) os.format("%d ", i); }

        byte indexToFront(int index)
        {
            byte value = _buf[index];
            for (int i = index; i > 0; i--) _buf[i] = _buf[i - 1];
            return _buf[0] = value;
        }
    }

    class Block
    {
        int[] _minLengths = new int[6], _bwtByteCounts, _merged;
        int[][] _bases = new int[6][25], _limits = new int[6][24], _symbols = new int[6][258];
        int _curTbl, _grpIdx, _grpPos, _last, _acc, _rleRepeat, _randomIndex, _randomCount;
        boolean _blockRandomised;
        byte[] _symbolMap, _bwtBlock;
        int _curp, _length = 0, _bwtBytesDecoded = 0;

        int _nextSymbol(BitInput bi, byte[] selectors) throws IOException
        {
            if (++_grpPos % 50 == 0)
                _curTbl = selectors[++_grpIdx];

            for (int n = _minLengths[_curTbl], codeBits = bi.readBits(n); n <= 23; n++)
            {
                if (codeBits <= _limits[_curTbl][n])
                    return _symbols[_curTbl][codeBits - _bases[_curTbl][n]];
                
                codeBits = codeBits << 1 | bi.readBits(1);
            }

            throw new IOException("Error decoding BZip2 block");
        }
       
        private int _maxLength(byte[] a, int n)
        {
            byte maxLength = 0;

            for (int i = 0; i < n; i++)
                maxLength = (byte)Math.max(a[i], maxLength);

            return maxLength;
        }

        private int _minLength(byte[] a, int n)
        {
            byte minLength = 0;

            for (int i = 0; i < n; i++)
                minLength = (byte)Math.min(a[i], minLength);

            return minLength;
        }

        int _nextByte()
        {
            int next = _curp & 0xff;
            _curp = _merged[_curp >>> 8];
            _bwtBytesDecoded++;
            return next;
	    }
        
	    public int read()
        {
            while (_rleRepeat < 1)
            {
                if (_bwtBytesDecoded == _length)
                    return -1;
                
                int nextByte = _nextByte();
                
                if (nextByte != _last)
                {
                    _last = nextByte;
                    _rleRepeat = 1;
                    _acc = 1;
                }
                else if (++_acc == 4)
                {
                    _rleRepeat = _nextByte() + 1;
                    _acc = 0;
                }
                else
                {
                    _rleRepeat = 1;
                }
            }
            
            _rleRepeat--;
            return _last;
        }
        
        public void init(BitInput bi) throws IOException
        {
            _minLengths = new int[6];
            _bases = new int[6][25];
            _limits = new int[6][24];
            _symbols = new int[6][258];
            _curTbl = 0;
            _grpIdx = -1;
            _grpPos = -1;
            _bwtBlock = new byte[900000];
            _symbolMap = new byte[256];
            _bwtByteCounts = new int[256];
            _last = -1;
            _randomIndex = 0;
            int crc = bi.readInt();
            //System.err.format("CRC: %x\n", crc);
            _blockRandomised = bi.readBool();
            _acc = 0;
            _rleRepeat = 0;
            _length = 0;
            _curp = 0;
            _bwtBytesDecoded = 0;
            int bwtStartPointer = bi.readBits(24), symbolCount = 0;
            byte[][] tableCodeLengths = new byte[6][258];
            //System.err.format("Starting Pointer into BWT: %d\n", bwtStartPointer);
        
            int ranges = bi.readBits(16);
            //System.err.format("Huffman Used Map: %d\n", ranges);
    
            for (int i = 0; i < 16; i++)
                if ((ranges & 1 << 15 >>> i) != 0)
                    for (int j = 0, k = i << 4; j < 16; j++, k++)
                        if (bi.readBool())
                            _symbolMap[symbolCount++] = (byte)k;
            
            //System.err.format("SymbolCount: %d\n", symbolCount);
            int eob = symbolCount + 1, tables = bi.readBits(3), selectors_n = bi.readBits(15);
            //System.err.format("Huffman Groups: %d\n", tables);
            //System.err.format("Selectors: %d\n", selectors_n);
            byte[] selectors = new byte[selectors_n];
            Table tableMTF2 = new Table();
            
            for (int i = 0; i < selectors_n; i++)
                selectors[i] = tableMTF2.indexToFront(bi.readUnary());

            for (int t = 0; t < tables; t++)
            {
                for (int i = 0, c = bi.readBits(5); i <= eob; i++)
                {
				    while (bi.readBool()) c += bi.readBool() ? -1 : 1;
                    tableCodeLengths[t][i] = (byte)c;
			    }
		    }
            
	        for (int table = 0; table < 6; table++)
            {
                int minLength = _minLength(tableCodeLengths[table], symbolCount + 2);
                int maxLength = _maxLength(tableCodeLengths[table], symbolCount + 2);
                _minLengths[table] = minLength;
                
                for (int i = 0; i < symbolCount + 2; i++)
                    _bases[table][tableCodeLengths[table][i] + 1]++;
                
                for (int i = 1; i < 25; i++)
                    _bases[table][i] += _bases[table][i - 1];
                
                for (int i = minLength, code = 0; i <= maxLength; i++)
                {
                    int base = code;
                    code += _bases[table][i + 1] - _bases[table][i];
                    _bases[table][i] = base - _bases[table][i];
                    _limits[table][i] = code - 1;
                    code <<= 1;
                }
                
                for (int n = minLength, i = 0; n <= maxLength; n++)
                    for (int symbol = 0; symbol < symbolCount + 2; symbol++)
                        if (tableCodeLengths[table][symbol] == n)
                            _symbols[table][i++] = symbol;
            }
            
            _curTbl = selectors[0];
            Table symbolMTF2 = new Table();
            _length = 0;
            
            for (int n = 0, inc = 1, mtfValue = 0;;)
            {
                int nextSymbol = _nextSymbol(bi, selectors);

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
            
            //System.err.println(_length);
            //_merged = new int[_length];
            _merged = new int[999999];
            int[] characterBase = new int[256];
            
            for (int i = 0; i < 255; i++)
                characterBase[i + 1] = _bwtByteCounts[i];
            
            for (int i = 2; i <= 255; i++)
                characterBase[i] += characterBase[i - 1];
            
            for (int i = 0; i < _length; i++)
            {
                int value = _bwtBlock[i] & 0xff;
                _merged[characterBase[value]++] = (i << 8) + value;
            }
            
            _bwtBlock = null;
            _curp = _merged[bwtStartPointer];
        }
    }

    class DecStream
    {
        BitInput _bi;
        boolean _streamComplete = false;
        Block _bd;
        
        public void extractTo(java.io.OutputStream o) throws IOException
        {
            for (int b = _read(); b != -1; b = _read())
                o.write(b);

            o.flush();
        }

        int _read() throws IOException
        {
            int next = _bd.read();
            return next = next == -1 && _initNextBlock() ? _bd.read() : next;
        }
        
        boolean _initNextBlock() throws IOException
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
                //System.err.format("CRC: %x\n", crc);
                return false;
            }
            
            _streamComplete = true;
            throw new IOException("BZip2 stream format error");
        }
        
        public DecStream(BitInput bi) throws IOException
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


