import java.io.IOException;
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.OutputStream;

class Bzip2
{

    class BZip2Exception extends IOException
    {
        private static final long serialVersionUID = -8931219115669559570L;

        public BZip2Exception (String reason)
        {
            super (reason);
        }
    }

    class BZip2BitOutputStream
    {
        private final OutputStream outputStream;
        private int bitBuffer;
        private int bitCount;

        public void writeBoolean (final boolean value) throws IOException
        {
            int bitCount = this.bitCount + 1;
            int bitBuffer = this.bitBuffer | ((value ? 1 : 0) << (32 - bitCount));

            if (bitCount == 8)
            {
                this.outputStream.write (bitBuffer >>> 24);
                bitBuffer = 0;
                bitCount = 0;
            }

            this.bitBuffer = bitBuffer;
            this.bitCount = bitCount;

        }

        public void writeUnary (int value) throws IOException
        {
            while (value-- > 0)
            {
                writeBoolean (true);
            }

            writeBoolean (false);
        }

        public void writeBits(final int count, final int value) throws IOException
        {
            int bitCount = this.bitCount;
            int bitBuffer = this.bitBuffer | ((value << (32 - count)) >>> bitCount);
            bitCount += count;

            while (bitCount >= 8)
            {
                this.outputStream.write (bitBuffer >>> 24);
                bitBuffer <<= 8;
                bitCount -= 8;
            }

            this.bitBuffer = bitBuffer;
            this.bitCount = bitCount;

        }

        public void writeInteger (final int value) throws IOException
        {
            writeBits (16, (value >>> 16) & 0xffff);
            writeBits (16, value & 0xffff);
        }


        public void flush() throws IOException
        {
            if (this.bitCount > 0)
            {
                writeBits (8 - this.bitCount, 0);
            }

        }

        public BZip2BitOutputStream (final OutputStream outputStream)
        {
            this.outputStream = outputStream;
        }


    }

    static class BZip2DivSufSort
    {
        private static final int STACK_SIZE = 64;
        private static final int BUCKET_A_SIZE = 256;
        private static final int BUCKET_B_SIZE = 65536;
        private static final int SS_BLOCKSIZE = 1024;
        private static final int INSERTIONSORT_THRESHOLD = 8;

        private static final int log2table[]= {
            -1,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
        };

        private final int[] SA;
        private final byte[] T;
        private final int n;

        private static final void swapElements(final int[] array1, final int index1,
            final int[] array2, final int index2)
        {
            final int temp = array1[index1];
            array1[index1] = array2[index2];
            array2[index2] = temp;
        }

        private int ssCompare (final int p1, final int p2, final int depth)
        {
            final int[] SA = this.SA;
            final byte[] T = this.T;

            final int U1n, U2n; // pointers within T
            int U1, U2;

            for (
                U1 = depth + SA[p1], U2=depth + SA[p2], U1n = SA[p1 + 1] + 2, U2n = SA[p2 + 1] + 2;
                (U1 < U1n) && (U2 < U2n) && (T[U1] == T[U2]);
                ++U1, ++U2
            );

            return U1<U1n ? (U2 < U2n ? (T[U1] & 0xff) - (T[U2] & 0xff) : 1) : (U2 < U2n ? -1 : 0);

        }

        private int ssCompareLast (int PA, int p1, int p2, int depth, int size)
        {
            final int[] SA = this.SA;
            final byte[] T = this.T;
            int U1, U2, U1n, U2n;

            for (
                U1 = depth + SA[p1], U2 = depth + SA[p2], U1n = size, U2n = SA[(p2 + 1)] + 2;
                (U1 < U1n) && (U2 < U2n) && (T[U1] == T[U2]);
                ++U1, ++U2
            );

            if (U1 < U1n) {
                return (U2 < U2n) ? (T[U1] & 0xff) - (T[U2] & 0xff) : 1;
            } else if (U2 == U2n) {
                return 1;
            }

            for (
                U1 = U1 % size, U1n = SA[PA] + 2;
                (U1 < U1n) && (U2 < U2n) && (T[U1] == T[U2]);
                ++U1, ++U2
            );

            return U1 < U1n ?
                  (U2 < U2n ? (T[U1] & 0xff) - (T[U2] & 0xff) : 1)
                : (U2 < U2n ? -1 : 0);

        }

        private void ssInsertionSort (int PA, int first, int last, int depth)
        {
            final int[] SA = this.SA;
            int i, j; // pointer within SA
            int t;
            int r;

            for (i = last - 2; first <= i; --i) {
            for (t = SA[i], j = i + 1; 0 < (r = ssCompare (PA + t, PA + SA[j], depth));) {
                do {
                    SA[j - 1] = SA[j];
                } while ((++j < last) && (SA[j] < 0));
                if (last <= j) {
                    break;
                }
            }
            if (r == 0) {
                SA[j] = ~SA[j];
            }
            SA[j - 1] = t;
            }

        }

        private void ssFixdown (int Td, int PA, int sa, int i, int size)
        {
            final int[] SA = this.SA;
            final byte[] T = this.T;
            int j, k;
            int v;
            int c, d, e;

            for (v = SA[sa + i], c = (T[Td + SA[PA + v]]) & 0xff;
                (j = 2 * i + 1) < size; SA[sa + i] = SA[sa + k], i = k)
            {
                d = T[Td + SA[PA + SA[sa + (k = j++)]]] & 0xff;

                if (d < (e = T[Td + SA[PA + SA[sa + j]]] & 0xff))
                {
                    k = j;
                    d = e;
                }
                if (d <= c) break;
            }
            SA[sa + i] = v;
        }

        private void ssHeapSort (int Td, int PA, int sa, int size) {

        final int[] SA = this.SA;
        final byte[] T = this.T;

        int i, m;
        int t;

        m = size;
        if ((size % 2) == 0) {
            m--;
            if ((T[Td + SA[PA + SA[sa + (m / 2)]]] & 0xff) < (T[Td + SA[PA + SA[sa + m]]] & 0xff)) {
                swapElements (SA, sa + m, SA, sa + (m / 2));
            }
        }

        for (i = m / 2 - 1; 0 <= i; --i) {
            ssFixdown (Td, PA, sa, i, m);
        }

        if ((size % 2) == 0) {
            swapElements (SA, sa, SA, sa + m);
            ssFixdown (Td, PA, sa, 0, m);
        }

        for (i = m - 1; 0 < i; --i) {
            t = SA[sa];
            SA[sa] = SA[sa + i];
            ssFixdown (Td, PA, sa, 0, i);
            SA[sa + i] = t;
        }

        }

        private int ssMedian3 (final int Td, final int PA, int v1, int v2, int v3)
        {
            final int[] SA = this.SA;
            final byte[] T = this.T;
            int T_v1 = T[Td + SA[PA + SA[v1]]] & 0xff;
            int T_v2 = T[Td + SA[PA + SA[v2]]] & 0xff;
            int T_v3 = T[Td + SA[PA + SA[v3]]] & 0xff;

            if (T_v1 > T_v2)
            {
                final int temp = v1;
                v1 = v2;
                v2 = temp;
                final int T_vtemp = T_v1;
                T_v1 = T_v2;
                T_v2 = T_vtemp;
            }

            if (T_v2 > T_v3)
            {
                if (T_v1 > T_v3)
                {
                    return v1;
                }
                return v3;
            }
            return v2;

        }

        private int ssMedian5 (final int Td, final int PA, int v1, int v2, int v3, int v4, int v5)
        {
            final int[] SA = this.SA;
            final byte[] T = this.T;
            int T_v1 = T[Td + SA[PA + SA[v1]]] & 0xff;
            int T_v2 = T[Td + SA[PA + SA[v2]]] & 0xff;
            int T_v3 = T[Td + SA[PA + SA[v3]]] & 0xff;
            int T_v4 = T[Td + SA[PA + SA[v4]]] & 0xff;
            int T_v5 = T[Td + SA[PA + SA[v5]]] & 0xff;
            int temp;
            int T_vtemp;

            if (T_v2 > T_v3) {
            temp = v2;
            v2 = v3;
            v3 = temp;
            T_vtemp = T_v2;
            T_v2 = T_v3;
            T_v3 = T_vtemp;
        }
        if (T_v4 > T_v5) {
            temp = v4;
            v4 = v5;
            v5 = temp;
            T_vtemp = T_v4;
            T_v4 = T_v5;
            T_v5 = T_vtemp;
        }
        if (T_v2 > T_v4) {
            temp = v2;
            v2 = v4;
            v4 = temp;
            T_vtemp = T_v2;
            T_v2 = T_v4;
            T_v4 = T_vtemp;
            temp = v3;
            v3 = v5;
            v5 = temp;
            T_vtemp = T_v3;
            T_v3 = T_v5;
            T_v5 = T_vtemp;
            }
            if (T_v1 > T_v3) {
            temp = v1;
            v1 = v3;
            v3 = temp;
            T_vtemp = T_v1;
            T_v1 = T_v3;
            T_v3 = T_vtemp;
            }
            if (T_v1 > T_v4) {
            temp = v1;
            v1 = v4;
            v4 = temp;
            T_vtemp = T_v1;
            T_v1 = T_v4;
            T_v4 = T_vtemp;
            temp = v3;
            v3 = v5;
            v5 = temp;
            T_vtemp = T_v3;
            T_v3 = T_v5;
            T_v5 = T_vtemp;
            }
            if (T_v3 > T_v4) {
                return v4;
            }
            return v3;
        }

        private int ssPivot (final int Td, final int PA, final int first, final int last)
        {
            int middle;
            int t;

            t = last - first;
            middle = first + t / 2;

            if (t <= 512) {
            if (t <= 32) {
                return ssMedian3 (Td, PA, first, middle, last - 1);
            }
            t >>= 2;
            return ssMedian5 (Td, PA, first, first + t, middle, last - 1 - t, last - 1);
            }
            t >>= 3;

            return ssMedian3(Td, PA,
                ssMedian3 (Td, PA, first, first + t, first + (t << 1)),
                ssMedian3 (Td, PA, middle - t, middle, middle + t),
                ssMedian3 (Td, PA, last - 1 - (t << 1), last - 1 - t, last - 1));

        }

        private int ssLog (final int n)
        {
            return ((n & 0xff00) != 0) ? 8 + log2table[(n >> 8) & 0xff] : log2table[n & 0xff];
        }

        private int ssSubstringPartition(final int PA, final int first, final int last,
                final int depth)
        {

            final int[] SA = this.SA;

            int a, b;
            int t;

            for (a = first - 1, b = last;;) {
            for (; (++a < b) && ((SA[PA + SA[a]] + depth) >= (SA[PA + SA[a] + 1] + 1));) {
                SA[a] = ~SA[a];
            }
            for (; (a < --b) && ((SA[PA + SA[b]] + depth) <  (SA[PA + SA[b] + 1] + 1)););
            if (b <= a) {
                break;
            }
            t = ~SA[b];
            SA[b] = SA[a];
            SA[a] = t;
            }
            if (first < a) {
                SA[first] = ~SA[first];
            }

            return a;

        }


        public BZip2DivSufSort (final byte[] T, final int[] SA, final int n)
        {
            this.T = T;
            this.SA = SA;
            this.n = n;
        }


    }

    class BZip2BlockCompressor
    {
        private final BZip2BitOutputStream bitOutputStream;
        private final CRC32 crc = new CRC32();
        private final byte[] block;
        private int blockLength = 0;
        private final int blockLengthLimit;
        private final boolean[] blockValuesPresent = new boolean[256];
        private final int[] bwtBlock;
        private int rleCurrentValue = -1;
        private int rleLength = 0;

        private void writeSymbolMap() throws IOException
        {
            BZip2BitOutputStream bitOutputStream = this.bitOutputStream;
            final boolean[] blockValuesPresent = this.blockValuesPresent;
            final boolean[] condensedInUse = new boolean[16];

            for (int i = 0; i < 16; i++)
            {
                for (int j = 0, k = i << 4; j < 16; j++, k++)
                {
                    if (blockValuesPresent[k])
                    {
                        condensedInUse[i] = true;
                    }
                }
            }

            for (int i = 0; i < 16; i++)
            {
                bitOutputStream.writeBoolean (condensedInUse[i]);
            }

            for (int i = 0; i < 16; i++) {
            if (condensedInUse[i]) {
                for (int j = 0, k = i * 16; j < 16; j++, k++) {
                    bitOutputStream.writeBoolean (blockValuesPresent[k]);
                }
            }
            }

        }

        private void writeRun (final int value, int runLength)
        {
            final int blockLength = this.blockLength;
            final byte[] block = this.block;
            this.blockValuesPresent[value] = true;
            this.crc.updateCRC (value, runLength);
            final byte byteValue = (byte)value;

            switch (runLength)
            {
            case 1:
                block[blockLength] = byteValue;
                this.blockLength = blockLength + 1;
                break;

            case 2:
                block[blockLength] = byteValue;
                block[blockLength + 1] = byteValue;
                this.blockLength = blockLength + 2;
                break;

            case 3:
                block[blockLength] = byteValue;
                block[blockLength + 1] = byteValue;
                block[blockLength + 2] = byteValue;
                this.blockLength = blockLength + 3;
                break;

            default:
                runLength -= 4;
                this.blockValuesPresent[runLength] = true;
                block[blockLength] = byteValue;
                block[blockLength + 1] = byteValue;
                block[blockLength + 2] = byteValue;
                block[blockLength + 3] = byteValue;
                block[blockLength + 4] = (byte)runLength;
                this.blockLength = blockLength + 5;
                break;
            }

        }

        public boolean write (final int value) {

        if (this.blockLength > this.blockLengthLimit) {
            return false;
        }

        final int rleCurrentValue = this.rleCurrentValue;
        final int rleLength = this.rleLength;

        if (rleLength == 0) {
            this.rleCurrentValue = value;
            this.rleLength = 1;
        } else if (rleCurrentValue != value) {
            writeRun (rleCurrentValue & 0xff, rleLength);
            this.rleCurrentValue = value;
            this.rleLength = 1;
        } else {
            if (rleLength == 254) {
                writeRun (rleCurrentValue & 0xff, 255);
                this.rleLength = 0;
            } else {
                this.rleLength = rleLength + 1;
            }
        }

        return true;

        }

        public int write (final byte[] data, int offset, int length)
        {
            int written = 0;

            while (length-- > 0)
            {
                if (!write (data[offset++]))
                {
                    break;
                }

                written++;
            }

            return written;
        }

        public void close() throws IOException
        {

            if (this.rleLength > 0)
            {
                writeRun (this.rleCurrentValue & 0xff, this.rleLength);
            }

/*
            this.block[this.blockLength] = this.block[0];

            BZip2DivSufSort divSufSort = new BZip2DivSufSort(this.block, this.bwtBlock,
                    this.blockLength);

            int bwtStartPointer = divSufSort.bwt();
            this.bitOutputStream.writeBits (24, BZip2Constants.BLOCK_HEADER_MARKER_1);
            this.bitOutputStream.writeBits (24, BZip2Constants.BLOCK_HEADER_MARKER_2);
            this.bitOutputStream.writeInteger (this.crc.getCRC());
            this.bitOutputStream.writeBoolean (false);
            this.bitOutputStream.writeBits (24, bwtStartPointer);

            writeSymbolMap();

            BZip2MTFAndRLE2StageEncoder mtfEncoder = new BZip2MTFAndRLE2StageEncoder(this.bwtBlock,
                this.blockLength, this.blockValuesPresent);
            mtfEncoder.encode();

            BZip2HuffmanStageEncoder huffmanEncoder = new BZip2HuffmanStageEncoder(
                this.bitOutputStream, mtfEncoder.getMtfBlock(), mtfEncoder.getMtfLength(),
                mtfEncoder.getMtfAlphabetSize(), mtfEncoder.getMtfSymbolFrequencies());

            huffmanEncoder.encode();
*/
        }

        public boolean isEmpty()
        {
            return ((this.blockLength == 0) && (this.rleLength == 0));
        }

        public int getCRC()
        {
            return this.crc.getCRC();
        }


        public BZip2BlockCompressor(final BZip2BitOutputStream bitOutputStream, final int blockSize)
        {
            this.bitOutputStream = bitOutputStream;
            this.block = new byte[blockSize + 1];
            this.bwtBlock = new int[blockSize + 1];
            this.blockLengthLimit = blockSize - 6;
        }


    }

    class Bzip2OutputStream extends OutputStream
    {
        private OutputStream outputStream;
        private BZip2BitOutputStream bitOutputStream;
        private boolean streamFinished = false;
        //private final int streamBlockSize;
        private int streamCRC = 0;
        //private BZip2BlockCompressor blockCompressor;

        public void write (final int value) throws IOException
        {

            if (this.outputStream == null)
            {
                throw new BZip2Exception ("Stream closed");
            }

            if (this.streamFinished)
            {
                throw new BZip2Exception ("Write beyond end of stream");
            }

/*
            if (!this.blockCompressor.write (value & 0xff))
            {
                closeBlock();
                initialiseNextBlock();
                this.blockCompressor.write (value & 0xff);
            }
*/
        }
    }


    static class CRC32
    {
        private static final int crc32Lookup[] = {
        0x00000000,0x04c11db7,0x09823b6e,0x0d4326d9,0x130476dc,0x17c56b6b,0x1a864db2,0x1e475005,
        0x2608edb8,0x22c9f00f,0x2f8ad6d6,0x2b4bcb61,0x350c9b64,0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
        0x4c11db70,0x48d0c6c7,0x4593e01e,0x4152fda9,0x5f15adac,0x5bd4b01b, 0x569796c2, 0x52568b75,
        0x6a1936c8,0x6ed82b7f,0x639b0da6,0x675a1011,0x791d4014,0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
        0x9823b6e0,0x9ce2ab57,0x91a18d8e,0x95609039,0x8b27c03c,0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
        0xbe2b5b58,0xbaea46ef,0xb7a96036,0xb3687d81,0xad2f2d84,0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
        0xd4326d90,0xd0f37027,0xddb056fe,0xd9714b49,0xc7361b4c,0xc3f706fb, 0xceb42022, 0xca753d95,
        0xf23a8028,0xf6fb9d9f,0xfbb8bb46,0xff79a6f1,0xe13ef6f4,0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
        0x34867077,0x30476dc0,0x3d044b19,0x39c556ae,0x278206ab,0x23431b1c, 0x2e003dc5, 0x2ac12072,
        0x128e9dcf,0x164f8078,0x1b0ca6a1,0x1fcdbb16,0x018aeb13,0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
        0x7897ab07,0x7c56b6b0,0x71159069,0x75d48dde,0x6b93dddb,0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
        0x5e9f46bf,0x5a5e5b08,0x571d7dd1,0x53dc6066,0x4d9b3063,0x495a2dd4, 0x44190b0d, 0x40d816ba,
        0xaca5c697,0xa864db20,0xa527fdf9,0xa1e6e04e,0xbfa1b04b,0xbb60adfc, 0xb6238b25, 0xb2e29692,
        0x8aad2b2f,0x8e6c3698,0x832f1041,0x87ee0df6,0x99a95df3,0x9d684044, 0x902b669d, 0x94ea7b2a,
        0xe0b41de7,0xe4750050,0xe9362689,0xedf73b3e,0xf3b06b3b,0xf771768c, 0xfa325055, 0xfef34de2,
        0xc6bcf05f,0xc27dede8,0xcf3ecb31,0xcbffd686,0xd5b88683,0xd1799b34, 0xdc3abded, 0xd8fba05a,
        0x690ce0ee,0x6dcdfd59,0x608edb80,0x644fc637,0x7a089632,0x7ec98b85, 0x738aad5c, 0x774bb0eb,
        0x4f040d56,0x4bc510e1,0x46863638,0x42472b8f,0x5c007b8a,0x58c1663d, 0x558240e4, 0x51435d53,
        0x251d3b9e,0x21dc2629,0x2c9f00f0,0x285e1d47,0x36194d42,0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
        0x0315d626,0x07d4cb91,0x0a97ed48,0x0e56f0ff,0x1011a0fa,0x14d0bd4d, 0x19939b94, 0x1d528623,
        0xf12f560e,0xf5ee4bb9,0xf8ad6d60,0xfc6c70d7,0xe22b20d2,0xe6ea3d65, 0xeba91bbc, 0xef68060b,
        0xd727bbb6,0xd3e6a601,0xdea580d8,0xda649d6f,0xc423cd6a,0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
        0xbd3e8d7e,0xb9ff90c9,0xb4bcb610,0xb07daba7,0xae3afba2,0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
        0x9b3660c6,0x9ff77d71,0x92b45ba8,0x9675461f,0x8832161a,0x8cf30bad, 0x81b02d74, 0x857130c3,
        0x5d8a9099,0x594b8d2e,0x5408abf7,0x50c9b640,0x4e8ee645,0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
        0x7b827d21,0x7f436096,0x7200464f,0x76c15bf8,0x68860bfd,0x6c47164a, 0x61043093, 0x65c52d24,
        0x119b4be9,0x155a565e,0x18197087,0x1cd86d30,0x029f3d35,0x065e2082, 0x0b1d065b, 0x0fdc1bec,
        0x3793a651,0x3352bbe6,0x3e119d3f,0x3ad08088,0x2497d08d,0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
        0xc5a92679,0xc1683bce,0xcc2b1d17,0xc8ea00a0,0xd6ad50a5,0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
        0xe3a1cbc1,0xe760d676,0xea23f0af,0xeee2ed18,0xf0a5bd1d,0xf464a0aa, 0xf9278673, 0xfde69bc4,
        0x89b8fd09,0x8d79e0be,0x803ac667,0x84fbdbd0,0x9abc8bd5,0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
        0xafb010b1,0xab710d06,0xa6322bdf,0xa2f33668,0xbcb4666d,0xb8757bda, 0xb5365d03, 0xb1f740b4
        };

        private int crc = 0xffffffff;


        public int getCRC()
        {
            return ~this.crc;
        }

        public void updateCRC (final int value)
        {
            final int crc = this.crc;
            this.crc = (crc << 8) ^ crc32Lookup[((crc >> 24) ^ value) & 0xff];
        }

        public void updateCRC (final int value, int count)
        {
            int crc = this.crc;

            while (count-- > 0)
                crc = (crc << 8) ^ crc32Lookup[((crc >> 24) ^ value) & 0xff];
        
            this.crc = crc;
        }


    }

    public void run(String[] args) throws IOException
    {
        File inputFile = new File(args[0]);
        File outputFile = new File(args[1]);
        InputStream fis = new BufferedInputStream(new FileInputStream(inputFile));
        OutputStream fos = new BufferedOutputStream(new FileOutputStream(outputFile));

    }

    public static void main(String[] args) throws IOException
    {
        new Bzip2().run(args);
    }

}

