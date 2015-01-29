import java.io.IOException;

class Bzcat
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

    class Block
    {
        final int[] _rnums = {
			619, 720, 127, 481, 931, 816, 813, 233, 566, 247, 985, 724, 205, 454, 863, 491,
			741, 242, 949, 214, 733, 859, 335, 708, 621, 574, 73, 654, 730, 472, 419, 436,
			278, 496, 867, 210, 399, 680, 480, 51, 878, 465, 811, 169, 869, 675, 611, 697,
			867, 561, 862, 687, 507, 283, 482, 129, 807, 591, 733, 623, 150, 238, 59, 379,
			684, 877, 625, 169, 643, 105, 170, 607, 520, 932, 727, 476, 693, 425, 174, 647,
			73, 122, 335, 530, 442, 853, 695, 249, 445, 515, 909, 545, 703, 919, 874, 474,
			882, 500, 594, 612, 641, 801, 220, 162, 819, 984, 589, 513, 495, 799, 161, 604,
			958, 533, 221, 400, 386, 867, 600, 782, 382, 596, 414, 171, 516, 375, 682, 485,
			911, 276, 98, 553, 163, 354, 666, 933, 424, 341, 533, 870, 227, 730, 475, 186,
			263, 647, 537, 686, 600, 224, 469, 68, 770, 919, 190, 373, 294, 822, 808, 206,
			184, 943, 795, 384, 383, 461, 404, 758, 839, 887, 715, 67, 618, 276, 204, 918,
			873, 777, 604, 560, 951, 160, 578, 722, 79, 804, 96, 409, 713, 940, 652, 934,
			970, 447, 318, 353, 859, 672, 112, 785, 645, 863, 803, 350, 139, 93, 354, 99,
			820, 908, 609, 772, 154, 274, 580, 184, 79, 626, 630, 742, 653, 282, 762, 623,
			680, 81, 927, 626, 789, 125, 411, 521, 938, 300, 821, 78, 343, 175, 128, 250,
			170, 774, 972, 275, 999, 639, 495, 78, 352, 126, 857, 956, 358, 619, 580, 124,
			737, 594, 701, 612, 669, 112, 134, 694, 363, 992, 809, 743, 168, 974, 944, 375,
			748, 52, 600, 747, 642, 182, 862, 81, 344, 805, 988, 739, 511, 655, 814, 334,
			249, 515, 897, 955, 664, 981, 649, 113, 974, 459, 893, 228, 433, 837, 553, 268,
			926, 240, 102, 654, 459, 51, 686, 754, 806, 760, 493, 403, 415, 394, 687, 700,
			946, 670, 656, 610, 738, 392, 760, 799, 887, 653, 978, 321, 576, 617, 626, 502,
			894, 679, 243, 440, 680, 879, 194, 572, 640, 724, 926, 56, 204, 700, 707, 151,
			457, 449, 797, 195, 791, 558, 945, 679, 297, 59, 87, 824, 713, 663, 412, 693,
			342, 606, 134, 108, 571, 364, 631, 212, 174, 643, 304, 329, 343, 97, 430, 751,
			497, 314, 983, 374, 822, 928, 140, 206, 73, 263, 980, 736, 876, 478, 430, 305,
			170, 514, 364, 692, 829, 82, 855, 953, 676, 246, 369, 970, 294, 750, 807, 827,
			150, 790, 288, 923, 804, 378, 215, 828, 592, 281, 565, 555, 710, 82, 896, 831,
			547, 261, 524, 462, 293, 465, 502, 56, 661, 821, 976, 991, 658, 869, 905, 758,
			745, 193, 768, 550, 608, 933, 378, 286, 215, 979, 792, 961, 61, 688, 793, 644,
			986, 403, 106, 366, 905, 644, 372, 567, 466, 434, 645, 210, 389, 550, 919, 135,
			780, 773, 635, 389, 707, 100, 626, 958, 165, 504, 920, 176, 193, 713, 857, 265,
			203, 50, 668, 108, 645, 990, 626, 197, 510, 357, 358, 850, 858, 364, 936, 638
        };
        
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

        byte[] _generate()
        {
            byte a[] = new byte[256];
            for (int i = 0; i < a.length; i++) a[i] = (byte)i;
            return a;
        }

        byte _indexToFront(byte[] a, int index)
        {
            byte value = a[index];
            for (int i = index; i > 0; i--) a[i] = a[i - 1];
            return a[0] = value;
        }
        
        int _nextByte()
        {
            int next = _curp & 0xff;
            _curp = _merged[_curp >>> 8];
            
            if (_blockRandomised && --_randomCount == 0)
            {
                next ^= 1;
                _randomIndex = (_randomIndex + 1) % 512;
                _randomCount = _rnums[_randomIndex];
            }
            
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
            _randomCount = _rnums[0] - 1;
            bi.readInt();
            _blockRandomised = bi.readBool();
            _acc = 0;
            _rleRepeat = 0;
            _length = 0;
            _curp = 0;
            _bwtBytesDecoded = 0;
            int bwtStartPointer = bi.readBits(24), symbolCount = 0;
            byte[][] tableCodeLengths = new byte[6][258];
            
            for (int i = 0, ranges = bi.readBits(16); i < 16; i++)
                if ((ranges & 1 << 15 >>> i) != 0)
                    for (int j = 0, k = i << 4; j < 16; j++, k++)
                        if (bi.readBool())
                            _symbolMap[symbolCount++] = (byte)k;
            
            int eob = symbolCount + 1, tables = bi.readBits(3), selectors_n = bi.readBits(15);
            byte[] tableMTF = _generate(), selectors = new byte[selectors_n];
            
            for (int i = 0; i < selectors_n; i++)
                selectors[i] = _indexToFront(tableMTF, bi.readUnary());

            for (int t = 0; t < tables; t++)
            {
                for (int i = 0, c = bi.readBits(5); i <= eob; i++)
                {
				    while (bi.readBool()) c += bi.readBool() ? -1 : 1;
                    tableCodeLengths[t][i] = (byte)c;
			    }
		    }
            
	        for (int table = 0, minLength = 23, maxLength = 0; table < 6; table++)
            {
                for (int i = 0; i < symbolCount + 2; i++)
                {
                    maxLength = Math.max(tableCodeLengths[table][i], maxLength);
                    minLength = Math.min(tableCodeLengths[table][i], minLength);
                }
                
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
            byte[] symbolMTF = _generate();
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
                
                    mtfValue = _indexToFront(symbolMTF, nextSymbol - 1) & 0xff;
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
                _bi.readInt();
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
    
	public static void main(String[] args) throws IOException { new Bzcat().run(args); }
}


