import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

class Bunzip2
{
    class BitInput
    {
        final InputStream _inputStream;
        int _bitBuffer, _bitCount;
        public int readInt() throws IOException { return readBits(16) << 16 | readBits(16); }
        public BitInput(final InputStream inputStream) { _inputStream = inputStream; }
        public boolean readBool() throws IOException { return readBits(1) == 0 ? false : true; }
        public int readUnary() throws IOException { int u = 0; while (readBool()) u++; return u; }
        
        public int readBits(final int count) throws IOException
        {
            for (; _bitCount < count; _bitCount += 8)
                _bitBuffer = _bitBuffer << 8 | _inputStream.read();
            
            _bitCount -= count;
            return _bitBuffer >>> _bitCount & (1 << count) - 1;
        }
    }

    class Huffman
    {
        BitInput _bi;
        byte[] _selectors;
        int[] _minLengths = new int[6];
        int[][] _codeBases=new int[6][25],_codeLimits=new int[6][24],_codeSymbols=new int[6][258];
        int _curTbl, _groupIdx = -1, _groupPos = -1;
        
        public int nextSymbol() throws IOException
        {
            System.err.format("GroupPos: %d\n", _groupPos);
            System.err.format("GroupIdx: %d\n", _groupIdx);

            if (++_groupPos % 50 == 0)
            {
                if (++_groupIdx == _selectors.length)
                    throw new IOException("Error decoding BZip2 block");
                
                System.err.format("GroupIdx: %d\n", _groupIdx);
                _curTbl = _selectors[_groupIdx] & 0xff;
                System.err.format("CurTbl: %d\n", _curTbl);
            }
            
            for (int n = _minLengths[_curTbl], codeBits = _bi.readBits(n); n <= 23; n++)
            {
                System.err.format("CodeBits: %d %d\n", n, codeBits);

                if (codeBits <= _codeLimits[_curTbl][n])
                    return _codeSymbols[_curTbl][codeBits - _codeBases[_curTbl][n]];
                
                codeBits = codeBits << 1 | _bi.readBits(1);
            }

            throw new IOException("Error decoding BZip2 block");
        }

        public Huffman(BitInput bi, int nalphabet, byte[][] tblCodeLengths, byte[] selectors)
        {
            _bi = bi;
            _selectors = selectors;
            _curTbl = _selectors[0];

            System.err.format("N_alphabet: %d\n", nalphabet);

	        for (int table = 0, minLength = 23, maxLength = 0; table < 6; table++)
            {
                for (int i = 0; i < nalphabet; i++)
                {
                    maxLength = Math.max(tblCodeLengths[table][i], maxLength);
                    minLength = Math.min(tblCodeLengths[table][i], minLength);
                }
                
                System.err.format("MinLength: %d\nMaxLength: %d\n", minLength, maxLength);
                _minLengths[table] = minLength;
                
                for (int i = 0; i < nalphabet; i++)
                    _codeBases[table][tblCodeLengths[table][i] + 1]++;
                
                for (int i = 1; i < 25; i++)
                    _codeBases[table][i] += _codeBases[table][i - 1];
                
                for (int i = minLength, code = 0; i <= maxLength; i++)
                {
                    int base = code;
                    code += _codeBases[table][i + 1] - _codeBases[table][i];
                    _codeBases[table][i] = base - _codeBases[table][i];
                    _codeLimits[table][i] = code - 1;
                    code <<= 1;
                }
                
                for (int bitLength = minLength, i = 0; bitLength <= maxLength; bitLength++)
                    for (int symbol = 0; symbol < nalphabet; symbol++)
                        if (tblCodeLengths[table][symbol] == bitLength)
                        {
                            _codeSymbols[table][i++] = symbol;
                            System.err.format("Symbol: %d\n", symbol);
                        }
            }
        }
    }

    class BlockDec
    {
        final int[] RNUMS = {
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
        
        final BitInput _bi;
        boolean _blockRandomised;
        final byte[] _huffmanSymbolMap = new byte[256];
        final int[] _bwtByteCounts = new int[256];
        byte[] _bwtBlock;
        int[] _bwtMergedPointers;
        int _bwtCurrentMergedPointer, _bwtBlockLength, _bwtBytesDecoded;
        int _rleLast = -1, _rleAccumulator, _rleRepeat, _randomIndex = 0;
        int _randomCount = RNUMS[0] - 1;

        byte[] generate()
        {
            byte a[] = new byte[256];
            for (int i = 0; i < a.length; i++) a[i] = (byte)i;
            return a;
        }

        byte indexToFront(byte[] a, int index)
        {
            byte value = a[index];
            System.arraycopy(a, 0, a, 1, index);
            return a[0] = value;
        }
        
        int decodeNextBWTByte()
        {
            int mergedPointer = _bwtCurrentMergedPointer, nextDecodedByte = mergedPointer & 0xff;
            _bwtCurrentMergedPointer = _bwtMergedPointers[mergedPointer >>> 8];
            
            if (_blockRandomised && --_randomCount == 0)
            {
                nextDecodedByte ^= 1;
                _randomIndex = (_randomIndex + 1) % 512;
                _randomCount = RNUMS[_randomIndex];
            }
            
            _bwtBytesDecoded++;
            return nextDecodedByte;
	    }
        
        public int read()
        {
            while (_rleRepeat < 1)
            {
                if (_bwtBytesDecoded == _bwtBlockLength)
                    return -1;
                
                int nextByte = decodeNextBWTByte();
                
                if (nextByte != _rleLast)
                {
                    _rleLast = nextByte;
                    _rleRepeat = 1;
                    _rleAccumulator = 1;
                }
                else if (++_rleAccumulator == 4)
                {
                    _rleRepeat = decodeNextBWTByte() + 1;
                    _rleAccumulator = 0;
                }
                else
                {
                    _rleRepeat = 1;
                }
            }
            
            _rleRepeat--;
            return _rleLast;
        }
        
        public BlockDec(BitInput bi) throws IOException
        {
            _bi = bi;
            _bwtBlock = new byte[900000];
        }

        public void reset() throws IOException
        {
            _bi.readInt();
            _blockRandomised = _bi.readBool();
            int bwtStartPointer = _bi.readBits(24), symbolCount = 0;
            byte[][] tableCodeLengths = new byte[6][258];
            
            for (int i = 0, ranges = _bi.readBits(16); i < 16; i++)
                if ((ranges & 1 << 15 >>> i) != 0)
                    for (int j = 0, k = i << 4; j < 16; j++, k++)
                        if (_bi.readBool())
                            _huffmanSymbolMap[symbolCount++] = (byte)k;

            System.err.format("SymbolCount: %d\n", symbolCount);
            int eob = symbolCount + 1, tables = _bi.readBits(3), selectors_n = _bi.readBits(15);
            System.err.format("Tables: %d\nSelectors_n: %d\n", tables, selectors_n);
            byte[] tableMTF = generate(), selectors = new byte[selectors_n];
            
            for (int i = 0; i < selectors_n; i++)
                selectors[i] = indexToFront(tableMTF, _bi.readUnary());

            for (int t = 0; t < tables; t++)
            {
                for (int i = 0, c = _bi.readBits(5); i <= eob; i++)
                {
                    System.err.format("C: %d\n", c);

                    while (_bi.readBool())
                        c += _bi.readBool() ? -1 : 1;
                    
                    tableCodeLengths[t][i] = (byte)c;
			    }
		    }
            
            Huffman h = new Huffman(_bi, symbolCount + 2, tableCodeLengths, selectors);
            byte[] symbolMTF = generate();
            _bwtBlockLength = 0;
            
            for (int n = 0, repeatIncrement = 1, mtfValue = 0;;)
            {
                int nextSymbol = h.nextSymbol();
                System.err.format("NextSymbol: %d\n", nextSymbol);
                
                if (nextSymbol == 0)
                {
                    n += repeatIncrement;
                    repeatIncrement <<= 1;
                }
                else if (nextSymbol == 1)
                {
                    n += repeatIncrement << 1;
                    repeatIncrement <<= 1;
                }
                else
                {
                    if (n > 0)
                    {
                        final byte nextByte = _huffmanSymbolMap[mtfValue];
                        _bwtByteCounts[nextByte & 0xff] += n;
                        
                        while (--n >= 0)
                            _bwtBlock[_bwtBlockLength++] = nextByte;
                    
                        n = 0;
                        repeatIncrement = 1;
                    }
                    
                    if (nextSymbol == eob)
                        break;
                
                    mtfValue = indexToFront(symbolMTF, nextSymbol - 1) & 0xff;
                    byte nextByte = _huffmanSymbolMap[mtfValue];
                    _bwtByteCounts[nextByte & 0xff]++;
                    _bwtBlock[_bwtBlockLength++] = nextByte;
			    }
            }
            
            _bwtMergedPointers = new int[_bwtBlockLength];
            int[] characterBase = new int[256];
            System.arraycopy(_bwtByteCounts, 0, characterBase, 1, 255);
            
            for (int i = 2; i <= 255; i++)
                characterBase[i] += characterBase[i - 1];
            
            for (int i = 0; i < _bwtBlockLength; i++)
            {
                int value = _bwtBlock[i] & 0xff;
                _bwtMergedPointers[characterBase[value]++] = (i << 8) + value;
            }
            
            _bwtBlock = null;
            _bwtCurrentMergedPointer = _bwtMergedPointers[bwtStartPointer];
        }
    }

    class DecStream
    {
        BitInput _bi;
        boolean _streamComplete = false;
        BlockDec _blockDecompressor = null;
        
        public void extractTo(OutputStream o) throws IOException
        {
            for (int b = read(); b != -1; b = read())
                o.write(b);
        }

        public int read() throws IOException
        {
            int nextByte = _blockDecompressor.read();
            
            if (nextByte == -1 && _initNextBlock())
                nextByte = _blockDecompressor.read();
            
            return nextByte;
        }
        
        boolean _initNextBlock() throws IOException
        {
            if (_streamComplete)
                return false;

            final int marker1 = _bi.readBits(24), marker2 = _bi.readBits(24);

            if (marker1 == 0x314159 && marker2 == 0x265359)
            {
                try
                {
                    _blockDecompressor.reset();
                }
                catch (IOException e)
                {
                    _streamComplete = true;
                    throw e;
                }
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

            try
            {
                int magic = _bi.readBits(16);
                System.err.format("Magic: %x\n", magic);
                _bi.readBits(16);
            }
            catch (IOException e)
            {
                _streamComplete = true;
                throw e;
            }

            _blockDecompressor = new BlockDec(_bi);
            _initNextBlock();
        }
    }

    public void run(String[] args) throws IOException
    {
        if (args.length == 0)
        {
            System.err.println("Demonstration BZip2 decompressor\n");
            System.err.println("Usage:\n  java demo.Decompress <filename>\n");
            System.exit(1);
		}
        
        File inputFile = new File(args[0]);
        
        if (!inputFile.exists() || !inputFile.canRead() || !args[0].endsWith(".bz2"))
        {
            System.err.println("Cannot read file " + inputFile.getPath());
			System.exit(1);
		}
        
		File outputFile = new File(args[0].substring (0, args[0].length() - 4));
        
		if (outputFile.exists())
        {
			System.err.println("File " + outputFile.getPath() + " already exists");
			System.exit(1);
		}
        
        FileInputStream fis = new FileInputStream(inputFile);
        BitInput bis = new BitInput(fis);
        DecStream is = new DecStream(bis);
        OutputStream f = new FileOutputStream(outputFile);
        is.extractTo(f);
        f.close();
        fis.close();
    }
    
    public static void main(String[] args) throws IOException
    {
        Bunzip2 d = new Bunzip2();
        d.run(args);
    }
}


