#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace std;

typedef enum
{
    PJPG_GRAYSCALE,
    PJPG_YH1V1,
    PJPG_YH2V1,
    PJPG_YH1V2,
    PJPG_YH2V2
}
pjpeg_scan_type_t;

struct pjpeg_image_info_t
{
    int m_width;
    int m_height;
    int m_comps;
    int m_MCUSPerRow;
    int m_MCUSPerCol;
    pjpeg_scan_type_t m_scanType;
    int m_MCUWidth;
    int m_MCUHeight;
    unsigned char *m_pMCUBufR;
    unsigned char *m_pMCUBufG;
    unsigned char *m_pMCUBufB;
};

struct HuffTable
{
    uint16_t mMinCode[16];
    uint16_t mMaxCode[16];
    uint8_t mValPtr[16];
};

class App
{
    ifstream ginfile;
    unsigned g_nInFileSize;
    unsigned g_nInFileOfs;
    uint8_t gCallbackStatus;
    pjpeg_scan_type_t gScanType;
    uint8_t gMaxBlocksPerMCU;
    uint8_t gMaxMCUXSize;
    uint8_t gMaxMCUYSize;
    uint16_t gMaxMCUSPerRow;
    uint16_t gMaxMCUSPerCol;
    uint16_t gNumMCUSRemaining;
    uint8_t gMCUOrg[6];
    uint8_t gCompIdent[3];
    uint8_t gCompHSamp[3];
    uint8_t gCompVSamp[3];
    uint8_t gCompQuant[3];
    uint16_t gRestartInterval;
    uint16_t gNextRestartNum;
    uint16_t gRestartsLeft;
    uint8_t gCompsInScan;
    uint8_t gCompList[3];
    uint8_t gCompDCTab[3];
    uint8_t gCompACTab[3];
    uint8_t gHuffVal3[256];
    uint8_t gValidHuffTables;
    uint8_t gValidQuantTables;
    uint8_t gTemFlag;
    static const uint8_t PJPG_NO_MORE_BLOCKS = 1;
    static const uint16_t PJPG_MAX_IN_BUF_SIZE = 256;
    uint8_t gInBuf[PJPG_MAX_IN_BUF_SIZE];
    uint8_t gInBufOfs;
    uint8_t gInBufLeft;
    uint16_t gBitBuf;
    uint8_t gBitsLeft;
    uint16_t gImageXSize;
    uint16_t gImageYSize;
    uint8_t gCompsInFrame;
    int16_t gQuant0[8*8];
    int16_t gQuant1[8*8];
    int16_t gLastDC[3];
    HuffTable gHuffTab0;
    uint8_t gHuffVal0[16];
    HuffTable gHuffTab1;
    uint8_t gHuffVal1[16];
    HuffTable gHuffTab2;
    uint8_t gHuffVal2[256];
    HuffTable gHuffTab3;
    int16_t gCoeffBuf[8*8];
    uint8_t gMCUBufR[256];
    uint8_t gMCUBufG[256];
    uint8_t gMCUBufB[256];
    const uint8_t WinogradBits = 10;
    const uint16_t PJPG_MAX_WIDTH = 16384;
    const uint16_t PJPG_MAX_HEIGHT = 16384;
    const uint8_t PJPG_MAXCOMPSINSCAN = 3;
    const uint8_t DCT_BITS = 7;
    int descale(int x) const { return x + (1U << DCT_BITS - 1) >> DCT_BITS; }
    uint8_t clamp(int16_t s) const;
    int16_t imul_b5(int16_t w) const { return w * 196 + 128 >> 8; }
    int16_t imul_b4(int16_t w) const { return w * 227 + 128 >> 8; }
    int16_t imul_b2(int16_t w) const { return w * 669 + 128 >> 8; }
    uint32_t min(const uint32_t &a, const uint32_t &b) const { return a < b ? a : b; }
    uint8_t subAndClamp(uint8_t a, int16_t b) const { b = a - b; return clamp(b); }
    int16_t imul_b1_b3(int16_t w) const { return w * 362 + 128 >> 8; }
    uint8_t addAndClamp(uint8_t a, int16_t b) const { b = a + b; return clamp(b); }
    void upsampleCb(uint8_t srcOfs, uint8_t dstOfs);
    void createWinogradQuant(int16_t* pQuant) const;
    HuffTable *getHuffTable(uint8_t index);
    void upsampleCbH(uint8_t srcOfs, uint8_t dstOfs);
    void stuffChar(uint8_t i);
    uint8_t *getHuffVal(uint8_t index);
    void write8(ostream &os, int x) const { uint8_t z = (uint8_t)x; os.put(z); }
    uint8_t getChar();
    uint16_t getMaxHuffCodes(uint8_t index) const { return (index < 2) ? 12 : 255; }
    int16_t getExtendOffset(uint8_t i);
    uint16_t getExtendTest(uint8_t i);
    uint8_t getOctet(uint8_t FFCheck);
    uint8_t getBit();
    uint16_t getBits2(uint8_t numBits) { return getBits(numBits, 1); }
    uint16_t getBits(uint8_t numBits, uint8_t FFCheck);
    int16_t huffExtend(uint16_t x, uint8_t s);
    void idctCols();
    void idctRows();
    int write_tga(ostream &os, int x, int y, int comp, void *data) const;
    int write_tga(char const *filename, int x, int y, int comp, void *data) const;
    void huffCreate(const uint8_t* pBits, HuffTable* pHuffTable);
    void upsampleCbV(uint8_t srcOfs, uint8_t dstOfs);
    void upsampleCr(uint8_t srcOfs, uint8_t dstOfs);
    void upsampleCrH(uint8_t srcOfs, uint8_t dstOfs);
    uint16_t getBits1(uint8_t numBits) { return getBits(numBits, 0); }
    uint8_t huffDecode(const HuffTable* pHuffTable, const uint8_t* pHuffVal);
    uint8_t readSOFMarker();
    uint8_t readDHTMarker();
    uint8_t skipVariableMarker();
    uint8_t readDRIMarker();
    uint8_t readSOSMarker();
    uint8_t nextMarker();
    uint8_t processMarkers(uint8_t *pMarker);
    uint8_t readDQTMarker();
    void checkQuantTables() const;
    void upsampleCrV(uint8_t srcOfs, uint8_t dstOfs);
    void copyY(uint8_t dstOfs);
    void convertCb(uint8_t dstOfs);
    void convertCr(uint8_t dstOfs);
    void fixInBuffer();
    uint8_t init();
    uint8_t locateSOIMarker();
    uint8_t locateSOSMarker(uint8_t *pFoundEOI);
    uint8_t locateSOFMarker();
    void checkHuffTables() const;
    void transformBlockReduce(uint8_t mcuBlock);
    void transformBlock(uint8_t mcuBlock);
    uint8_t processRestart();
    uint8_t initFrame();
    uint8_t decodeNextMCU();
    int print_usage(ostream &os) const;
    uint8_t pjpeg_decode_mcu();
    void get_pixel(int* pDst, const uint8_t *pSrc, int luma_only, int num_comps);
    uint8_t initScan();
    void fillInBuf();
    uint8_t pjpeg_decode_init(pjpeg_image_info_t *pInfo);

    uint8_t *pjpeg_load_from_file(const char *pFilename, int *x, int *y, int *comps,
            pjpeg_scan_type_t *pScan_type);

    void write_pixels(ostream &f, int rgb_dir, int vdir, int x, int y,
                int comp, void *data, int write_alpha, int scanline_pad) const;
public:
    int run(int argc, char **argv);
};

void App::write_pixels(ostream &f, int rgb_dir, int vdir, int x, int y,
                int comp, void *data, int write_alpha, int scanline_pad) const
{
    uint8_t bg[3] = { 255, 0, 255}, px[3];
    uint32_t zero = 0;
    int j,k, j_end;

    if (vdir < 0) 
        j_end = -1, j = y-1;
    else
        j_end =  y, j = 0;

    for (; j != j_end; j += vdir)
    {
        for (int i=0; i < x; ++i)
        {
            uint8_t *d = (uint8_t *) data + (j * x + i) * comp;

            if (write_alpha < 0)
                f.write((char *)&d[comp-1], 1);

            switch (comp)
            {
            case 1:
            case 2:
                write8(f, d[0]);
                write8(f, d[0]);
                write8(f, d[0]);
                break;
            case 4:
                if (!write_alpha)
                {
                    for (k=0; k < 3; ++k)
                        px[k] = bg[k] + ((d[k] - bg[k]) * d[3])/255;

                    write8(f, px[1-rgb_dir]);
                    write8(f, px[1]);
                    write8(f, px[1+rgb_dir]);
                    break;
                }
            case 3:
                write8(f, d[1 - rgb_dir]);
                write8(f, d[1]);
                write8(f, d[1 + rgb_dir]);
                break;
            }

            if (write_alpha > 0)
                f.write((char *)&d[comp - 1], 1);
        }

        f.write((char *)&zero, scanline_pad);
    }
}

int App::write_tga(ostream &f, int x, int y, int comp, void *data) const
{
    int has_alpha = !(comp & 1);
    write8(f, 0);
    write8(f, 0);
    write8(f, 2);
    write8(f,0);
    write8(f,0>>8);
    write8(f,0);
    write8(f,0>>8);
    write8(f, 0);
    write8(f,0);
    write8(f,0>>8);
    write8(f,0);
    write8(f,0>>8);
    write8(f,x);
    write8(f,x>>8);
    write8(f,y);
    write8(f,y>>8);
    write8(f, 24 + 8 * has_alpha);
    write8(f, 8 * has_alpha);
    write_pixels(f, -1, -1, x,y,comp,data, has_alpha, 0);
    return 0;
}

int App::write_tga(char const *filename, int x, int y, int comp, void *data) const
{
    ofstream f(filename);
    write_tga(f, x, y, comp, data);
    f.close();
    return 0;
}

int App::print_usage(ostream &os) const
{
    os << "Usage: jpg2tga [source_file] [dest_file] <reduce>\n"
       << "source_file: JPEG file to decode. Note: Progressive files are not supported.\n"
       << "dest_file: Output .TGA file\n"
       << "reduce: Optional, if 1 the JPEG file is quickly decoded to ~1/8th resolution.\n"
       << "\nOutputs 8-bit grayscale or truecolor 24-bit TGA files.\n";
    return EXIT_FAILURE;
}

uint8_t *App::pjpeg_load_from_file(const char *pFilename, int *x, int *y,
                int *comps, pjpeg_scan_type_t *pScan_type)
{
    pjpeg_image_info_t image_info;
    int mcu_x = 0;
    int mcu_y = 0;
    unsigned row_pitch;
    uint8_t *pImage;
    uint8_t status;
    unsigned decoded_width, decoded_height;
    *x = 0;
    *y = 0;
    *comps = 0;

    if (pScan_type)
        *pScan_type = PJPG_GRAYSCALE;

    ginfile.open(pFilename);
    g_nInFileOfs = 0;
    ginfile.seekg(0, ginfile.end);
    g_nInFileSize = ginfile.tellg();
    ginfile.seekg(0, ginfile.beg);
    status = pjpeg_decode_init(&image_info);
   
    if (pScan_type)
        *pScan_type = image_info.m_scanType;

    decoded_width = image_info.m_width;
    decoded_height = image_info.m_height;
    row_pitch = decoded_width * image_info.m_comps;
    pImage = (uint8_t *)malloc(row_pitch * decoded_height);

    while (true)
    {
        uint8_t *pDst_row;
        status = pjpeg_decode_mcu();
      
        if (status)
            break;

        if (mcu_y >= image_info.m_MCUSPerCol)
        {
            free(pImage);
            ginfile.close();
            return NULL;
        }

        pDst_row = pImage + (mcu_y * image_info.m_MCUHeight) *
                row_pitch + (mcu_x * image_info.m_MCUWidth * image_info.m_comps);

        for (int y = 0; y < image_info.m_MCUHeight; y += 8)
        {
            const int by_limit = min(8, image_info.m_height - (mcu_y*image_info.m_MCUHeight+y));

            for (int x = 0; x < image_info.m_MCUWidth; x += 8)
            {
                uint8_t *pDst_block = pDst_row + x * image_info.m_comps;
                unsigned src_ofs = (x * 8U) + (y * 16U);
                const uint8_t *pSrcR = image_info.m_pMCUBufR + src_ofs;
                const uint8_t *pSrcG = image_info.m_pMCUBufG + src_ofs;
                const uint8_t *pSrcB = image_info.m_pMCUBufB + src_ofs;
                const int bx_limit=min(8,image_info.m_width-(mcu_x*image_info.m_MCUWidth+x));

                if (image_info.m_scanType == PJPG_GRAYSCALE)
                {
                    for (int by = 0; by < by_limit; by++)
                    {
                        uint8_t *pDst = pDst_block;

                        for (int bx = 0; bx < bx_limit; bx++)
                            *pDst++ = *pSrcR++;

                        pSrcR += (8 - bx_limit);
                        pDst_block += row_pitch;
                    }
                }
                else
                {
                    for (int by = 0; by < by_limit; by++)
                    {
                        uint8_t *pDst = pDst_block;

                        for (int bx = 0; bx < bx_limit; bx++)
                        {
                            pDst[0] = *pSrcR++;
                            pDst[1] = *pSrcG++;
                            pDst[2] = *pSrcB++;
                            pDst += 3;
                        }

                        pSrcR += (8 - bx_limit);
                        pSrcG += (8 - bx_limit);
                        pSrcB += (8 - bx_limit);
                        pDst_block += row_pitch;
                    }
                }
            }

            pDst_row += (row_pitch * 8);
        }

        mcu_x++;

        if (mcu_x == image_info.m_MCUSPerRow)
        {
            mcu_x = 0;
            mcu_y++;
        }
    }

    ginfile.close();
    *x = decoded_width;
    *y = decoded_height;
    *comps = image_info.m_comps;
    return pImage;
}

void App::get_pixel(int *pDst, const uint8_t *pSrc, int luma_only, int num_comps)
{
    int r, g, b;

    if (num_comps == 1)
    {
        r = g = b = pSrc[0];
    }
    else if (luma_only)
    {
        const int YR = 19595, YG = 38470, YB = 7471;
        r = g = b = (pSrc[0] * YR + pSrc[1] * YG + pSrc[2] * YB + 32768) / 65536;
    }
    else
    {
        r = pSrc[0];
        g = pSrc[1];
        b = pSrc[2];
    }

    pDst[0] = r;
    pDst[1] = g;
    pDst[2] = b;
}

int main(int argc, char **argv)
{
    App app;
    int retval = 0;
    
    try
    {
        app.run(argc, argv);
    }
    catch (...)
    {
        retval = -1;
    }

    return retval;
}

int App::run(int argc, char **argv)
{
    const char *pSrc_filename;
    const char *pDst_filename;
    int width, height, comps;
    pjpeg_scan_type_t scan_type;
    const char* p = "?";
    uint8_t *pImage;
    printf("picojpeg example v1.1, Rich Geldreich, Compiled " __TIME__ " " __DATE__ "\n");

    if ((argc < 3) || (argc > 4))
        return print_usage(cout);
   
    pSrc_filename = argv[1];
    pDst_filename = argv[2];
    printf("Source file:      \"%s\"\n", pSrc_filename);
    printf("Destination file: \"%s\"\n", pDst_filename);
    pImage = pjpeg_load_from_file(pSrc_filename, &width, &height, &comps, &scan_type);

    if (!pImage)
        throw "Failed loading source image!";

    printf("Width: %i, Height: %i, Comps: %i\n", width, height, comps);
   
    switch (scan_type)
    {
    case PJPG_GRAYSCALE:
        p = "GRAYSCALE";
        break;
    case PJPG_YH1V1:
        p = "H1V1";
        break;
    case PJPG_YH2V1:
        p = "H2V1";
        break;
    case PJPG_YH1V2:
        p = "H1V2";
        break;
    case PJPG_YH2V2:
        p = "H2V2";
        break;
    }

    printf("Scan type: %s\n", p);
    write_tga(pDst_filename, width, height, comps, pImage);
    printf("Successfully wrote destination file %s\n", pDst_filename);
    free(pImage);
    return 0;
}

typedef enum
{
    M_SOF0  = 0xC0,
    M_SOF1  = 0xC1,
    M_SOF2  = 0xC2,
    M_SOF3  = 0xC3,
    M_SOF5  = 0xC5,
    M_SOF6  = 0xC6,
    M_SOF7  = 0xC7,
    M_JPG   = 0xC8,
    M_SOF9  = 0xC9,
    M_SOF10 = 0xCA,
    M_SOF11 = 0xCB,
    M_SOF13 = 0xCD,
    M_SOF14 = 0xCE,
    M_SOF15 = 0xCF,
    M_DHT   = 0xC4,
    M_DAC   = 0xCC,
    M_RST0  = 0xD0,
    M_RST1  = 0xD1,
    M_RST2  = 0xD2,
    M_RST3  = 0xD3,
    M_RST4  = 0xD4,
    M_RST5  = 0xD5,
    M_RST6  = 0xD6,
    M_RST7  = 0xD7,
    M_SOI   = 0xD8,
    M_EOI   = 0xD9,
    M_SOS   = 0xDA,
    M_DQT   = 0xDB,
    M_DNL   = 0xDC,
    M_DRI   = 0xDD,
    M_DHP   = 0xDE,
    M_EXP   = 0xDF,
    M_APP0  = 0xE0,
    M_APP15 = 0xEF,
    M_JPG0  = 0xF0,
    M_JPG13 = 0xFD,
    M_COM   = 0xFE,
    M_TEM   = 0x01,
    M_ERROR = 0x100,
    RST0    = 0xD0
}
JPEG_MARKER;

const int8_t ZAG[] = 
{
    0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63,
};

void App::fillInBuf()
{
    gInBufOfs = 4;
    gInBufLeft = 0;
    uint32_t n = min(g_nInFileSize - g_nInFileOfs, PJPG_MAX_IN_BUF_SIZE - gInBufOfs);
    ginfile.read((char *)(gInBuf + gInBufOfs), n);
    gInBufLeft = (uint8_t)n;
    g_nInFileOfs += n;
}

uint8_t App::getChar()
{
    if (!gInBufLeft)
    {
        fillInBuf();

        if (!gInBufLeft)
        {
            gTemFlag = ~gTemFlag;
            return gTemFlag ? 0xFF : 0xD9;
        } 
    }
   
    gInBufLeft--;
    return gInBuf[gInBufOfs++];
}

void App::stuffChar(uint8_t i)
{
    gInBufOfs--;
    gInBuf[gInBufOfs] = i;
    gInBufLeft++;
}

uint8_t App::getOctet(uint8_t FFCheck)
{
    uint8_t c = getChar();
      
    if ((FFCheck) && (c == 0xFF))
    {
        uint8_t n = getChar();

        if (n)
        {
            stuffChar(n);
            stuffChar(0xFF);
        }
    }

    return c;
}

uint16_t App::getBits(uint8_t numBits, uint8_t FFCheck)
{
    uint8_t origBits = numBits;
    uint16_t ret = gBitBuf;
   
    if (numBits > 8)
    {
        numBits -= 8;
        gBitBuf <<= gBitsLeft;
        gBitBuf |= getOctet(FFCheck);
        gBitBuf <<= (8 - gBitsLeft);
        ret = (ret & 0xFF00) | (gBitBuf >> 8);
    }
      
    if (gBitsLeft < numBits)
    {
        gBitBuf <<= gBitsLeft;
        gBitBuf |= getOctet(FFCheck);
        gBitBuf <<= (numBits - gBitsLeft);
        gBitsLeft = 8 - (numBits - gBitsLeft);
    }
    else
    {
        gBitsLeft = (uint8_t)(gBitsLeft - numBits);
        gBitBuf <<= numBits;
    }
   
    return ret >> 16 - origBits;
}

uint8_t App::getBit()
{
    uint8_t ret = gBitBuf & 0x8000 ? 1 : 0;
   
    if (!gBitsLeft)
    {
        gBitBuf |= getOctet(1);
        gBitsLeft += 8;
    }
   
    gBitsLeft--;
    gBitBuf <<= 1;
    return ret;
}

uint16_t App::getExtendTest(uint8_t i)
{
    switch (i)
    {
    case 0: return 0;
    case 1: return 0x0001;
    case 2: return 0x0002;
    case 3: return 0x0004;
    case 4: return 0x0008;
    case 5: return 0x0010; 
    case 6: return 0x0020;
    case 7: return 0x0040;
    case 8:  return 0x0080;
    case 9:  return 0x0100;
    case 10: return 0x0200;
    case 11: return 0x0400;
    case 12: return 0x0800;
    case 13: return 0x1000;
    case 14: return 0x2000; 
    case 15: return 0x4000;
    default: return 0;
    }      
}

int16_t App::getExtendOffset(uint8_t i)
{ 
    switch (i)
    {
      case 0: return 0;
      case 1: return ((-1)<<1) + 1; 
      case 2: return ((-1)<<2) + 1; 
      case 3: return ((-1)<<3) + 1; 
      case 4: return ((-1)<<4) + 1; 
      case 5: return ((-1)<<5) + 1; 
      case 6: return ((-1)<<6) + 1; 
      case 7: return ((-1)<<7) + 1; 
      case 8: return ((-1)<<8) + 1; 
      case 9: return ((-1)<<9) + 1;
      case 10: return ((-1)<<10) + 1; 
      case 11: return ((-1)<<11) + 1; 
      case 12: return ((-1)<<12) + 1; 
      case 13: return ((-1)<<13) + 1; 
      case 14: return ((-1)<<14) + 1; 
      case 15: return ((-1)<<15) + 1;
      default: return 0;
    }
}

int16_t App::huffExtend(uint16_t x, uint8_t s)
{
    return x < getExtendTest(s) ? (int16_t)x + getExtendOffset(s) : x;
}

uint8_t App::huffDecode(const HuffTable* pHuffTable, const uint8_t* pHuffVal)
{
    uint8_t i = 0;
    uint8_t j;
    uint16_t code = getBit();

    while (true)
    {
        uint16_t maxCode;

        if (i == 16)
            return 0;

        maxCode = pHuffTable->mMaxCode[i];

        if ((code <= maxCode) && (maxCode != 0xFFFF))
            break;

        i++;
        code <<= 1;
        code |= getBit();
    }

    j = pHuffTable->mValPtr[i];
    j = (uint8_t)(j + (code - pHuffTable->mMinCode[i]));
    return pHuffVal[j];
}

void App::huffCreate(const uint8_t* pBits, HuffTable* pHuffTable)
{
    uint16_t code = 0;
      
    for (uint8_t i = 0, j = 0; i < 16; i++)
    {
        uint8_t num = pBits[i];
      
        if (!num)
        {
            pHuffTable->mMinCode[i] = 0x0000;
            pHuffTable->mMaxCode[i] = 0xFFFF;
            pHuffTable->mValPtr[i] = 0;
        }
        else
        {
            pHuffTable->mMinCode[i] = code;
            pHuffTable->mMaxCode[i] = code + num - 1;
            pHuffTable->mValPtr[i] = j;
            j = (uint8_t)(j + num);
            code = (uint16_t)(code + num);
        }
      
        code <<= 1;
    }
}

HuffTable *App::getHuffTable(uint8_t index)
{
    switch (index)
    {
        case 0: return &gHuffTab0;
        case 1: return &gHuffTab1;
        case 2: return &gHuffTab2;
        case 3: return &gHuffTab3;
        default: return 0;
    }
}

uint8_t *App::getHuffVal(uint8_t index)
{
    switch (index)
    {
        case 0: return gHuffVal0;
        case 1: return gHuffVal1;
        case 2: return gHuffVal2;
        case 3: return gHuffVal3;
        default: return 0;
    }
}

uint8_t App::readDHTMarker()
{
    uint8_t bits[16];
    uint16_t left = getBits1(16);

    if (left < 2)
        throw "PJPG_BAD_DHT_MARKER";

    left -= 2;

    while (left)
    {
        uint8_t i, tableIndex, index;
        uint8_t *pHuffVal;
        HuffTable *pHuffTable;
        uint16_t count, totalRead;    
        index = (uint8_t)getBits1(8);
      
        if (((index & 0xF) > 1) || ((index & 0xF0) > 0x10) )
            throw "PJPG_BAD_DHT_INDEX";
      
        tableIndex = ((index >> 3) & 2) + (index & 1);
        pHuffTable = getHuffTable(tableIndex);
        pHuffVal = getHuffVal(tableIndex);
        gValidHuffTables |= (1 << tableIndex);
        count = 0;

        for (i = 0; i <= 15; i++)
        {
            uint8_t n = (uint8_t)getBits1(8);
            bits[i] = n;
            count = (uint16_t)(count + n);
        }
      
        if (count > getMaxHuffCodes(tableIndex))
            throw "PJPG_BAD_DHT_COUNTS";

        for (i = 0; i < count; i++)
            pHuffVal[i] = (uint8_t)getBits1(8);

        totalRead = 1 + 16 + count;

        if (left < totalRead)
            throw "PJPG_BAD_DHT_MARKER";

        left = (uint16_t)(left - totalRead);
        huffCreate(bits, pHuffTable);
    }
      
    return 0;
}

const uint8_t gWinogradQuant[] = 
{
   128,  178,  178,  167,  246,  167,  151,  232,
   232,  151,  128,  209,  219,  209,  128,  101,
   178,  197,  197,  178,  101,   69,  139,  167,
   177,  167,  139,   69,   35,   96,  131,  151,
   151,  131,   96,   35,   49,   91,  118,  128,
   118,   91,   49,   46,   81,  101,  101,   81,
   46,   42,   69,   79,   69,   42,   35,   54,
   54,   35,   28,   37,   28,   19,   19,   10,
};   

void App::createWinogradQuant(int16_t *pQuant) const
{
    for (uint8_t i = 0; i < 64; i++) 
    {
        long x = pQuant[i];
        x *= gWinogradQuant[i];
        pQuant[i] = (int16_t)(x + (1 << WinogradBits - DCT_BITS - 1) >> WinogradBits - DCT_BITS);
    }
}

uint8_t App::readDQTMarker()
{
    uint16_t left = getBits1(16);

    if (left < 2)
        throw "PJPG_BAD_DQT_MARKER";

    left -= 2;

    while (left)
    {
        uint8_t n = (uint8_t)getBits1(8);
        uint8_t prec = n >> 4;
        uint16_t totalRead;
        n &= 0x0F;

        if (n > 1)
            throw "PJPG_BAD_DQT_TABLE";

        gValidQuantTables |= (n ? 2 : 1);         

        for (uint8_t i = 0; i < 64; i++)
        {
            uint16_t temp = getBits1(8);
            temp = prec ? (temp << 8) + getBits1(8) : temp;

            if (n)
                gQuant1[i] = (int16_t)temp;
            else
                gQuant0[i] = (int16_t)temp;
        }
      
        createWinogradQuant(n ? gQuant1 : gQuant0);
        totalRead = 64 + 1;

        if (prec)
            totalRead += 64;

        if (left < totalRead)
            throw "PJPG_BAD_DQT_LENGTH";

        left = (uint16_t)(left - totalRead);
    }
   
    return 0;
}

uint8_t App::readSOFMarker()
{
    uint16_t left = getBits1(16);

    if (getBits1(8) != 8)   
        throw "PJPG_BAD_PRECISION";

    gImageYSize = getBits1(16);

    if ((!gImageYSize) || (gImageYSize > PJPG_MAX_HEIGHT))
        throw "PJPG_BAD_HEIGHT";

    gImageXSize = getBits1(16);

    if ((!gImageXSize) || (gImageXSize > PJPG_MAX_WIDTH))
        throw "PJPG_BAD_WIDTH";

    gCompsInFrame = (uint8_t)getBits1(8);

    if (gCompsInFrame > 3)
        throw "PJPG_TOO_MANY_COMPONENTS";

    if (left != (gCompsInFrame + gCompsInFrame + gCompsInFrame + 8))
        throw "PJPG_BAD_SOF_LENGTH";
   
    for (uint8_t i = 0; i < gCompsInFrame; i++)
    {
        gCompIdent[i] = (uint8_t)getBits1(8);
        gCompHSamp[i] = (uint8_t)getBits1(4);
        gCompVSamp[i] = (uint8_t)getBits1(4);
        gCompQuant[i] = (uint8_t)getBits1(8);
      
        if (gCompQuant[i] > 1)
            throw "PJPG_UNSUPPORTED_QUANT_TABLE";
    }
   
    return 0;
}

uint8_t App::skipVariableMarker()
{
    uint16_t left = getBits1(16);

    if (left < 2)
        throw "PJPG_BAD_VARIABLE_MARKER";

    left -= 2;

    while (left)
    {
        getBits1(8);
        left--;
    }
   
    return 0;
}

uint8_t App::readDRIMarker()
{
    if (getBits1(16) != 4)
        throw "PJPG_BAD_DRI_LENGTH";

    gRestartInterval = getBits1(16);
    return 0;
}

uint8_t App::readSOSMarker()
{
    uint16_t left = getBits1(16);
    gCompsInScan = (uint8_t)getBits1(8);
    left -= 3;

    if ((left != (gCompsInScan + gCompsInScan + 3)) || (gCompsInScan < 1) ||
        (gCompsInScan > PJPG_MAXCOMPSINSCAN))
    {
        throw "PJPG_BAD_SOS_LENGTH";
    }
   
    for (uint8_t i = 0; i < gCompsInScan; i++)
    {
        uint8_t cc = (uint8_t)getBits1(8);
        uint8_t c = (uint8_t)getBits1(8);
        uint8_t ci;
        left -= 2;
     
        for (ci = 0; ci < gCompsInFrame; ci++)
            if (cc == gCompIdent[ci])
                break;

        if (ci >= gCompsInFrame)
            throw "PJPG_BAD_SOS_COMP_ID";

        gCompList[i] = ci;
        gCompDCTab[ci] = (c >> 4) & 15;
        gCompACTab[ci] = (c & 15);
    }

    left -= 3;

    while (left)                  
    {
        getBits1(8);
        left--;
    }
   
    return 0;
}

uint8_t App::nextMarker()
{
    uint8_t c;
    uint8_t bytes = 0;

    do
    {
        do
        {
            bytes++;
            c = (uint8_t)getBits1(8);
        }
        while (c != 0xFF);

        do
        {
            c = (uint8_t)getBits1(8);
        }
        while (c == 0xFF);

    }
    while (c == 0);

    return c;
}

uint8_t App::processMarkers(uint8_t *pMarker)
{
    while (true)
    {
        uint8_t c = nextMarker();

        switch (c)
        {
        case M_SOF0:
        case M_SOF1:
        case M_SOF2:
        case M_SOF3:
        case M_SOF5:
        case M_SOF6:
        case M_SOF7:
        case M_SOF9:
        case M_SOF10:
        case M_SOF11:
        case M_SOF13:
        case M_SOF14:
        case M_SOF15:
        case M_SOI:
        case M_EOI:
        case M_SOS:
            *pMarker = c;
            return 0;
        case M_DHT:
            readDHTMarker();
            break;
        case M_DAC:
            throw "PJPG_NO_ARITHMITIC_SUPPORT";
        case M_DQT:
            readDQTMarker();
            break;
        case M_DRI:
            readDRIMarker();
            break;
        case M_JPG:
        case M_RST0:
        case M_RST1:
        case M_RST2:
        case M_RST3:
        case M_RST4:
        case M_RST5:
        case M_RST6:
        case M_RST7:
        case M_TEM:
            throw "PJPG_UNEXPECTED_MARKER";
        default:
            skipVariableMarker();
            break;
        }
    }
}

uint8_t App::locateSOIMarker()
{
    uint16_t bytesleft;
    uint8_t lastchar = (uint8_t)getBits1(8);
    uint8_t thischar = (uint8_t)getBits1(8);

    if ((lastchar == 0xFF) && (thischar == M_SOI))
        return 0;

    bytesleft = 4096;

    while (true)
    {
        if (--bytesleft == 0)
            throw "PJPG_NOT_JPEG";

        lastchar = thischar;
        thischar = (uint8_t)getBits1(8);

        if (lastchar == 0xFF) 
        {
            if (thischar == M_SOI)
                break;
            else if (thischar == M_EOI)
                throw "PJPG_NOT_JPEG";
        }
    }

    thischar = (uint8_t)((gBitBuf >> 8) & 0xFF);

    if (thischar != 0xFF)
        throw "PJPG_NOT_JPEG";
      
    return 0;
}



uint8_t App::locateSOFMarker()
{
    uint8_t c;
    locateSOIMarker();
    processMarkers(&c);

    switch (c)
    {
        case M_SOF2:
            throw "PJPG_UNSUPPORTED_MODE";
        case M_SOF0:
            readSOFMarker();
            break;
        case M_SOF9:  
            throw "PJPG_NO_ARITHMITIC_SUPPORT";
        case M_SOF1:
        default:
            throw "PJPG_UNSUPPORTED_MARKER";
    }
   
    return 0;
}

uint8_t App::locateSOSMarker(uint8_t* pFoundEOI)
{
    uint8_t c;
    *pFoundEOI = 0;  
    processMarkers(&c);

    if (c == M_EOI)
    {
        *pFoundEOI = 1;
        return 0;
    }
    else if (c != M_SOS)
    {
        throw "PJPG_UNEXPECTED_MARKER";
    }

    return readSOSMarker();
}

uint8_t App::init()
{
    gImageXSize = 0;
    gImageYSize = 0;
    gCompsInFrame = 0;
    gRestartInterval = 0;
    gCompsInScan = 0;
    gValidHuffTables = 0;
    gValidQuantTables = 0;
    gTemFlag = 0;
    gInBufOfs = 0;
    gInBufLeft = 0;
    gBitBuf = 0;
    gBitsLeft = 8;
    getBits1(8);
    getBits1(8);
    return 0;
}

void App::fixInBuffer()
{
    if (gBitsLeft > 0)  
        stuffChar((uint8_t)gBitBuf);
   
    stuffChar((uint8_t)(gBitBuf >> 8));
    gBitsLeft = 8;
    getBits2(8);
    getBits2(8);
}

uint8_t App::processRestart()
{
    uint16_t i;
    uint8_t c = 0;

    for (i = 1536; i > 0; i--)
        if (getChar() == 0xFF)
            break;

    if (i == 0)
        throw "PJPG_BAD_RESTART_MARKER";
   
    for ( ; i > 0; i--)
        if ((c = getChar()) != 0xFF)
            break;

    if (i == 0)
        throw "PJPG_BAD_RESTART_MARKER";

    if (c != (gNextRestartNum + M_RST0))
        throw "PJPG_BAD_RESTART_MARKER";

    gLastDC[0] = 0;
    gLastDC[1] = 0;
    gLastDC[2] = 0;
    gRestartsLeft = gRestartInterval;
    gNextRestartNum = (gNextRestartNum + 1) & 7;
    gBitsLeft = 8;
    getBits2(8);
    getBits2(8);
    return 0;
}

void App::checkHuffTables() const
{
    for (uint8_t i = 0; i < gCompsInScan; i++)
    {
        uint8_t compDCTab = gCompDCTab[gCompList[i]];
        uint8_t compACTab = gCompACTab[gCompList[i]] + 2;
      
        if (((gValidHuffTables & (1 << compDCTab)) == 0) ||
           ((gValidHuffTables & (1 << compACTab)) == 0))
        {
            throw "PJPG_UNDEFINED_HUFF_TABLE";
        }
    }
}

void App::checkQuantTables() const
{
    for (uint8_t i = 0; i < gCompsInScan; i++)
    {
        uint8_t compQuantMask = gCompQuant[gCompList[i]] ? 2 : 1;
      
        if ((gValidQuantTables & compQuantMask) == 0)
            throw "PJPG_UNDEFINED_QUANT_TABLE";
    }
}

uint8_t App::initScan()
{
    uint8_t foundEOI;
    uint8_t status = locateSOSMarker(&foundEOI);

    if (status)
        return status;

    if (foundEOI)
        throw "PJPG_UNEXPECTED_MARKER";
   
    checkHuffTables();
    checkQuantTables();
    gLastDC[0] = 0;
    gLastDC[1] = 0;
    gLastDC[2] = 0;

    if (gRestartInterval)
    {
        gRestartsLeft = gRestartInterval;
        gNextRestartNum = 0;
    }

    fixInBuffer();
    return 0;
}

uint8_t App::initFrame()
{
    if (gCompsInFrame == 1)
    {
        if ((gCompHSamp[0] != 1) || (gCompVSamp[0] != 1))
            throw "PJPG_UNSUPPORTED_SAMP_FACTORS";

        gScanType = PJPG_GRAYSCALE;
        gMaxBlocksPerMCU = 1;
        gMCUOrg[0] = 0;
        gMaxMCUXSize     = 8;
        gMaxMCUYSize     = 8;
    }
    else if (gCompsInFrame == 3)
    {
        if (((gCompHSamp[1] != 1) || (gCompVSamp[1] != 1)) ||
            ((gCompHSamp[2] != 1) || (gCompVSamp[2] != 1)))
        {
            throw "PJPG_UNSUPPORTED_SAMP_FACTORS";
        }

        if ((gCompHSamp[0] == 1) && (gCompVSamp[0] == 1))
        {
            gScanType = PJPG_YH1V1;
            gMaxBlocksPerMCU = 3;
            gMCUOrg[0] = 0;
            gMCUOrg[1] = 1;
            gMCUOrg[2] = 2;
            gMaxMCUXSize = 8;
            gMaxMCUYSize = 8;
        }
        else if ((gCompHSamp[0] == 1) && (gCompVSamp[0] == 2))
        {
            gScanType = PJPG_YH1V2;
            gMaxBlocksPerMCU = 4;
            gMCUOrg[0] = 0;
            gMCUOrg[1] = 0;
            gMCUOrg[2] = 1;
            gMCUOrg[3] = 2;
            gMaxMCUXSize = 8;
            gMaxMCUYSize = 16;
        }
        else if ((gCompHSamp[0] == 2) && (gCompVSamp[0] == 1))
        {
            gScanType = PJPG_YH2V1;
            gMaxBlocksPerMCU = 4;
            gMCUOrg[0] = 0;
            gMCUOrg[1] = 0;
            gMCUOrg[2] = 1;
            gMCUOrg[3] = 2;
            gMaxMCUXSize = 16;
            gMaxMCUYSize = 8;
        }
        else if ((gCompHSamp[0] == 2) && (gCompVSamp[0] == 2))
        {
            gScanType = PJPG_YH2V2;
            gMaxBlocksPerMCU = 6;
            gMCUOrg[0] = 0;
            gMCUOrg[1] = 0;
            gMCUOrg[2] = 0;
            gMCUOrg[3] = 0;
            gMCUOrg[4] = 1;
            gMCUOrg[5] = 2;
            gMaxMCUXSize = 16;
            gMaxMCUYSize = 16;
        }
        else
        {
            throw "PJPG_UNSUPPORTED_SAMP_FACTORS";
        }
    }
    else
    {
        throw "PJPG_UNSUPPORTED_COLORSPACE";
    }

    gMaxMCUSPerRow = gImageXSize + gMaxMCUXSize - 1 >> ((gMaxMCUXSize == 8) ? 3 : 4);
    gMaxMCUSPerCol = gImageYSize + gMaxMCUYSize - 1 >> ((gMaxMCUYSize == 8) ? 3 : 4);
    gNumMCUSRemaining = gMaxMCUSPerRow * gMaxMCUSPerCol;
    return 0;
}

uint8_t App::clamp(int16_t s) const
{
    if ((uint16_t)s > 255U)
        return s < 0 ? 0 : s > 255 ? 255 : s;
      
    return (uint8_t)s;
}

void App::idctRows()
{
    int16_t* pSrc = gCoeffBuf;
            
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((pSrc[1] | pSrc[2] | pSrc[3] | pSrc[4] | pSrc[5] | pSrc[6] | pSrc[7]) == 0)
        {
            int16_t src0 = *pSrc;
            *(pSrc+1) = src0;
            *(pSrc+2) = src0;
            *(pSrc+3) = src0;
            *(pSrc+4) = src0;
            *(pSrc+5) = src0;
            *(pSrc+6) = src0;
            *(pSrc+7) = src0;
        }
        else
        {
            int16_t src4 = *(pSrc+5);
            int16_t src7 = *(pSrc+3);
            int16_t x4  = src4 - src7;
            int16_t x7  = src4 + src7;
            int16_t src5 = *(pSrc+1);
            int16_t src6 = *(pSrc+7);
            int16_t x5  = src5 + src6;
            int16_t x6  = src5 - src6;
            int16_t tmp1 = imul_b5(x4 - x6);
            int16_t stg26 = imul_b4(x6) - tmp1;
            int16_t x24 = tmp1 - imul_b2(x4);
            int16_t x15 = x5 - x7;
            int16_t x17 = x5 + x7;
            int16_t tmp2 = stg26 - x17;
            int16_t tmp3 = imul_b1_b3(x15) - tmp2;
            int16_t x44 = tmp3 + x24;
            int16_t src0 = *(pSrc+0);
            int16_t src1 = *(pSrc+4);
            int16_t x30 = src0 + src1;
            int16_t x31 = src0 - src1;
            int16_t src2 = *(pSrc+2);
            int16_t src3 = *(pSrc+6);
            int16_t x12 = src2 - src3;
            int16_t x13 = src2 + src3;
            int16_t x32 = imul_b1_b3(x12) - x13;
            int16_t x40 = x30 + x13;
            int16_t x43 = x30 - x13;
            int16_t x41 = x31 + x32;
            int16_t x42 = x31 - x32;
            *(pSrc+0) = x40 + x17;
            *(pSrc+1) = x41 + tmp2;
            *(pSrc+2) = x42 + tmp3;
            *(pSrc+3) = x43 - x44;
            *(pSrc+4) = x43 + x44;
            *(pSrc+5) = x42 - tmp3;
            *(pSrc+6) = x41 - tmp2;
            *(pSrc+7) = x40 - x17;
        }
                  
        pSrc += 8;
    }      
}

void App::idctCols()
{
    int16_t *pSrc = gCoeffBuf;
   
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((pSrc[1*8]|pSrc[2*8]|pSrc[3*8]|pSrc[4*8]|pSrc[5*8]|pSrc[6*8]|pSrc[7*8]) == 0)
        {
            uint8_t c = clamp(descale(*pSrc) + 128);
            *(pSrc+0*8) = c;
            *(pSrc+1*8) = c;
            *(pSrc+2*8) = c;
            *(pSrc+3*8) = c;
            *(pSrc+4*8) = c;
            *(pSrc+5*8) = c;
            *(pSrc+6*8) = c;
            *(pSrc+7*8) = c;
        }
        else
        {
            int16_t src4 = *(pSrc+5*8);
            int16_t src7 = *(pSrc+3*8);
            int16_t x4  = src4 - src7;
            int16_t x7  = src4 + src7;
            int16_t src5 = *(pSrc+1*8);
            int16_t src6 = *(pSrc+7*8);
            int16_t x5  = src5 + src6;
            int16_t x6  = src5 - src6;
            int16_t tmp1 = imul_b5(x4 - x6);
            int16_t stg26 = imul_b4(x6) - tmp1;
            int16_t x24 = tmp1 - imul_b2(x4);
            int16_t x15 = x5 - x7;
            int16_t x17 = x5 + x7;
            int16_t tmp2 = stg26 - x17;
            int16_t tmp3 = imul_b1_b3(x15) - tmp2;
            int16_t x44 = tmp3 + x24;
            int16_t src0 = *(pSrc+0*8);
            int16_t src1 = *(pSrc+4*8);
            int16_t x30 = src0 + src1;
            int16_t x31 = src0 - src1;
            int16_t src2 = *(pSrc+2*8);
            int16_t src3 = *(pSrc+6*8);
            int16_t x12 = src2 - src3;
            int16_t x13 = src2 + src3;
            int16_t x32 = imul_b1_b3(x12) - x13;
            int16_t x40 = x30 + x13;
            int16_t x43 = x30 - x13;
            int16_t x41 = x31 + x32;
            int16_t x42 = x31 - x32;
            *(pSrc+0*8) = clamp(descale(x40 + x17)  + 128);
            *(pSrc+1*8) = clamp(descale(x41 + tmp2) + 128);
            *(pSrc+2*8) = clamp(descale(x42 + tmp3) + 128);
            *(pSrc+3*8) = clamp(descale(x43 - x44)  + 128);
            *(pSrc+4*8) = clamp(descale(x43 + x44)  + 128);
            *(pSrc+5*8) = clamp(descale(x42 - tmp3) + 128);
            *(pSrc+6*8) = clamp(descale(x41 - tmp2) + 128);
            *(pSrc+7*8) = clamp(descale(x40 - x17)  + 128);
        }

        pSrc++;      
    }      
}

void App::upsampleCb(uint8_t srcOfs, uint8_t dstOfs)
{
    int16_t *pSrc = gCoeffBuf + srcOfs;
    uint8_t *pDstG = gMCUBufG + dstOfs;
    uint8_t *pDstB = gMCUBufB + dstOfs;

    for (uint8_t y = 0; y < 4; y++)
    {
        for (uint8_t x = 0; x < 4; x++)
        {
            uint8_t cb = (uint8_t)*pSrc++;
            int16_t cbG, cbB;
            cbG = ((cb * 88U) >> 8U) - 44U;
            pDstG[0] = subAndClamp(pDstG[0], cbG);
            pDstG[1] = subAndClamp(pDstG[1], cbG);
            pDstG[8] = subAndClamp(pDstG[8], cbG);
            pDstG[9] = subAndClamp(pDstG[9], cbG);
            cbB = (cb + ((cb * 198U) >> 8U)) - 227U;
            pDstB[0] = addAndClamp(pDstB[0], cbB);
            pDstB[1] = addAndClamp(pDstB[1], cbB);
            pDstB[8] = addAndClamp(pDstB[8], cbB);
            pDstB[9] = addAndClamp(pDstB[9], cbB);
            pDstG += 2;
            pDstB += 2;
        }

        pSrc = pSrc - 4 + 8;
        pDstG = pDstG - 8 + 16;
        pDstB = pDstB - 8 + 16;
    }
}   

void App::upsampleCbH(uint8_t srcOfs, uint8_t dstOfs)
{
   int16_t *pSrc = gCoeffBuf + srcOfs;
   uint8_t *pDstG = gMCUBufG + dstOfs;
   uint8_t *pDstB = gMCUBufB + dstOfs;

   for (uint8_t y = 0; y < 8; y++)
   {
      for (uint8_t x = 0; x < 4; x++)
      {
         uint8_t cb = (uint8_t)*pSrc++;
         int16_t cbG, cbB;
         cbG = ((cb * 88U) >> 8U) - 44U;
         pDstG[0] = subAndClamp(pDstG[0], cbG);
         pDstG[1] = subAndClamp(pDstG[1], cbG);
         cbB = (cb + ((cb * 198U) >> 8U)) - 227U;
         pDstB[0] = addAndClamp(pDstB[0], cbB);
         pDstB[1] = addAndClamp(pDstB[1], cbB);
         pDstG += 2;
         pDstB += 2;
      }

      pSrc = pSrc - 4 + 8;
   }
}
   
void App::upsampleCbV(uint8_t srcOfs, uint8_t dstOfs)
{
    int16_t *pSrc = gCoeffBuf + srcOfs;
    uint8_t *pDstG = gMCUBufG + dstOfs;
    uint8_t *pDstB = gMCUBufB + dstOfs;

    for (uint8_t y = 0; y < 4; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            uint8_t cb = (uint8_t)*pSrc++;
            int16_t cbG, cbB;
            cbG = ((cb * 88U) >> 8U) - 44U;
            pDstG[0] = subAndClamp(pDstG[0], cbG);
            pDstG[8] = subAndClamp(pDstG[8], cbG);
            cbB = (cb + ((cb * 198U) >> 8U)) - 227U;
            pDstB[0] = addAndClamp(pDstB[0], cbB);
            pDstB[8] = addAndClamp(pDstB[8], cbB);
            ++pDstG;
            ++pDstB;
        }

        pDstG = pDstG - 8 + 16;
        pDstB = pDstB - 8 + 16;
   }
}   

void App::upsampleCr(uint8_t srcOfs, uint8_t dstOfs)
{
    int16_t *pSrc = gCoeffBuf + srcOfs;
    uint8_t *pDstR = gMCUBufR + dstOfs;
    uint8_t *pDstG = gMCUBufG + dstOfs;

    for (uint8_t y = 0; y < 4; y++)
    {
        for (uint8_t x = 0; x < 4; x++)
        {
            uint8_t cr = (uint8_t)*pSrc++;
            int16_t crR, crG;
            crR = (cr + ((cr * 103U) >> 8U)) - 179;
            pDstR[0] = addAndClamp(pDstR[0], crR);
            pDstR[1] = addAndClamp(pDstR[1], crR);
            pDstR[8] = addAndClamp(pDstR[8], crR);
            pDstR[9] = addAndClamp(pDstR[9], crR);
            crG = ((cr * 183U) >> 8U) - 91;
            pDstG[0] = subAndClamp(pDstG[0], crG);
            pDstG[1] = subAndClamp(pDstG[1], crG);
            pDstG[8] = subAndClamp(pDstG[8], crG);
            pDstG[9] = subAndClamp(pDstG[9], crG);
            pDstR += 2;
            pDstG += 2;
        }

        pSrc = pSrc - 4 + 8;
        pDstR = pDstR - 8 + 16;
        pDstG = pDstG - 8 + 16;
    }
}

void App::upsampleCrH(uint8_t srcOfs, uint8_t dstOfs)
{
    int16_t *pSrc = gCoeffBuf + srcOfs;
    uint8_t *pDstR = gMCUBufR + dstOfs;
    uint8_t *pDstG = gMCUBufG + dstOfs;

    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 4; x++)
        {
            uint8_t cr = (uint8_t)*pSrc++;
            int16_t crR = (cr + ((cr * 103U) >> 8U)) - 179;
            pDstR[0] = addAndClamp(pDstR[0], crR);
            pDstR[1] = addAndClamp(pDstR[1], crR);
            int16_t crG = ((cr * 183U) >> 8U) - 91;
            pDstG[0] = subAndClamp(pDstG[0], crG);
            pDstG[1] = subAndClamp(pDstG[1], crG);
            pDstR += 2;
            pDstG += 2;
        }

        pSrc = pSrc - 4 + 8;
    }
}

void App::upsampleCrV(uint8_t srcOfs, uint8_t dstOfs)
{
    int16_t *pSrc = gCoeffBuf + srcOfs;
    uint8_t *pDstR = gMCUBufR + dstOfs;
    uint8_t *pDstG = gMCUBufG + dstOfs;

    for (uint8_t y = 0; y < 4; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            uint8_t cr = (uint8_t)*pSrc++;
            int16_t crR, crG;
            crR = (cr + ((cr * 103U) >> 8U)) - 179;
            pDstR[0] = addAndClamp(pDstR[0], crR);
            pDstR[8] = addAndClamp(pDstR[8], crR);
            crG = ((cr * 183U) >> 8U) - 91;
            pDstG[0] = subAndClamp(pDstG[0], crG);
            pDstG[8] = subAndClamp(pDstG[8], crG);
            ++pDstR;
            ++pDstG;
        }

        pDstR = pDstR - 8 + 16;
        pDstG = pDstG - 8 + 16;
    }
} 

void App::copyY(uint8_t dstOfs)
{
    uint8_t *pRDst = gMCUBufR + dstOfs;
    uint8_t *pGDst = gMCUBufG + dstOfs;
    uint8_t* pBDst = gMCUBufB + dstOfs;
    int16_t* pSrc = gCoeffBuf;
   
    for (uint8_t i = 64; i > 0; i--)
    {
        uint8_t c = (uint8_t)*pSrc++;
        *pRDst++ = c;
        *pGDst++ = c;
        *pBDst++ = c;
    }
}

void App::convertCb(uint8_t dstOfs)
{
    uint8_t *pDstG = gMCUBufG + dstOfs;
    uint8_t *pDstB = gMCUBufB + dstOfs;
    int16_t *pSrc = gCoeffBuf;

    for (uint8_t i = 64; i > 0; i--)
    {
        uint8_t cb = (uint8_t)*pSrc++;
        int16_t cbG, cbB;
        cbG = ((cb * 88U) >> 8U) - 44U;
        *pDstG = subAndClamp(pDstG[0], cbG);
        pDstG++;
        cbB = (cb + ((cb * 198U) >> 8U)) - 227U;
        *pDstB = addAndClamp(pDstB[0], cbB);
        pDstB++;
    }
}

void App::convertCr(uint8_t dstOfs)
{
    uint8_t *pDstR = gMCUBufR + dstOfs;
    uint8_t *pDstG = gMCUBufG + dstOfs;
    int16_t *pSrc = gCoeffBuf;

    for (uint8_t i = 64; i > 0; i--)
    {
        uint8_t cr = (uint8_t)*pSrc++;
        int16_t crR, crG;
        crR = (cr + ((cr * 103U) >> 8U)) - 179;
        *pDstR = addAndClamp(pDstR[0], crR);
        pDstR++;
        crG = ((cr * 183U) >> 8U) - 91;
        *pDstG = subAndClamp(pDstG[0], crG);
        pDstG++;
    }
}

void App::transformBlock(uint8_t mcuBlock)
{
    idctRows();
    idctCols();
   
    switch (gScanType)
    {
        case PJPG_GRAYSCALE:
            copyY(0);
            break;
        case PJPG_YH1V1:
            switch (mcuBlock)
            {
            case 0:
                copyY(0);
                break;
            case 1:
                convertCb(0);
                break;
            case 2:
                convertCr(0);
                break;
            }

            break;
        case PJPG_YH1V2:
            switch (mcuBlock)
            {
            case 0:
                copyY(0);
                break;
            case 1:
                copyY(128);
                break;
            case 2:
                upsampleCbV(0, 0);
                upsampleCbV(4*8, 128);
                break;
            case 3:
                upsampleCrV(0, 0);
                upsampleCrV(4*8, 128);
                break;
            }
            break;
        case PJPG_YH2V1:
            switch (mcuBlock)
            {
            case 0:
                copyY(0);
                break;
            case 1:
                copyY(64);
                break;
            case 2:
                upsampleCbH(0, 0);
                upsampleCbH(4, 64);
                break;
            case 3:
                upsampleCrH(0, 0);
                upsampleCrH(4, 64);
                break;
            }
            break;
        case PJPG_YH2V2:
            switch (mcuBlock)
            {
            case 0:
                copyY(0);
                break;
            case 1:
                copyY(64);
                break;
            case 2:
                copyY(128);
                break;
            case 3:
                copyY(192);
                break;
            case 4:
                upsampleCb(0, 0);
                upsampleCb(4, 64);
                upsampleCb(4*8, 128);
                upsampleCb(4+4*8, 192);
                break;
            case 5:
                upsampleCr(0, 0);
                upsampleCr(4, 64);
                upsampleCr(4*8, 128);
                upsampleCr(4+4*8, 192);
                break;
            }
            break;
    }
}

void App::transformBlockReduce(uint8_t mcuBlock)
{
    uint8_t c = clamp(descale(gCoeffBuf[0]) + 128);
    int16_t cbG, cbB, crR, crG;

    switch (gScanType)
    {
        case PJPG_GRAYSCALE:
            gMCUBufR[0] = c;
            break;
        case PJPG_YH1V1:
         switch (mcuBlock)
         {
            case 0:
               gMCUBufR[0] = c;
               gMCUBufG[0] = c;
               gMCUBufB[0] = c;
               break;
            case 1:
               cbG = ((c * 88U) >> 8U) - 44U;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], cbG);
               cbB = (c + ((c * 198U) >> 8U)) - 227U;
               gMCUBufB[0] = addAndClamp(gMCUBufB[0], cbB);
               break;
            case 2:
               crR = (c + ((c * 103U) >> 8U)) - 179;
               gMCUBufR[0] = addAndClamp(gMCUBufR[0], crR);
               crG = ((c * 183U) >> 8U) - 91;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], crG);
               break;
         }

         break;
      case PJPG_YH1V2:
         switch (mcuBlock)
         {
            case 0:
               gMCUBufR[0] = c;
               gMCUBufG[0] = c;
               gMCUBufB[0] = c;
               break;
            case 1:
               gMCUBufR[128] = c;
               gMCUBufG[128] = c;
               gMCUBufB[128] = c;
               break;
            case 2:
               cbG = ((c * 88U) >> 8U) - 44U;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], cbG);
               gMCUBufG[128] = subAndClamp(gMCUBufG[128], cbG);
               cbB = (c + ((c * 198U) >> 8U)) - 227U;
               gMCUBufB[0] = addAndClamp(gMCUBufB[0], cbB);
               gMCUBufB[128] = addAndClamp(gMCUBufB[128], cbB);
               break;
            case 3:
               crR = (c + ((c * 103U) >> 8U)) - 179;
               gMCUBufR[0] = addAndClamp(gMCUBufR[0], crR);
               gMCUBufR[128] = addAndClamp(gMCUBufR[128], crR);
               crG = ((c * 183U) >> 8U) - 91;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], crG);
               gMCUBufG[128] = subAndClamp(gMCUBufG[128], crG);
               break;
         }
         break;
      case PJPG_YH2V1:
         switch (mcuBlock)
         {
            case 0:
               gMCUBufR[0] = c;
               gMCUBufG[0] = c;
               gMCUBufB[0] = c;
               break;
            case 1:
               gMCUBufR[64] = c;
               gMCUBufG[64] = c;
               gMCUBufB[64] = c;
               break;
            case 2:
               cbG = ((c * 88U) >> 8U) - 44U;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], cbG);
               gMCUBufG[64] = subAndClamp(gMCUBufG[64], cbG);
               cbB = (c + ((c * 198U) >> 8U)) - 227U;
               gMCUBufB[0] = addAndClamp(gMCUBufB[0], cbB);
               gMCUBufB[64] = addAndClamp(gMCUBufB[64], cbB);
               break;
            case 3:
               crR = (c + ((c * 103U) >> 8U)) - 179;
               gMCUBufR[0] = addAndClamp(gMCUBufR[0], crR);
               gMCUBufR[64] = addAndClamp(gMCUBufR[64], crR);
               crG = ((c * 183U) >> 8U) - 91;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], crG);
               gMCUBufG[64] = subAndClamp(gMCUBufG[64], crG);
               break;
         }
         break;
      case PJPG_YH2V2:
         switch (mcuBlock)
         {
            case 0:
               gMCUBufR[0] = c;
               gMCUBufG[0] = c;
               gMCUBufB[0] = c;
               break;
            case 1:
               gMCUBufR[64] = c;
               gMCUBufG[64] = c;
               gMCUBufB[64] = c;
               break;
            case 2:
               gMCUBufR[128] = c;
               gMCUBufG[128] = c;
               gMCUBufB[128] = c;
               break;
            case 3:
               gMCUBufR[192] = c;
               gMCUBufG[192] = c;
               gMCUBufB[192] = c;
               break;
            case 4:
               cbG = ((c * 88U) >> 8U) - 44U;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], cbG);
               gMCUBufG[64] = subAndClamp(gMCUBufG[64], cbG);
               gMCUBufG[128] = subAndClamp(gMCUBufG[128], cbG);
               gMCUBufG[192] = subAndClamp(gMCUBufG[192], cbG);
               cbB = (c + ((c * 198U) >> 8U)) - 227U;
               gMCUBufB[0] = addAndClamp(gMCUBufB[0], cbB);
               gMCUBufB[64] = addAndClamp(gMCUBufB[64], cbB);
               gMCUBufB[128] = addAndClamp(gMCUBufB[128], cbB);
               gMCUBufB[192] = addAndClamp(gMCUBufB[192], cbB);
               break;
            case 5:
               crR = (c + ((c * 103U) >> 8U)) - 179;
               gMCUBufR[0] = addAndClamp(gMCUBufR[0], crR);
               gMCUBufR[64] = addAndClamp(gMCUBufR[64], crR);
               gMCUBufR[128] = addAndClamp(gMCUBufR[128], crR);
               gMCUBufR[192] = addAndClamp(gMCUBufR[192], crR);
               crG = ((c * 183U) >> 8U) - 91;
               gMCUBufG[0] = subAndClamp(gMCUBufG[0], crG);
               gMCUBufG[64] = subAndClamp(gMCUBufG[64], crG);
               gMCUBufG[128] = subAndClamp(gMCUBufG[128], crG);
               gMCUBufG[192] = subAndClamp(gMCUBufG[192], crG);
               break;
         }
         break;
    }
}

uint8_t App::decodeNextMCU()
{
    uint8_t status;
    uint8_t mcuBlock;   

    if (gRestartInterval) 
    {
        if (gRestartsLeft == 0)
        {
            status = processRestart();

            if (status)
                return status;
        }

        gRestartsLeft--;
    }      
   
    for (mcuBlock = 0; mcuBlock < gMaxBlocksPerMCU; mcuBlock++)
    {
        uint8_t componentID = gMCUOrg[mcuBlock];
        uint8_t compQuant = gCompQuant[componentID];	
        uint8_t compDCTab = gCompDCTab[componentID];
        uint8_t compACTab, k;
        const int16_t *pQ = compQuant ? gQuant1 : gQuant0;
        uint8_t s = huffDecode(compDCTab?&gHuffTab1:&gHuffTab0,compDCTab?gHuffVal1:gHuffVal0);
        uint16_t r = 0;
        uint8_t numExtraBits = s & 0xF;

        if (numExtraBits)
            r = getBits2(numExtraBits);

        uint16_t dc = huffExtend(r, s);
        dc += gLastDC[componentID];
        gLastDC[componentID] = dc;
        gCoeffBuf[0] = dc * pQ[0];
        compACTab = gCompACTab[componentID];

        for (k = 1; k < 64; k++)
        {
            uint16_t extraBits;
            s=huffDecode(compACTab?&gHuffTab3:&gHuffTab2,compACTab ? gHuffVal3 : gHuffVal2);
            extraBits = 0;
            numExtraBits = s & 0xF;

            if (numExtraBits)
                extraBits = getBits2(numExtraBits);

            r = s >> 4;
            s &= 15;

            if (s)
            {
                int16_t ac;

                if (r)
                {
                    if ((k + r) > 63)
                        throw "PJPG_DECODE_ERROR";

                    while (r)
                    {
                        gCoeffBuf[ZAG[k++]] = 0;
                        r--;
                    }
                }

                ac = huffExtend(extraBits, s);
                gCoeffBuf[ZAG[k]] = ac * pQ[k]; 
            }
            else
            {
                if (r == 15)
                {
                    if ((k + 16) > 64)
                        throw "PJPG_DECODE_ERROR";
              
                    for (r = 16; r > 0; r--)
                        gCoeffBuf[ZAG[k++]] = 0;
              
                    k--;
                }
                else
                {
                    break;
                }
            }
        }
     
        while (k < 64)
            gCoeffBuf[ZAG[k++]] = 0;

        transformBlock(mcuBlock); 
    }
         
    return 0;
}

uint8_t App::pjpeg_decode_mcu()
{
    if (!gNumMCUSRemaining)
        return PJPG_NO_MORE_BLOCKS;
      
    decodeNextMCU();
    gNumMCUSRemaining--;
    return 0;
}

uint8_t App::pjpeg_decode_init(pjpeg_image_info_t *pInfo)
{
    pInfo->m_width = 0;
    pInfo->m_height = 0;
    pInfo->m_comps = 0;
    pInfo->m_MCUSPerRow = 0;
    pInfo->m_MCUSPerCol = 0;
    pInfo->m_scanType = PJPG_GRAYSCALE;
    pInfo->m_MCUWidth = 0;
    pInfo->m_MCUHeight = 0;
    pInfo->m_pMCUBufR = (uint8_t *)0;
    pInfo->m_pMCUBufG = (uint8_t *)0;
    pInfo->m_pMCUBufB = (uint8_t *)0;
    gCallbackStatus = 0;
    init();
    locateSOFMarker();
    initFrame();
    initScan();
    pInfo->m_width = gImageXSize;
    pInfo->m_height = gImageYSize;
    pInfo->m_comps = gCompsInFrame;
    pInfo->m_scanType = gScanType;
    pInfo->m_MCUSPerRow = gMaxMCUSPerRow;
    pInfo->m_MCUSPerCol = gMaxMCUSPerCol;
    pInfo->m_MCUWidth = gMaxMCUXSize;
    pInfo->m_MCUHeight = gMaxMCUYSize;
    pInfo->m_pMCUBufR = gMCUBufR;
    pInfo->m_pMCUBufG = gMCUBufG;
    pInfo->m_pMCUBufB = gMCUBufB;  
    return 0;
}



