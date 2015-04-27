#include "common.h"
#include "filesys.h"
#include <sys/stat.h>



uint32_t BZ2_crc32Table[256] = {
   0x00000000L, 0x04c11db7L, 0x09823b6eL, 0x0d4326d9L,
   0x130476dcL, 0x17c56b6bL, 0x1a864db2L, 0x1e475005L,
   0x2608edb8L, 0x22c9f00fL, 0x2f8ad6d6L, 0x2b4bcb61L,
   0x350c9b64L, 0x31cd86d3L, 0x3c8ea00aL, 0x384fbdbdL,
   0x4c11db70L, 0x48d0c6c7L, 0x4593e01eL, 0x4152fda9L,
   0x5f15adacL, 0x5bd4b01bL, 0x569796c2L, 0x52568b75L,
   0x6a1936c8L, 0x6ed82b7fL, 0x639b0da6L, 0x675a1011L,
   0x791d4014L, 0x7ddc5da3L, 0x709f7b7aL, 0x745e66cdL,
   0x9823b6e0L, 0x9ce2ab57L, 0x91a18d8eL, 0x95609039L,
   0x8b27c03cL, 0x8fe6dd8bL, 0x82a5fb52L, 0x8664e6e5L,
   0xbe2b5b58L, 0xbaea46efL, 0xb7a96036L, 0xb3687d81L,
   0xad2f2d84L, 0xa9ee3033L, 0xa4ad16eaL, 0xa06c0b5dL,
   0xd4326d90L, 0xd0f37027L, 0xddb056feL, 0xd9714b49L,
   0xc7361b4cL, 0xc3f706fbL, 0xceb42022L, 0xca753d95L,
   0xf23a8028L, 0xf6fb9d9fL, 0xfbb8bb46L, 0xff79a6f1L,
   0xe13ef6f4L, 0xe5ffeb43L, 0xe8bccd9aL, 0xec7dd02dL,
   0x34867077L, 0x30476dc0L, 0x3d044b19L, 0x39c556aeL,
   0x278206abL, 0x23431b1cL, 0x2e003dc5L, 0x2ac12072L,
   0x128e9dcfL, 0x164f8078L, 0x1b0ca6a1L, 0x1fcdbb16L,
   0x018aeb13L, 0x054bf6a4L, 0x0808d07dL, 0x0cc9cdcaL,
   0x7897ab07L, 0x7c56b6b0L, 0x71159069L, 0x75d48ddeL,
   0x6b93dddbL, 0x6f52c06cL, 0x6211e6b5L, 0x66d0fb02L,
   0x5e9f46bfL, 0x5a5e5b08L, 0x571d7dd1L, 0x53dc6066L,
   0x4d9b3063L, 0x495a2dd4L, 0x44190b0dL, 0x40d816baL,
   0xaca5c697L, 0xa864db20L, 0xa527fdf9L, 0xa1e6e04eL,
   0xbfa1b04bL, 0xbb60adfcL, 0xb6238b25L, 0xb2e29692L,
   0x8aad2b2fL, 0x8e6c3698L, 0x832f1041L, 0x87ee0df6L,
   0x99a95df3L, 0x9d684044L, 0x902b669dL, 0x94ea7b2aL,
   0xe0b41de7L, 0xe4750050L, 0xe9362689L, 0xedf73b3eL,
   0xf3b06b3bL, 0xf771768cL, 0xfa325055L, 0xfef34de2L,
   0xc6bcf05fL, 0xc27dede8L, 0xcf3ecb31L, 0xcbffd686L,
   0xd5b88683L, 0xd1799b34L, 0xdc3abdedL, 0xd8fba05aL,
   0x690ce0eeL, 0x6dcdfd59L, 0x608edb80L, 0x644fc637L,
   0x7a089632L, 0x7ec98b85L, 0x738aad5cL, 0x774bb0ebL,
   0x4f040d56L, 0x4bc510e1L, 0x46863638L, 0x42472b8fL,
   0x5c007b8aL, 0x58c1663dL, 0x558240e4L, 0x51435d53L,
   0x251d3b9eL, 0x21dc2629L, 0x2c9f00f0L, 0x285e1d47L,
   0x36194d42L, 0x32d850f5L, 0x3f9b762cL, 0x3b5a6b9bL,
   0x0315d626L, 0x07d4cb91L, 0x0a97ed48L, 0x0e56f0ffL,
   0x1011a0faL, 0x14d0bd4dL, 0x19939b94L, 0x1d528623L,
   0xf12f560eL, 0xf5ee4bb9L, 0xf8ad6d60L, 0xfc6c70d7L,
   0xe22b20d2L, 0xe6ea3d65L, 0xeba91bbcL, 0xef68060bL,
   0xd727bbb6L, 0xd3e6a601L, 0xdea580d8L, 0xda649d6fL,
   0xc423cd6aL, 0xc0e2d0ddL, 0xcda1f604L, 0xc960ebb3L,
   0xbd3e8d7eL, 0xb9ff90c9L, 0xb4bcb610L, 0xb07daba7L,
   0xae3afba2L, 0xaafbe615L, 0xa7b8c0ccL, 0xa379dd7bL,
   0x9b3660c6L, 0x9ff77d71L, 0x92b45ba8L, 0x9675461fL,
   0x8832161aL, 0x8cf30badL, 0x81b02d74L, 0x857130c3L,
   0x5d8a9099L, 0x594b8d2eL, 0x5408abf7L, 0x50c9b640L,
   0x4e8ee645L, 0x4a4ffbf2L, 0x470cdd2bL, 0x43cdc09cL,
   0x7b827d21L, 0x7f436096L, 0x7200464fL, 0x76c15bf8L,
   0x68860bfdL, 0x6c47164aL, 0x61043093L, 0x65c52d24L,
   0x119b4be9L, 0x155a565eL, 0x18197087L, 0x1cd86d30L,
   0x029f3d35L, 0x065e2082L, 0x0b1d065bL, 0x0fdc1becL,
   0x3793a651L, 0x3352bbe6L, 0x3e119d3fL, 0x3ad08088L,
   0x2497d08dL, 0x2056cd3aL, 0x2d15ebe3L, 0x29d4f654L,
   0xc5a92679L, 0xc1683bceL, 0xcc2b1d17L, 0xc8ea00a0L,
   0xd6ad50a5L, 0xd26c4d12L, 0xdf2f6bcbL, 0xdbee767cL,
   0xe3a1cbc1L, 0xe760d676L, 0xea23f0afL, 0xeee2ed18L,
   0xf0a5bd1dL, 0xf464a0aaL, 0xf9278673L, 0xfde69bc4L,
   0x89b8fd09L, 0x8d79e0beL, 0x803ac667L, 0x84fbdbd0L,
   0x9abc8bd5L, 0x9e7d9662L, 0x933eb0bbL, 0x97ffad0cL,
   0xafb010b1L, 0xab710d06L, 0xa6322bdfL, 0xa2f33668L,
   0xbcb4666dL, 0xb8757bdaL, 0xb5365d03L, 0xb1f740b4L
};

int32_t BZ2_rNums[512] = { 
   619, 720, 127, 481, 931, 816, 813, 233, 566, 247, 
   985, 724, 205, 454, 863, 491, 741, 242, 949, 214, 
   733, 859, 335, 708, 621, 574, 73, 654, 730, 472, 
   419, 436, 278, 496, 867, 210, 399, 680, 480, 51, 
   878, 465, 811, 169, 869, 675, 611, 697, 867, 561, 
   862, 687, 507, 283, 482, 129, 807, 591, 733, 623, 
   150, 238, 59, 379, 684, 877, 625, 169, 643, 105, 
   170, 607, 520, 932, 727, 476, 693, 425, 174, 647, 
   73, 122, 335, 530, 442, 853, 695, 249, 445, 515, 
   909, 545, 703, 919, 874, 474, 882, 500, 594, 612, 
   641, 801, 220, 162, 819, 984, 589, 513, 495, 799, 
   161, 604, 958, 533, 221, 400, 386, 867, 600, 782, 
   382, 596, 414, 171, 516, 375, 682, 485, 911, 276, 
   98, 553, 163, 354, 666, 933, 424, 341, 533, 870, 
   227, 730, 475, 186, 263, 647, 537, 686, 600, 224, 
   469, 68, 770, 919, 190, 373, 294, 822, 808, 206, 
   184, 943, 795, 384, 383, 461, 404, 758, 839, 887, 
   715, 67, 618, 276, 204, 918, 873, 777, 604, 560, 
   951, 160, 578, 722, 79, 804, 96, 409, 713, 940, 
   652, 934, 970, 447, 318, 353, 859, 672, 112, 785, 
   645, 863, 803, 350, 139, 93, 354, 99, 820, 908, 
   609, 772, 154, 274, 580, 184, 79, 626, 630, 742, 
   653, 282, 762, 623, 680, 81, 927, 626, 789, 125, 
   411, 521, 938, 300, 821, 78, 343, 175, 128, 250, 
   170, 774, 972, 275, 999, 639, 495, 78, 352, 126, 
   857, 956, 358, 619, 580, 124, 737, 594, 701, 612, 
   669, 112, 134, 694, 363, 992, 809, 743, 168, 974, 
   944, 375, 748, 52, 600, 747, 642, 182, 862, 81, 
   344, 805, 988, 739, 511, 655, 814, 334, 249, 515, 
   897, 955, 664, 981, 649, 113, 974, 459, 893, 228, 
   433, 837, 553, 268, 926, 240, 102, 654, 459, 51, 
   686, 754, 806, 760, 493, 403, 415, 394, 687, 700, 
   946, 670, 656, 610, 738, 392, 760, 799, 887, 653, 
   978, 321, 576, 617, 626, 502, 894, 679, 243, 440, 
   680, 879, 194, 572, 640, 724, 926, 56, 204, 700, 
   707, 151, 457, 449, 797, 195, 791, 558, 945, 679, 
   297, 59, 87, 824, 713, 663, 412, 693, 342, 606, 
   134, 108, 571, 364, 631, 212, 174, 643, 304, 329, 
   343, 97, 430, 751, 497, 314, 983, 374, 822, 928, 
   140, 206, 73, 263, 980, 736, 876, 478, 430, 305, 
   170, 514, 364, 692, 829, 82, 855, 953, 676, 246, 
   369, 970, 294, 750, 807, 827, 150, 790, 288, 923, 
   804, 378, 215, 828, 592, 281, 565, 555, 710, 82, 
   896, 831, 547, 261, 524, 462, 293, 465, 502, 56, 
   661, 821, 976, 991, 658, 869, 905, 758, 745, 193, 
   768, 550, 608, 933, 378, 286, 215, 979, 792, 961, 
   61, 688, 793, 644, 986, 403, 106, 366, 905, 644, 
   372, 567, 466, 434, 645, 210, 389, 550, 919, 135, 
   780, 773, 635, 389, 707, 100, 626, 958, 165, 504, 
   920, 176, 193, 713, 857, 265, 203, 50, 668, 108, 
   645, 990, 626, 197, 510, 357, 358, 850, 858, 364, 
   936, 638
};

static const uint16_t MTFA_SIZE = 4096;
static const uint16_t MTFL_SIZE = 16;
static const uint16_t BZ_MAX_UNUSED = 5000;
typedef void BZFILE;
static const uint16_t BZ_MAX_ALPHA_SIZE = 258;
static const uint8_t BZ_MAX_CODE_LEN = 23;
static const uint8_t BZ_N_GROUPS = 6;
static const uint16_t BZ_MAX_SELECTORS = 18002;
static const uint8_t BZ_N_SUFFIX_PAIRS = 4;
const char* zSuffix[BZ_N_SUFFIX_PAIRS] = { ".bz2", ".bz", ".tbz2", ".tbz" };
const char* unzSuffix[BZ_N_SUFFIX_PAIRS] = { "", "", ".tar", ".tar" };
static struct stat fileMetaInfo;

struct XState
{
};

struct bz_stream
{
    char *next_in;
    unsigned int avail_in;
    unsigned int total_in_lo32;
    unsigned int total_in_hi32;
    char *next_out;
    unsigned int avail_out;
    unsigned int total_out_lo32;
    unsigned int total_out_hi32;
    XState *state;
    void *opaque;
};

struct EState : public XState
{
    bz_stream *strm;
    int32_t mode;
    int32_t state;
    unsigned avail_in_expect;
    unsigned *arr1;
    unsigned *arr2;
    uint32_t *ftab;
    int32_t origPtr;
    uint32_t *ptr;
    uint8_t *block;
    uint16_t *mtfv;
    uint8_t *zbits;
    int32_t workFactor;
    uint32_t state_in_ch;
    int32_t state_in_len;
    int32_t rNToGo;                  
    int32_t rTPos;
    int32_t nblock;
    int32_t nblockMAX;
    int32_t numZ;
    int32_t state_out_pos;
    int32_t nInUse;
    uint8_t inUse[256];
    uint8_t unseqToSeq[256];
    uint32_t bsBuff;
    int32_t bsLive;
    uint32_t blockCRC;
    uint32_t combinedCRC;
    int32_t verbosity;
    int32_t blockNo;
    int32_t blockSize100k;
    int32_t nMTF;
    int32_t mtfFreq[BZ_MAX_ALPHA_SIZE];
    uint8_t selector[BZ_MAX_SELECTORS];
    uint8_t selectorMtf[BZ_MAX_SELECTORS];
    uint8_t len[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    int32_t code[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    int32_t rfreq[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    uint32_t len_pack[BZ_MAX_ALPHA_SIZE][4];
};

struct UInt64
{
    uint8_t b[8];
};

struct DState : public XState
{
    bz_stream* strm;
    int32_t state;
    uint8_t state_out_ch;
    int32_t state_out_len;
    uint8_t blockRandomised;
    int32_t rNToGo;
    int32_t rTPos;
    uint32_t bsBuff;
    int32_t bsLive;
    int32_t blockSize100k;
    uint8_t smallDecompress;
    int32_t currBlockNo;
    int32_t verbosity;
    int32_t origPtr;
    uint32_t tPos;
    int32_t k0;
    int32_t unzftab[256];
    int32_t nblock_used;
    int32_t cftab[257];
    int32_t cftabCopy[257];
    uint32_t *tt;
    uint16_t *ll16;
    uint8_t *ll4;
    uint32_t storedBlockCRC;
    uint32_t storedCombinedCRC;
    uint32_t calculatedBlockCRC;
    uint32_t calculatedCombinedCRC;
    int32_t nInUse;
    uint8_t inUse[256];
    uint8_t inUse16[16];
    uint8_t seqToUnseq[256];
    uint8_t mtfa[MTFA_SIZE];
    int32_t mtfbase[256 / MTFL_SIZE];
    uint8_t selector[BZ_MAX_SELECTORS];
    uint8_t selectorMtf[BZ_MAX_SELECTORS];
    uint8_t len[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    int32_t limit[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    int32_t base[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    int32_t perm[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
    int32_t minLens[BZ_N_GROUPS];
    int32_t save_i;
    int32_t save_j;
    int32_t save_t;
    int32_t save_alphaSize;
    int32_t save_nGroups;
    int32_t save_nSelectors;
    int32_t save_EOB;
    int32_t save_groupNo;
    int32_t save_groupPos;
    int32_t save_nextSym;
    int32_t save_nblockMAX;
    int32_t save_nblock;
    int32_t save_es;
    int32_t save_N;
    int32_t save_curr;
    int32_t save_zt;
    int32_t save_zn; 
    int32_t save_zvec;
    int32_t save_zj;
    int32_t save_gSel;
    int32_t save_gMinlen;
    int32_t *save_gLimit;
    int32_t *save_gBase;
    int32_t *save_gPerm;
};

struct Cell
{
    char *name;
    Cell *link;
};

struct bzFile
{
    FILE *handle;
    char buf[BZ_MAX_UNUSED];
    int32_t bufN;
    uint8_t writing;
    bz_stream strm;
    int32_t lastErr;
    uint8_t initialisedOk;
};

class AppBzip2
{
    static const uint8_t BZ_RUNA = 0;
    static const uint8_t BZ_RUNB = 1;
    int32_t verbosity;
    uint8_t keepInputFiles, smallMode, deleteOutputOnInterrupt;
    uint8_t forceOverwrite, testFailsExist, unzFailsExist, noisy;
    int32_t numFileNames, numFilesProcessed, blockSize100k;
    int32_t exitValue;
    int32_t opMode;
    int32_t srcMode;
    static const uint16_t FILE_NAME_LEN = 1034;
    int32_t longestFileName;
    char inName [FILE_NAME_LEN];
    char outName[FILE_NAME_LEN];
    char tmpName[FILE_NAME_LEN];
    char *progName;
    char progNameReally[FILE_NAME_LEN];
    FILE *outputHandleJustInCase;
    int32_t workFactor;
    static const uint8_t BZ_HDR_B = 0x42;
    static const uint8_t BZ_HDR_Z = 0x5a;
    static const uint8_t BZ_HDR_h = 0x68;
    static const uint8_t BZ_HDR_0 = 0x30;
    static const uint8_t BZ_RUN = 0;
    static const uint8_t BZ_FLUSH = 1;
    static const uint8_t BZ_FINISH = 2;
    static const uint8_t BZ_OK = 0;
    static const uint8_t BZ_RUN_OK = 1;
    static const uint8_t BZ_FLUSH_OK = 2;
    static const uint8_t BZ_FINISH_OK = 3;
    static const uint8_t BZ_STREAM_END = 4;
    static const int8_t BZ_SEQUENCE_ERROR = -1;
    static const int8_t BZ_PARAM_ERROR = -2;
    static const int8_t BZ_MEM_ERROR = -3;
    static const int8_t BZ_DATA_ERROR = -4;
    static const int8_t BZ_DATA_ERROR_MAGIC = -5;
    static const int8_t BZ_IO_ERROR = -6;
    static const int8_t BZ_UNEXPECTED_EOF = -7;
    static const int8_t BZ_OUTBUFF_FULL = -8;
    static const int8_t BZ_CONFIG_ERROR = -9;
    static const uint8_t SM_I2O = 1;
    static const uint8_t SM_F2O = 2;
    static const uint8_t SM_F2F = 3;
    static const uint8_t OM_Z = 1;
    static const uint8_t OM_UNZ = 2;
    static const uint8_t OM_TEST = 3;
    static const uint8_t BZ_X_MTF_1 = 36;
    static const uint8_t BZ_X_MTF_2 = 37;
    static const uint8_t BZ_X_MTF_3 = 38;
    static const uint8_t BZ_X_MTF_4 = 39;
    static const uint8_t BZ_X_MTF_5 = 40;
    static const uint8_t BZ_X_MTF_6 = 41;
    static const uint8_t BZ_X_CCRC_1 = 47;
    static const uint8_t BZ_X_CCRC_2 = 48;
    static const uint8_t BZ_X_CCRC_3 = 49;
    static const uint8_t BZ_X_CCRC_4 = 50;
    static const uint8_t BZ_X_ORIGPTR_1 = 25;
    static const uint8_t BZ_X_ORIGPTR_2 = 26;
    static const uint8_t BZ_X_ORIGPTR_3 = 27;
    static const uint8_t BZ_X_MAPPING_1 = 28;
    static const uint8_t BZ_X_MAPPING_2 = 29;
    static const uint8_t BZ_X_SELECTOR_1 = 30;
    static const uint8_t BZ_X_SELECTOR_2 = 31;
    static const uint8_t BZ_X_SELECTOR_3 = 32;
    static const uint8_t BZ_X_CODING_1 = 33;
    static const uint8_t BZ_X_CODING_2 = 34;
    static const uint8_t BZ_X_CODING_3 = 35;
    static const uint8_t BZ_M_IDLE = 1;
    static const uint8_t BZ_M_RUNNING = 2;
    static const uint8_t BZ_M_FLUSHING = 3;
    static const uint8_t BZ_M_FINISHING = 4;
    static const uint8_t BZ_S_OUTPUT = 1;
    static const uint8_t BZ_S_INPUT = 2;
    static const uint8_t BZ_N_OVERSHOOT = 34;
    static const uint8_t BZ_X_IDLE = 1;
    static const uint8_t BZ_X_OUTPUT = 2;
    static const uint8_t BZ_X_MAGIC_1 = 10;
    static const uint8_t BZ_X_MAGIC_2 = 11;
    static const uint8_t BZ_X_MAGIC_3 = 12;
    static const uint8_t BZ_X_MAGIC_4 = 13;
    static const uint8_t BZ_X_BLKHDR_1 = 14;
    static const uint8_t BZ_X_BLKHDR_2 = 15;
    static const uint8_t BZ_X_BLKHDR_3 = 16;
    static const uint8_t BZ_X_BLKHDR_4 = 17;
    static const uint8_t BZ_X_BLKHDR_5 = 18;
    static const uint8_t BZ_X_BLKHDR_6 = 19;
    static const uint8_t BZ_X_BCRC_1 = 20;
    static const uint8_t BZ_X_BCRC_2 = 21;
    static const uint8_t BZ_X_BCRC_3 = 22;
    static const uint8_t BZ_X_BCRC_4 = 23;
    static const uint8_t BZ_X_RANDBIT = 24;
    static const uint8_t BZ_X_ENDHDR_2 = 42;
    static const uint8_t BZ_X_ENDHDR_3 = 43;
    static const uint8_t BZ_X_ENDHDR_4 = 44;
    static const uint8_t BZ_X_ENDHDR_5 = 45;
    static const uint8_t BZ_X_ENDHDR_6 = 46;
    static const uint8_t MAIN_QSORT_SMALL_THRESH = 20;
    static const uint8_t MAIN_QSORT_DEPTH_THRESH = 14;
    static const uint8_t MAIN_QSORT_STACK_SIZE = 100;
    static const uint8_t FALLBACK_QSORT_SMALL_THRESH = 10;
    static const uint8_t FALLBACK_QSORT_STACK_SIZE = 100;
    static const uint32_t SETMASK = 1 << 21;
    static const uint32_t CLEARMASK = ~(1<<21);
    static const uint8_t BZ_GREATER_ICOST = 15;

    int BZ2_bzBuffToBuffCompress(char *dest, unsigned int *destLen, char *source, 
                  unsigned int sourceLen, int blockSize100k, int verbosity, int workFactor);

    void mainQSort3(uint32_t *ptr, uint8_t *block, uint16_t *quadrant,
               int32_t nblock, int32_t loSt, int32_t hiSt, int32_t dSt, int32_t *budget);

    void BZ2_bzclose(BZFILE* b);
    int BZ2_bzCompressInit(bz_stream *strm, int blockSize100k, int verbosity, int workFactor);
    BZFILE *BZ2_bzopen(const char *path, const char *mode);

    void mainSort(uint32_t *ptr, uint8_t *block, uint16_t *quadrant, 
                uint32_t *ftab, int32_t nblock, int32_t verb, int32_t *budget);

    BZFILE *BZ2_bzWriteOpen(int *bzerror, FILE *f, int blockSize100k,
        int verbosity, int workFactor);

    void BZ2_bzWriteClose(int *bzerror, BZFILE *b, int abandon, unsigned int *nbytes_in,
                    unsigned int *nbytes_out);

    void mainSimpleSort(uint32_t *ptr, uint8_t *block, uint16_t *quadrant, int32_t nblock,
                      int32_t lo, int32_t hi, int32_t d, int32_t *budget);

    uint8_t mainGtU(uint32_t i1, uint32_t i2, uint8_t* block,  uint16_t* quadrant,
               uint32_t nblock, int32_t* budget);

    void BZ2_hbCreateDecodeTables(int32_t *limit, int32_t *base, int32_t *perm,
            uint8_t *length, int32_t minLen, int32_t maxLen, int32_t alphaSize);

    BZFILE *BZ2_bzReadOpen(int *bzerr, FILE *f, int verbosity, int sm, void *unused, int nUnused);
    //static void default_bzfree(void* opaque, void* addr) { if (addr != NULL) free(addr); }
    //static void default_bzfree(void* opaque, void* addr) { if (addr != NULL) delete[] addr; }
    BZFILE *BZ2_bzdopen(int fd, const char *mode);
    BZFILE *bzopen_or_bzdopen(const char *path, int fd, const char *mode, int open_mode);
    Cell *snocString(Cell *root, char *name);
    int VPrintf0(const char *zf) { return ::fprintf(stderr, zf); }
    void saveInputFileMetaInfo(char *srcName);
    void ERROR_IF_NOT_ZERO(int i);
    void ioError();
    void crcError();
    void uInt64_toAscii(char* outbuf, UInt64* n);
    uint8_t hasSuffix(const char* s, const char* suffix);
    uint8_t uInt64_isZero(UInt64* n);
    uint8_t notAStandardFile(char* name);
    void panic(const char *s);
    int BZ2_bzflush(BZFILE *b);
    void BZ2_bzReadGetUnused(int *bzerror, BZFILE* b, void **unused, int *nUnused);
    void unRLE_obuf_to_output_FAST(DState* s);
    uint8_t copy_output_until_stop(EState* s);
    int32_t uInt64_qrm10(UInt64* n);
    void prepare_new_block(EState *s);
    void BZ_INITIALISE_CRC(uint32_t &crcVar);
    //void applySavedMetaInfoToOutputFile(char *dstName);
    uint8_t mapSuffix(char* name, const char* oldSuffix, const char* newSuffix);
    int BZ2_bzCompressEnd(bz_stream *strm);
    uint8_t myfeof(FILE* f);
    void cleanUpAndFail(int32_t ec);
    uint8_t isempty_RL(EState* s);
    void init_RL(EState* s);
    void flush_RL(EState* s);
    int BZ2_bzDecompressInit(bz_stream* strm, int verbosity, int small);
    int bz_config_ok();
    void compressedStreamEOF();
    void add_pair_to_block(EState* s);
    int BZ2_bzDecompressEnd(bz_stream *strm);
    void BZ2_bzReadClose(int *bzerror, BZFILE *b);
    void makeMaps_d(DState *s);
    uint8_t copy_input_until_stop(EState* s);
    void unRLE_obuf_to_output_SMALL(DState* s);
    int32_t BZ2_indexIntoF(int32_t indx, int32_t *cftab);
    void bsPutUChar(EState* s, uint8_t c);
    void bsW(EState *s, int32_t n, uint32_t v);
    void bsFinishWrite(EState *s);
    int32_t MYMAX(int32_t zz2, int32_t zz3);
    uint8_t mmed3(uint8_t a, uint8_t b, uint8_t c);
    void BZ2_bsInitWrite(EState *s);
    void bsPutUInt32(EState* s, uint32_t u);
    uint8_t containsDubiousChars(char *name);
    void redundant(char* flag);
    void addFlagsFromEnvVar(Cell **argList, const char *varName);
    void compressStream(FILE *stream, FILE *zStream);
    void copyFileName(char* to, const char* from);
    void compress(char *name);
    void usage(char *fullProgName);
    void license();
    void uncompress(char *name);
    void testf(char *name);
    void fallbackQSort3(uint32_t* fmap, uint32_t* eclass, int32_t loSt, int32_t hiSt);
    void BZ2_compressBlock(EState* s, uint8_t is_last_block);
    void generateMTFValues(EState *s);
    void BZ2_blockSort(EState *s);
    void makeMaps_e(EState* s);
    void sendMTFValues(EState* s);
    uint8_t handle_compress(bz_stream* strm);
    int BZ2_bzCompress(bz_stream *strm, int action);
    void BZ2_bzWrite(int *bzerror, BZFILE* b, void *buf, int len);
    int BZ2_bzwrite(BZFILE* b, void* buf, int len);
    void fallbackSimpleSort(uint32_t *fmap, uint32_t *eclass, int32_t lo, int32_t hi);
    const char *BZ2_bzlibVersion();
    void BZ2_bz__AssertH__fail(int errcode);
    void AssertH(bool cond, uint32_t errcode);
    int32_t BZ2_decompress(DState *s);
    int BZ2_bzDecompress(bz_stream *strm);
    int BZ2_bzRead(int *bzerror, BZFILE* b, void *buf, int len);
    int BZ2_bzread(BZFILE* b, void* buf, int len);
    uint8_t uncompressStream(FILE *zStream, FILE *stream);
    uint8_t testStream(FILE *zStream);
    void outOfMemory();
    void showFileNames();
    void configError();
    void fswap(uint32_t &zz1, uint32_t &zz2);
    Cell *mkCell();
    const char *BZ2_bzerror(BZFILE *b, int *errnum);
    void uInt64_from_UInt32s(UInt64* n, uint32_t lo32, uint32_t hi32);
    double uInt64_to_double(UInt64 *n);
    void setExit(int32_t v);
    void cadvise();
    void BZ_UPDATE_CRC(uint32_t &crcVar, uint8_t &cha);
    void BZ2_hbMakeCodeLengths(uint8_t *len, int32_t *freq, int32_t alphaSize, int32_t maxLen);
    int32_t countHardLinks(char *name);
    FILE *fopen_output_safely(char* name, const char* mode);
    uint8_t fileExists(char *name);
    void pad(char *s);
    void BZ2_hbAssignCodes(int32_t *, uint8_t *, int32_t, int32_t, int32_t);
    void fallbackSort(uint32_t*, uint32_t*, uint32_t *, int32_t, int32_t);

    int BZ2_bzBuffToBuffDecompress(char *dest, unsigned int* destLen,
                      char *source, unsigned int sourceLen, int small, int verbosity);

    void BZ2_bzWriteClose64(int *bzerror, BZFILE *b, int abandon, unsigned int *nbytes_in_lo32,
      unsigned int *nbytes_in_hi32, unsigned int *nbytes_out_lo32, unsigned int *nbytes_out_hi32);
public:
    int run(int argc, char **argv);
};

void AppBzip2::BZ_UPDATE_CRC(uint32_t &crcVar, uint8_t &cha)
{
    crcVar = (crcVar << 8) ^ BZ2_crc32Table[(crcVar >> 24) ^ ((uint8_t)cha)];
}

void AppBzip2::fallbackSimpleSort(uint32_t *fmap, uint32_t *eclass, int32_t lo, int32_t hi)
{
    int32_t i, j, tmp;
    uint32_t ec_tmp;
    if (lo == hi) return;

    if (hi - lo > 3)
    {
        for ( i = hi-4; i >= lo; i-- )
        {
            tmp = fmap[i];
            ec_tmp = eclass[tmp];

            for (j = i+4; j <= hi && ec_tmp > eclass[fmap[j]]; j += 4)
                fmap[j-4] = fmap[j];

            fmap[j-4] = tmp;
        }
    }

    for (i = hi-1; i >= lo; i--)
    {
        tmp = fmap[i];
        ec_tmp = eclass[tmp];

        for ( j = i+1; j <= hi && ec_tmp > eclass[fmap[j]]; j++ )
            fmap[j-1] = fmap[j];

        fmap[j-1] = tmp;
    }
}

const char *AppBzip2::BZ2_bzlibVersion()
{
    return "1.0.2, 30-Dec-2001";
}

void AppBzip2::BZ2_bz__AssertH__fail(int errcode )
{
    fprintf(stderr, 
      "\n\nbzip2/libbzip2: internal error number %d.\n"
      "This is a bug in bzip2/libbzip2, %s.\n"
      "Please report it to me at: jseward@acm.org.  If this happened\n"
      "when you were using some program which uses libbzip2 as a\n"
      "component, you should also report this bug to the author(s)\n"
      "of that program.  Please make an effort to report this bug;\n"
      "timely and accurate bug reports eventually lead to higher\n"
      "quality software.  Thanks.  Julian Seward, 30 December 2001.\n\n",
      errcode,
      BZ2_bzlibVersion());

    if (errcode == 1007)
    {
        fprintf(stderr,
      "\n*** A special note about internal error number 1007 ***\n"
      "\n"
      "Experience suggests that a common cause of i.e. 1007\n"
      "is unreliable memory or other hardware.  The 1007 assertion\n"
      "just happens to cross-check the results of huge numbers of\n"
      "memory reads/writes, and so acts (unintendedly) as a stress\n"
      "test of your memory system.\n"
      "\n"
      "I suggest the following: try compressing the file again,\n"
      "possibly monitoring progress in detail with the -vv flag.\n"
      "\n"
      "* If the error cannot be reproduced, and/or happens at different\n"
      "  points in compression, you may have a flaky memory system.\n"
      "  Try a memory-test program.  I have used Memtest86\n"
      "  (www.memtest86.com).  At the time of writing it is free (GPLd).\n"
      "  Memtest86 tests memory much more thorougly than your BIOSs\n"
      "  power-on test, and may find failures that the BIOS doesn't.\n"
      "\n"
      "* If the error can be repeatably reproduced, this is a bug in\n"
      "  bzip2, and I would very much like to hear about it.  Please\n"
      "  let me know, and, ideally, save a copy of the file causing the\n"
      "  problem -- without which I will be unable to investigate it.\n"
      "\n"
    );
    }

    //exit(3);
    throw "3";
}

void AppBzip2::AssertH(bool cond, uint32_t errcode)
{
    if (!cond)
        BZ2_bz__AssertH__fail(errcode);
}

void AppBzip2::fswap(uint32_t &zz1, uint32_t &zz2)
{
    int32_t zztmp = zz1;
    zz1 = zz2;
    zz2 = zztmp;
}

void AppBzip2::fallbackQSort3(uint32_t* fmap, uint32_t* eclass, int32_t loSt, int32_t hiSt)
{
    int32_t unLo, unHi, ltLo, gtHi, n, m;
    int32_t sp, lo, hi;
    uint32_t med, r, r3;
    int32_t stackLo[FALLBACK_QSORT_STACK_SIZE];
    int32_t stackHi[FALLBACK_QSORT_STACK_SIZE];
    r = 0;
    sp = 0;
    stackLo[sp] = loSt;
    stackHi[sp] = hiSt;
    sp++;

    while (sp > 0)
    {
        AssertH(sp < FALLBACK_QSORT_STACK_SIZE, 1004);
        sp--;
        lo = stackLo[sp];
        hi = stackHi[sp];

        if (hi - lo < FALLBACK_QSORT_SMALL_THRESH)
        {
            fallbackSimpleSort ( fmap, eclass, lo, hi );
            continue;
        }

        r = ((r * 7621) + 1) % 32768;
        r3 = r % 3;
        if (r3 == 0) med = eclass[fmap[lo]]; else
        if (r3 == 1) med = eclass[fmap[(lo+hi)>>1]]; else
                   med = eclass[fmap[hi]];

        unLo = ltLo = lo;
        unHi = gtHi = hi;

        while (1) {
         while (1) {
            if (unLo > unHi) break;
            n = (int32_t)eclass[fmap[unLo]] - (int32_t)med;
            if (n == 0)
            {
                uint32_t zztmp = fmap[unLo]; fmap[unLo] = fmap[ltLo]; fmap[ltLo] = zztmp;
               ltLo++; unLo++; 
               continue; 
            };
            if (n > 0) break;
            unLo++;
         }
         while (1) {
            if (unLo > unHi) break;
            n = (int32_t)eclass[fmap[unHi]] - (int32_t)med;
            if (n == 0) { 
               fswap(fmap[unHi], fmap[gtHi]); 
               gtHi--; unHi--; 
               continue; 
            };
            if (n < 0) break;
            unHi--;
         }
         if (unLo > unHi) break;
         fswap(fmap[unLo], fmap[unHi]); unLo++; unHi--;
      }

      if (gtHi < ltLo) continue;

        n = ((ltLo-lo) < (unLo-ltLo)) ? (ltLo-lo) : (unLo-ltLo);

        for (int32_t yyp1 = lo, yyp2 = unLo - n, yyn = n; yyn > 0;)
        {
            fswap(fmap[yyp1], fmap[yyp2]);
            yyp1++; yyp2++; yyn--;
        }


        m = ((hi-gtHi) < (gtHi-unHi)) ? (hi-gtHi) : (gtHi-unHi);

        for (int32_t yyp1 = unLo, yyp2 = hi - m + 1, yyn = m; yyn > 0;)
        {
            fswap(fmap[yyp1], fmap[yyp2]);
            yyp1++; yyp2++; yyn--;
        }

        n = lo + unLo - ltLo - 1;
        m = hi - (gtHi - unHi) + 1;

        if (n - lo > hi - m)
        {
            stackLo[sp] = lo;
            stackHi[sp] = n;
            sp++;

            stackLo[sp] = m;
            stackHi[sp] = hi;
            sp++;
        }
        else
        {
            stackLo[sp] = m;
            stackHi[sp] = hi;
            sp++;

            stackLo[sp] = lo;
            stackHi[sp] = n;
            sp++;
        }
    }
}

void AppBzip2::fallbackSort(uint32_t* fmap, uint32_t* eclass, uint32_t *bhtab, int32_t nblock,
        int32_t verb)
{
    int32_t ftab[257];
    int32_t ftabCopy[256];
    int32_t H, i, j, k, l, r, cc, cc1;
    int32_t nNotDone;
    int32_t nBhtab;
    uint8_t* eclass8 = (uint8_t*)eclass;
    if (verb >= 4) fprintf(stderr, "        bucket sorting ...\n" );
    for (i = 0; i < 257;    i++) ftab[i] = 0;
    for (i = 0; i < nblock; i++) ftab[eclass8[i]]++;
    for (i = 0; i < 256;    i++) ftabCopy[i] = ftab[i];
    for (i = 1; i < 257;    i++) ftab[i] += ftab[i-1];

    for (i = 0; i < nblock; i++)
    {
        j = eclass8[i];
        k = ftab[j] - 1;
        ftab[j] = k;
        fmap[k] = i;
    }

    nBhtab = 2 + (nblock / 32);

    for (i = 0; i < nBhtab; i++)
        bhtab[i] = 0;

    for (i = 0; i < 256; i++)
        bhtab[(ftab[i]) >> 5] |= (1 << ((ftab[i]) & 31));

    for (i = 0; i < 32; i++)
    {
        int32_t w = nblock + 2 * i;
        bhtab[(w) >> 5] |= (1 << (w & 31));
        int32_t x = nblock + 2 * i + 1;
        bhtab[x >> 5] &= ~(1 << (x & 31));
    }

    H = 1;
    while (1)
    {

        if (verb >= 4) 
            fprintf(stderr, "        depth %6d has ", H );

        j = 0;

        for (i = 0; i < nblock; i++)
        {
            if (bhtab[(i) >> 5] & (1 << ((i) & 31)))
                j = i;

            k = fmap[i] - H; if (k < 0) k += nblock;
            eclass[k] = j;
        }

        nNotDone = 0;
        r = -1;

        while (1)
        {
            k = r + 1;

            while (bhtab[(k) >> 5] & (1 << ((k) & 31)) && (k & 0x01f))
                k++;

            if (bhtab[(k) >> 5] & (1 << ((k) & 31)))
            {
                while ((bhtab[(k) >> 5])  == 0xffffffff)
                    k += 32;

                while (bhtab[(k) >> 5] & (1 << ((k) & 31)))
                    k++;
            }
            l = k - 1;

            if (l >= nblock)
                break;

            while (!(bhtab[(k) >> 5] & (1 << ((k) & 31)))        && (k & 0x01f))
                k++;

            if (  !(bhtab[(k) >> 5] & (1 << ((k) & 31)))           )
            {
                while (  (bhtab[(k) >> 5])  == 0x00000000) k += 32;
                while (!(bhtab[(k) >> 5] & (1 << ((k) & 31))) ) k++;
         }
         r = k - 1;
         if (r >= nblock) break;

         if (r > l) {
            nNotDone += (r - l + 1);
            fallbackQSort3 ( fmap, eclass, l, r );

                cc = -1;
                for (i = l; i <= r; i++)
                {
                    cc1 = eclass[fmap[i]];
                    if (cc != cc1)
                    {
                        bhtab[(i) >> 5] |= (1 << ((i) & 31));
                        cc = cc1;
                    };
                }
            }
        }

        if (verb >= 4) 
            fprintf(stderr, "%6d unresolved strings\n", nNotDone );

        H *= 2;
        if (H > nblock || nNotDone == 0)
            break;
    }

    if (verb >= 4)
        VPrintf0 ( "        reconstructing block ...\n" );

    j = 0;
    for (i = 0; i < nblock; i++)
    {
        while (ftabCopy[j] == 0) j++;
        ftabCopy[j]--;
        eclass8[fmap[i]] = (uint8_t)j;
    }
    AssertH ( j < 256, 1005 );
}

uint8_t AppBzip2::mainGtU(uint32_t i1, uint32_t i2, uint8_t* block, uint16_t* quadrant,
               uint32_t nblock, int32_t* budget)
{
    int32_t k;
    uint8_t c1, c2;
    uint16_t s1, s2;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    k = nblock + 8;

    do
    {
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;
      c1 = block[i1]; c2 = block[i2];
      if (c1 != c2) return (c1 > c2);
      s1 = quadrant[i1]; s2 = quadrant[i2];
      if (s1 != s2) return (s1 > s2);
      i1++; i2++;

      if (i1 >= nblock) i1 -= nblock;
      if (i2 >= nblock) i2 -= nblock;

      k -= 8;
      (*budget)--;
   }
      while (k >= 0);

   return 0;
}

static int32_t incs[14] = { 1, 4, 13, 40, 121, 364, 1093, 3280, 9841, 29524, 88573, 265720,
                   797161, 2391484 };

void AppBzip2::mainSimpleSort(uint32_t *ptr, uint8_t *block, uint16_t *quadrant, int32_t nblock,
                      int32_t lo, int32_t hi, int32_t d, int32_t *budget)
{
    int32_t i, j, h, bigN, hp;
    uint32_t v;
    bigN = hi - lo + 1;
    if (bigN < 2) return;
    hp = 0;
    while (incs[hp] < bigN) hp++;
    hp--;

    for (; hp >= 0; hp--)
    {
        h = incs[hp];
        i = lo + h;
        while (1)
        {
         if (i > hi) break;
         v = ptr[i];
         j = i;
         while ( mainGtU ( 
                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget 
                 ) ) {
            ptr[j] = ptr[j-h];
            j = j - h;
            if (j <= (lo + h - 1)) break;
         }
         ptr[j] = v;
         i++;

         if (i > hi) break;
         v = ptr[i];
         j = i;
         while ( mainGtU ( 
                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget 
                 ) ) {
            ptr[j] = ptr[j-h];
            j = j - h;
            if (j <= (lo + h - 1)) break;
         }
         ptr[j] = v;
         i++;

         if (i > hi) break;
         v = ptr[i];
         j = i;
         while ( mainGtU ( 
                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget 
                 ) ) {
            ptr[j] = ptr[j-h];
            j = j - h;
            if (j <= (lo + h - 1)) break;
         }
         ptr[j] = v;
         i++;

         if (*budget < 0) return;
      }
   }
}

uint8_t AppBzip2::mmed3(uint8_t a, uint8_t b, uint8_t c)
{
    uint8_t t;

    if (a > b) { t = a; a = b; b = t; };

    if (b > c)
    {
        b = c;

        if (a > b)
            b = a;
    }

    return b;
}

void AppBzip2::mainQSort3(uint32_t *ptr, uint8_t *block, uint16_t *quadrant,
               int32_t nblock, int32_t loSt, int32_t hiSt, int32_t dSt, int32_t *budget)
{
    int32_t unLo, unHi, ltLo, gtHi, n, m, med;
    int32_t lo, hi, d;
    int32_t stackLo[MAIN_QSORT_STACK_SIZE];
    int32_t stackHi[MAIN_QSORT_STACK_SIZE];
    int32_t stackD [MAIN_QSORT_STACK_SIZE];
    int32_t nextLo[3];
    int32_t nextHi[3];
    int32_t nextD [3];
    int32_t sp = 0;
    stackLo[sp] = loSt;
    stackHi[sp] = hiSt;
    stackD[sp] = dSt;
    sp++;

    while (sp > 0)
    {
        AssertH ( sp < MAIN_QSORT_STACK_SIZE, 1001 );
        sp--; lo = stackLo[sp]; hi = stackHi[sp]; d = stackD[sp];

        if (hi - lo < MAIN_QSORT_SMALL_THRESH || d > MAIN_QSORT_DEPTH_THRESH)
        {
            mainSimpleSort ( ptr, block, quadrant, nblock, lo, hi, d, budget );
            if (*budget < 0) return;
            continue;
        }

        med = (int32_t)mmed3 ( block[ptr[ lo         ]+d],
                    block[ptr[ hi         ]+d],
                    block[ptr[ (lo+hi)>>1 ]+d] );

        unLo = ltLo = lo;
        unHi = gtHi = hi;

        while (1)
        {
            while (1) {
            if (unLo > unHi) break;
            n = ((int32_t)block[ptr[unLo]+d]) - med;
            if (n == 0)
            {
                int32_t zztmp = ptr[unLo];
                ptr[unLo] = ptr[ltLo];
                ptr[ltLo] = zztmp;
               ltLo++; unLo++; continue; 
            };
            if (n >  0) break;
            unLo++;
         }
         while (1) {
            if (unLo > unHi) break;
            n = ((int32_t)block[ptr[unHi]+d]) - med;
            if (n == 0) {
                int32_t zztmp = ptr[unHi];
                ptr[unHi] = ptr[gtHi];
                ptr[gtHi] = zztmp;
               gtHi--; unHi--; continue; 
            };
            if (n <  0) break;
            unHi--;
         }
         if (unLo > unHi) break;
        int32_t zztmp = ptr[unLo];
        ptr[unLo] = ptr[unHi];
        ptr[unHi] = zztmp;
        unLo++; unHi--;
      }


        if (gtHi < ltLo)
        {
            stackLo[sp] = lo;
            stackHi[sp] = hi;
            stackD[sp] = d + 1;
            sp++;
            continue;
        }

        n = ((ltLo-lo) < (unLo-ltLo)) ? (ltLo-lo) : (unLo-ltLo);


        for (int32_t yyp1 = lo, yyp2 = unLo - n, yyn = n; yyn > 0;)
        {
            int32_t zztmp = ptr[yyp1];
            ptr[yyp1] = ptr[yyp2];
            ptr[yyp2] = zztmp;
            yyp1++; yyp2++; yyn--;
        }

        m = ((hi-gtHi) < (gtHi-unHi)) ? (hi-gtHi) : (gtHi-unHi);

        for (int32_t yyp1 = unLo, yyp2 = hi - m + 1, yyn = m; yyn > 0;)
        {
            int32_t zztmp = ptr[yyp1];
            ptr[yyp1] = ptr[yyp2];
            ptr[yyp2] = zztmp;
            yyp1++; yyp2++; yyn--;
        }

        n = lo + unLo - ltLo - 1;
        m = hi - (gtHi - unHi) + 1;
        nextLo[0] = lo;
        nextHi[0] = n;   nextD[0] = d;
        nextLo[1] = m;   nextHi[1] = hi;  nextD[1] = d;
        nextLo[2] = n+1; nextHi[2] = m-1; nextD[2] = d+1;

        if ((nextHi[0] - nextLo[0]) < (nextHi[1] - nextLo[1]))
        {
            int32_t tz;
            tz = nextLo[0]; nextLo[0] = nextLo[1]; nextLo[1] = tz;
            tz = nextHi[0]; nextHi[0] = nextHi[1]; nextHi[1] = tz;
            tz = nextD[0]; nextD[0] = nextD[1]; nextD[1] = tz;
        }

        if ((nextHi[1] - nextLo[1]) < (nextHi[2] - nextLo[2]))
        {
            int32_t tz;
            tz = nextLo[1]; nextLo[1] = nextLo[2]; nextLo[2] = tz;
            tz = nextHi[1]; nextHi[1] = nextHi[2]; nextHi[2] = tz;
            tz = nextD[1]; nextD[1] = nextD[2]; nextD[2] = tz;
        }

        if ((nextHi[0] - nextLo[0]) < (nextHi[1] - nextLo[1]))
        {
            int32_t tz;
            tz = nextLo[0]; nextLo[0] = nextLo[1]; nextLo[1] = tz;
            tz = nextHi[0]; nextHi[0] = nextHi[1]; nextHi[1] = tz;
            tz = nextD[0]; nextD[0] = nextD[1]; nextD[1] = tz;
        }

        stackLo[sp] = nextLo[0];
        stackHi[sp] = nextHi[0];
        stackD[sp] = nextD[0];
        sp++;
        stackLo[sp] = nextLo[1];
        stackHi[sp] = nextHi[1];
        stackD[sp] = nextD[1];
        sp++;
        stackLo[sp] = nextLo[2];
        stackHi[sp] = nextHi[2];
        stackD[sp] = nextD[2];
        sp++;
    }
}

void AppBzip2::mainSort(uint32_t *ptr, uint8_t *block, uint16_t *quadrant, 
                uint32_t *ftab, int32_t nblock, int32_t verb, int32_t *budget)
{
    int32_t i, j, k, ss, sb;
    int32_t runningOrder[256];
    uint8_t bigDone[256];
    int32_t copyStart[256];
    int32_t copyEnd  [256];
    uint8_t c1;
    int32_t numQSorted;
    uint16_t s;
    if (verb >= 4) VPrintf0 ( "        main sort initialise ...\n" );
    for (i = 65536; i >= 0; i--) ftab[i] = 0;
    j = block[0] << 8;
    i = nblock-1;

    for (; i >= 3; i -= 4)
    {
        quadrant[i] = 0;
        j = (j >> 8) | ( ((uint16_t)block[i]) << 8);
        ftab[j]++;
        quadrant[i-1] = 0;
        j = (j >> 8) | ( ((uint16_t)block[i-1]) << 8);
        ftab[j]++;
        quadrant[i-2] = 0;
        j = (j >> 8) | ( ((uint16_t)block[i-2]) << 8);
        ftab[j]++;
        quadrant[i-3] = 0;
        j = (j >> 8) | ( ((uint16_t)block[i-3]) << 8);
        ftab[j]++;
    }

    for (; i >= 0; i--)
    {
        quadrant[i] = 0;
        j = (j >> 8) | ( ((uint16_t)block[i]) << 8);
        ftab[j]++;
    }

    for (i = 0; i < BZ_N_OVERSHOOT; i++)
    {
        block   [nblock+i] = block[i];
        quadrant[nblock+i] = 0;
    }

    if (verb >= 4) VPrintf0 ( "        bucket sorting ...\n" );
    for (i = 1; i <= 65536; i++) ftab[i] += ftab[i-1];
    s = block[0] << 8;
    i = nblock-1;

    for (; i >= 3; i -= 4)
    {
        s = (s >> 8) | (block[i] << 8);
        j = ftab[s] -1;
        ftab[s] = j;
        ptr[j] = i;
        s = (s >> 8) | (block[i-1] << 8);
        j = ftab[s] -1;
        ftab[s] = j;
        ptr[j] = i-1;
        s = (s >> 8) | (block[i-2] << 8);
        j = ftab[s] -1;
        ftab[s] = j;
        ptr[j] = i-2;
        s = (s >> 8) | (block[i-3] << 8);
        j = ftab[s] -1;
        ftab[s] = j;
        ptr[j] = i-3;
    }

    for (; i >= 0; i--)
    {
        s = (s >> 8) | (block[i] << 8);
        j = ftab[s] -1;
        ftab[s] = j;
        ptr[j] = i;
    }

    for (i = 0; i <= 255; i++)
    {
        bigDone     [i] = 0;
        runningOrder[i] = i;
    }

   {
      int32_t vv;
      int32_t h = 1;
      do h = 3 * h + 1; while (h <= 256);
      do {
         h = h / 3;
         for (i = h; i <= 255; i++) {
            vv = runningOrder[i];
            j = i;
            while ((ftab[((runningOrder[j-h]) + 1) << 8] - ftab[(runningOrder[j-h]) << 8])
                    >  (ftab[((vv)+1) << 8] - ftab[(vv) << 8]))
            {
               runningOrder[j] = runningOrder[j-h];
               j = j - h;
               if (j <= (h - 1)) goto zero;
            }
            zero:
            runningOrder[j] = vv;
         }
      } while (h != 1);
   }

   numQSorted = 0;

   for (i = 0; i <= 255; i++) {

      ss = runningOrder[i];

      for (j = 0; j <= 255; j++) {
         if (j != ss) {
            sb = (ss << 8) + j;
            if ( ! (ftab[sb] & SETMASK) ) {
               int32_t lo = ftab[sb]   & CLEARMASK;
               int32_t hi = (ftab[sb+1] & CLEARMASK) - 1;
               if (hi > lo) {
                  if (verb >= 4)
                     fprintf(stderr, "        qsort [0x%x, 0x%x]   "
                                "done %d   this %d\n",
                                ss, j, numQSorted, hi - lo + 1 );
                  mainQSort3 ( 
                     ptr, block, quadrant, nblock, 
                     lo, hi, 2, budget 
                  );   
                  numQSorted += (hi - lo + 1);
                  if (*budget < 0) return;
               }
            }
            ftab[sb] |= SETMASK;
         }
      }

      AssertH ( !bigDone[ss], 1006 );
      {
         for (j = 0; j <= 255; j++) {
            copyStart[j] =  ftab[(j << 8) + ss]     & CLEARMASK;
            copyEnd  [j] = (ftab[(j << 8) + ss + 1] & CLEARMASK) - 1;
         }
         for (j = ftab[ss << 8] & CLEARMASK; j < copyStart[ss]; j++) {
            k = ptr[j]-1; if (k < 0) k += nblock;
            c1 = block[k];
            if (!bigDone[c1])
               ptr[ copyStart[c1]++ ] = k;
         }
         for (j = (ftab[(ss+1) << 8] & CLEARMASK) - 1; j > copyEnd[ss]; j--) {
            k = ptr[j]-1; if (k < 0) k += nblock;
            c1 = block[k];
            if (!bigDone[c1]) 
               ptr[ copyEnd[c1]-- ] = k;
         }
      }

      AssertH ( (copyStart[ss]-1 == copyEnd[ss])
                || 
      
                (copyStart[ss] == 0 && copyEnd[ss] == nblock-1),
                1007 );

      for (j = 0; j <= 255; j++) ftab[(j << 8) + ss] |= SETMASK;

      bigDone[ss] = 1;

      if (i < 255) {
         int32_t bbStart  = ftab[ss << 8] & CLEARMASK;
         int32_t bbSize   = (ftab[(ss+1) << 8] & CLEARMASK) - bbStart;
         int32_t shifts   = 0;

         while ((bbSize >> shifts) > 65534) shifts++;

         for (j = bbSize-1; j >= 0; j--) {
            int32_t a2update     = ptr[bbStart + j];
            uint16_t qVal        = (uint16_t)(j >> shifts);
            quadrant[a2update] = qVal;
            if (a2update < BZ_N_OVERSHOOT)
               quadrant[a2update + nblock] = qVal;
         }
         AssertH ( ((bbSize-1) >> shifts) <= 65535, 1002 );
      }

   }

   if (verb >= 4)
      fprintf(stderr, "        %d pointers, %d sorted, %d scanned\n",
                 nblock, numQSorted, nblock - numQSorted );
}

void AppBzip2::BZ2_blockSort(EState* s)
{
    uint32_t *ptr    = s->ptr; 
    uint8_t *block  = s->block;
    uint32_t *ftab   = s->ftab;
    int32_t nblock = s->nblock;
    int32_t verb   = s->verbosity;
    int32_t wfact  = s->workFactor;
    uint16_t *quadrant;
    int32_t budget;
    int32_t budgetInit;
    int32_t i;

    if (nblock < 10000) {
        fallbackSort ( s->arr1, s->arr2, ftab, nblock, verb );
    } else {
        i = nblock+BZ_N_OVERSHOOT;
        if (i & 1) i++;
        quadrant = (uint16_t*)(&(block[i]));
        if (wfact < 1  ) wfact = 1;
        if (wfact > 100) wfact = 100;
        budgetInit = nblock * ((wfact-1) / 3);
        budget = budgetInit;

      mainSort ( ptr, block, quadrant, ftab, nblock, verb, &budget );
      if (verb >= 3) 
         fprintf(stderr, "      %d work, %d block, ratio %5.2f\n",
                    budgetInit - budget,
                    nblock, 
                    (float)(budgetInit - budget) /
                    (float)(nblock==0 ? 1 : nblock) ); 
      if (budget < 0) {
         if (verb >= 2) 
            VPrintf0 ( "    too repetitive; using fallback"
                       " sorting algorithm\n" );
         fallbackSort ( s->arr1, s->arr2, ftab, nblock, verb );
      }
    }

    s->origPtr = -1;
    for (i = 0; i < s->nblock; i++)
        if (ptr[i] == 0)
         { s->origPtr = i; break; };

    AssertH( s->origPtr != -1, 1003 );
}


int32_t AppBzip2::MYMAX(int32_t zz2, int32_t zz3)
{
    return zz2 > zz3 ? zz2 : zz3;
}

void AppBzip2::BZ2_hbMakeCodeLengths(uint8_t *len, int32_t *freq,
    int32_t alphaSize, int32_t maxLen)
{
    int32_t nNodes, nHeap, n1, n2, i, j, k;
    uint8_t tooLong;

    int32_t heap   [ BZ_MAX_ALPHA_SIZE + 2 ];
    int32_t weight [ BZ_MAX_ALPHA_SIZE * 2 ];
    int32_t parent [ BZ_MAX_ALPHA_SIZE * 2 ]; 

    for (i = 0; i < alphaSize; i++)
        weight[i+1] = (freq[i] == 0 ? 1 : freq[i]) << 8;

    while (1)
    {
        nNodes = alphaSize;
        nHeap = 0;

        heap[0] = 0;
        weight[0] = 0;
        parent[0] = -2;

        for (i = 1; i <= alphaSize; i++)
        {
            parent[i] = -1;
            nHeap++;
            heap[nHeap] = i;

            int32_t zz, tmp;
            zz = nHeap;
            tmp = heap[zz];
            while (weight[tmp] < weight[heap[zz >> 1]]) {
                heap[zz] = heap[zz >> 1];
                zz >>= 1;
            }
            heap[zz] = tmp;

        }

        AssertH( nHeap < (BZ_MAX_ALPHA_SIZE+2), 2001 );
   
        while (nHeap > 1)
        {
            n1 = heap[1]; heap[1] = heap[nHeap]; nHeap--;

            {
                int32_t zz, yy, tmp;
                zz = 1; tmp = heap[zz];

                while (1)
                {
                    yy = zz << 1;
                    if (yy > nHeap) break;
                    if (yy < nHeap && weight[heap[yy+1]] < weight[heap[yy]]) yy++;
                    if (weight[tmp] < weight[heap[yy]]) break;
                    heap[zz] = heap[yy];
                    zz = yy;
                }

                heap[zz] = tmp;
            }

            n2 = heap[1]; heap[1] = heap[nHeap]; nHeap--;

            {
                int32_t zz, yy, tmp;
                zz = 1; tmp = heap[zz];

                while (1)
                {
                    yy = zz << 1;
                    if (yy > nHeap) break;
                    if (yy < nHeap && weight[heap[yy+1]] < weight[heap[yy]]) yy++;
                    if (weight[tmp] < weight[heap[yy]]) break;
                    heap[zz] = heap[yy];
                    zz = yy;
                }

                heap[zz] = tmp;
            }


            nNodes++;
            parent[n1] = parent[n2] = nNodes;

            weight[nNodes] = ( (weight[n1] & 0xffffff00)+ (weight[n2] & 0xffffff00)) |
                    (1 + MYMAX(weight[n1] & 0x000000ff, weight[n2] & 0x000000ff));

            parent[nNodes] = -1;
            nHeap++;
            heap[nHeap] = nNodes;
            int32_t zz, tmp;
            zz = nHeap;
            tmp = heap[zz];
            while (weight[tmp] < weight[heap[zz >> 1]]) {
                heap[zz] = heap[zz >> 1];
                zz >>= 1;
            }
            heap[zz] = tmp;
        }

        AssertH( nNodes < (BZ_MAX_ALPHA_SIZE * 2), 2002 );

      tooLong = 0;
      for (i = 1; i <= alphaSize; i++) {
         j = 0;
         k = i;
         while (parent[k] >= 0) { k = parent[k]; j++; }
         len[i-1] = j;
         if (j > maxLen) tooLong = 1;
      }
      
      if (! tooLong) break;

      for (i = 1; i < alphaSize; i++) {
         j = weight[i] >> 8;
         j = 1 + (j / 2);
         weight[i] = j << 8;
      }
   }
}

void AppBzip2::BZ2_hbAssignCodes(int32_t *code, uint8_t *length, int32_t minLen, int32_t maxLen,
                       int32_t alphaSize)
{
    int32_t vec = 0;

    for (int32_t n = minLen; n <= maxLen; n++)
    {
        for (int32_t i = 0; i < alphaSize; i++)
            if (length[i] == n) { code[i] = vec; vec++; };

        vec <<= 1;
   }
}

void AppBzip2::BZ2_hbCreateDecodeTables(int32_t *limit, int32_t *base, int32_t *perm,
            uint8_t *length, int32_t minLen, int32_t maxLen, int32_t alphaSize)
{
    int32_t pp, i, j, vec;
    pp = 0;

    for (i = minLen; i <= maxLen; i++)
        for (j = 0; j < alphaSize; j++)
            if (length[j] == i) { perm[pp] = j; pp++; };

    for (i = 0; i < BZ_MAX_CODE_LEN; i++) base[i] = 0;
    for (i = 0; i < alphaSize; i++) base[length[i]+1]++;

    for (i = 1; i < BZ_MAX_CODE_LEN; i++) base[i] += base[i-1];

    for (i = 0; i < BZ_MAX_CODE_LEN; i++) limit[i] = 0;
    vec = 0;

    for (i = minLen; i <= maxLen; i++) {
      vec += (base[i+1] - base[i]);
      limit[i] = vec-1;
      vec <<= 1;
   }
   for (i = minLen + 1; i <= maxLen; i++)
      base[i] = ((limit[i-1] + 1) << 1) - base[i];
}



void AppBzip2::BZ2_bsInitWrite(EState *s)
{
    s->bsLive = 0;
    s->bsBuff = 0;
}

void AppBzip2::bsFinishWrite(EState *s)
{
    while (s->bsLive > 0)
    {
        s->zbits[s->numZ] = (uint8_t)(s->bsBuff >> 24);
        s->numZ++;
        s->bsBuff <<= 8;
        s->bsLive -= 8;
    }
}

void AppBzip2::bsW(EState *s, int32_t n, uint32_t v)
{
    while (s->bsLive >= 8)
    {
        s->zbits[s->numZ] = (uint8_t)(s->bsBuff >> 24);          
        s->numZ++;
        s->bsBuff <<= 8;                        
        s->bsLive -= 8;
    }

    s->bsBuff |= (v << (32 - s->bsLive - n));
    s->bsLive += n;
}

void AppBzip2::bsPutUInt32(EState* s, uint32_t u)
{
    bsW(s, 8, (u >> 24) & 0xffL );
    bsW(s, 8, (u >> 16) & 0xffL );
    bsW(s, 8, (u >>  8) & 0xffL );
    bsW(s, 8,  u        & 0xffL );
}

void AppBzip2::bsPutUChar(EState* s, uint8_t c)
{
    bsW(s, 8, (uint32_t)c);
}

void AppBzip2::makeMaps_e(EState* s)
{
    int32_t i;
    s->nInUse = 0;

    for (i = 0; i < 256; i++)
    {
        if (s->inUse[i])
        {
            s->unseqToSeq[i] = s->nInUse;
            s->nInUse++;
        }
    }
}

void AppBzip2::generateMTFValues(EState *s)
{
    uint8_t yy[256];
    int32_t i, j;
    int32_t zPend;
    int32_t wr;
    int32_t EOB;
    uint32_t* ptr = s->ptr;
    uint8_t* block = s->block;
    uint16_t* mtfv = s->mtfv;
    makeMaps_e ( s );
    EOB = s->nInUse+1;
    for (i = 0; i <= EOB; i++) s->mtfFreq[i] = 0;
    wr = 0;
    zPend = 0;
    for (i = 0; i < s->nInUse; i++) yy[i] = (uint8_t)i;

    for (i = 0; i < s->nblock; i++) {
      uint8_t ll_i;
      j = ptr[i]-1; if (j < 0) j += s->nblock;
      ll_i = s->unseqToSeq[block[j]];

      if (yy[0] == ll_i) { 
         zPend++;
      } else {

         if (zPend > 0) {
            zPend--;
            while (1) {
               if (zPend & 1) {
                  mtfv[wr] = BZ_RUNB; wr++; 
                  s->mtfFreq[BZ_RUNB]++; 
               } else {
                  mtfv[wr] = BZ_RUNA; wr++; 
                  s->mtfFreq[BZ_RUNA]++; 
               }
               if (zPend < 2) break;
               zPend = (zPend - 2) / 2;
            };
            zPend = 0;
         }
         {
            register uint8_t rtmp;
            register uint8_t* ryy_j;
            register uint8_t  rll_i;
            rtmp  = yy[1];
            yy[1] = yy[0];
            ryy_j = &(yy[1]);
            rll_i = ll_i;
            while ( rll_i != rtmp ) {
               register uint8_t rtmp2;
               ryy_j++;
               rtmp2  = rtmp;
               rtmp   = *ryy_j;
               *ryy_j = rtmp2;
            };
            yy[0] = rtmp;
            j = ryy_j - &(yy[0]);
            mtfv[wr] = j+1; wr++; s->mtfFreq[j+1]++;
         }

      }
   }

   if (zPend > 0) {
      zPend--;
      while (1) {
         if (zPend & 1) {
            mtfv[wr] = BZ_RUNB; wr++; 
            s->mtfFreq[BZ_RUNB]++; 
         } else {
            mtfv[wr] = BZ_RUNA; wr++; 
            s->mtfFreq[BZ_RUNA]++; 
         }
         if (zPend < 2) break;
         zPend = (zPend - 2) / 2;
      };
      zPend = 0;
   }

   mtfv[wr] = EOB; wr++; s->mtfFreq[EOB]++;

   s->nMTF = wr;
}

void AppBzip2::sendMTFValues(EState* s)
{
    int32_t v, t, i, j, gs, ge, totc, bt, bc, iter;
    int32_t nSelectors, alphaSize, minLen, maxLen, selCtr;
    int32_t nGroups, nBytes;
    uint16_t cost[BZ_N_GROUPS];
    int32_t fave[BZ_N_GROUPS];
    uint16_t *mtfv = s->mtfv;

    if (s->verbosity >= 3)
    {
        fprintf(stderr, "      %d in block, %d after MTF & 1-2 coding, "
                "%d+2 syms in use\n", 
                s->nblock, s->nMTF, s->nInUse );
    }

    alphaSize = s->nInUse+2;

    for (t = 0; t < BZ_N_GROUPS; t++)
        for (v = 0; v < alphaSize; v++)
            s->len[t][v] = BZ_GREATER_ICOST;

    AssertH ( s->nMTF > 0, 3001 );
    if (s->nMTF < 200)  nGroups = 2; else
    if (s->nMTF < 600)  nGroups = 3; else
    if (s->nMTF < 1200) nGroups = 4; else
    if (s->nMTF < 2400) nGroups = 5; else
                       nGroups = 6;

    { 
      int32_t nPart, remF, tFreq, aFreq;

      nPart = nGroups;
      remF  = s->nMTF;
      gs = 0;
      while (nPart > 0) {
         tFreq = remF / nPart;
         ge = gs-1;
         aFreq = 0;
         while (aFreq < tFreq && ge < alphaSize-1) {
            ge++;
            aFreq += s->mtfFreq[ge];
         }

         if (ge > gs 
             && nPart != nGroups && nPart != 1 
             && ((nGroups-nPart) % 2 == 1)) {
            aFreq -= s->mtfFreq[ge];
            ge--;
         }

         if (s->verbosity >= 3)
            fprintf(stderr, "      initial group %d, [%d .. %d], "
                      "has %d syms (%4.1f%%)\n",
                      nPart, gs, ge, aFreq, 
                      (100.0 * (float)aFreq) / (float)(s->nMTF) );
 
         for (v = 0; v < alphaSize; v++)
            if (v >= gs && v <= ge) 
               s->len[nPart-1][v] = 0; else
               s->len[nPart-1][v] = BZ_GREATER_ICOST;
 
         nPart--;
         gs = ge+1;
         remF -= aFreq;
      }
   }

   for (iter = 0; iter < 4; iter++) {

      for (t = 0; t < nGroups; t++) fave[t] = 0;

      for (t = 0; t < nGroups; t++)
         for (v = 0; v < alphaSize; v++)
            s->rfreq[t][v] = 0;

      if (nGroups == 6) {
         for (v = 0; v < alphaSize; v++) {
            s->len_pack[v][0] = (s->len[1][v] << 16) | s->len[0][v];
            s->len_pack[v][1] = (s->len[3][v] << 16) | s->len[2][v];
            s->len_pack[v][2] = (s->len[5][v] << 16) | s->len[4][v];
	 }
      }

      nSelectors = 0;
      totc = 0;
      gs = 0;
      while (1) {

         if (gs >= s->nMTF) break;
         ge = gs + 50 - 1; 
         if (ge >= s->nMTF) ge = s->nMTF-1;

         for (t = 0; t < nGroups; t++) cost[t] = 0;

         if (nGroups == 6 && 50 == ge-gs+1) {
            register uint32_t cost01, cost23, cost45;
            register uint16_t icv;
            cost01 = cost23 = cost45 = 0;


        for (uint8_t i = 0; i <= 49; i++)
        {
               icv = mtfv[gs+(i)];           
               cost01 += s->len_pack[icv][0]; 
               cost23 += s->len_pack[icv][1]; 
               cost45 += s->len_pack[icv][2]; 

        }

            cost[0] = cost01 & 0xffff; cost[1] = cost01 >> 16;
            cost[2] = cost23 & 0xffff; cost[3] = cost23 >> 16;
            cost[4] = cost45 & 0xffff; cost[5] = cost45 >> 16;

         } else {
            for (i = gs; i <= ge; i++) { 
               uint16_t icv = mtfv[i];
               for (t = 0; t < nGroups; t++) cost[t] += s->len[t][icv];
            }
         }
 
         bc = 999999999; bt = -1;
         for (t = 0; t < nGroups; t++)
            if (cost[t] < bc) { bc = cost[t]; bt = t; };
         totc += bc;
         fave[bt]++;
         s->selector[nSelectors] = bt;
         nSelectors++;

         if (nGroups == 6 && 50 == ge-gs+1)
         {

            for (uint8_t i = 0; i <= 49; i++)
                s->rfreq[bt][mtfv[gs+i]]++;

         } else {
            for (i = gs; i <= ge; i++)
               s->rfreq[bt][ mtfv[i] ]++;
         }

         gs = ge+1;
      }
      if (s->verbosity >= 3) {
         fprintf(stderr, "      pass %d: size is %d, grp uses are ", 
                   iter+1, totc/8 );
         for (t = 0; t < nGroups; t++)
            fprintf(stderr, "%d ", fave[t] );
         VPrintf0 ( "\n" );
      }

      for (t = 0; t < nGroups; t++)
         BZ2_hbMakeCodeLengths(&(s->len[t][0]), &(s->rfreq[t][0]), alphaSize, 20);
    }

    AssertH(nGroups < 8, 3002);
    AssertH(nSelectors < 32768 && nSelectors <= (2 + (900000 / 50)), 3003);

   {
      uint8_t pos[BZ_N_GROUPS], ll_i, tmp2, tmp;
      for (i = 0; i < nGroups; i++) pos[i] = i;
      for (i = 0; i < nSelectors; i++) {
         ll_i = s->selector[i];
         j = 0;
         tmp = pos[j];
         while ( ll_i != tmp ) {
            j++;
            tmp2 = tmp;
            tmp = pos[j];
            pos[j] = tmp2;
         };
         pos[0] = tmp;
         s->selectorMtf[i] = j;
      }
   };

   for (t = 0; t < nGroups; t++) {
      minLen = 32;
      maxLen = 0;
      for (i = 0; i < alphaSize; i++) {
         if (s->len[t][i] > maxLen) maxLen = s->len[t][i];
         if (s->len[t][i] < minLen) minLen = s->len[t][i];
      }
      AssertH ( !(maxLen > 20), 3004 );
      AssertH ( !(minLen < 1),  3005 );
      BZ2_hbAssignCodes ( &(s->code[t][0]), &(s->len[t][0]), 
                          minLen, maxLen, alphaSize );
   }

   { 
      uint8_t inUse16[16];
      for (i = 0; i < 16; i++) {
          inUse16[i] = 0;
          for (j = 0; j < 16; j++)
             if (s->inUse[i * 16 + j]) inUse16[i] = 1;
      }
     
      nBytes = s->numZ;
      for (i = 0; i < 16; i++)
         if (inUse16[i]) bsW(s,1,1); else bsW(s,1,0);

      for (i = 0; i < 16; i++)
         if (inUse16[i])
            for (j = 0; j < 16; j++) {
               if (s->inUse[i * 16 + j]) bsW(s,1,1); else bsW(s,1,0);
            }

      if (s->verbosity >= 3) 
         fprintf(stderr, "      bytes: mapping %d, ", s->numZ-nBytes );
   }

   nBytes = s->numZ;
   bsW ( s, 3, nGroups );
   bsW ( s, 15, nSelectors );
   for (i = 0; i < nSelectors; i++) { 
      for (j = 0; j < s->selectorMtf[i]; j++) bsW(s,1,1);
      bsW(s,1,0);
   }
   if (s->verbosity >= 3)
      fprintf(stderr, "selectors %d, ", s->numZ-nBytes );

   nBytes = s->numZ;

   for (t = 0; t < nGroups; t++) {
      int32_t curr = s->len[t][0];
      bsW ( s, 5, curr );
      for (i = 0; i < alphaSize; i++) {
         while (curr < s->len[t][i]) { bsW(s,2,2); curr++;  };
         while (curr > s->len[t][i]) { bsW(s,2,3); curr--;  };
         bsW ( s, 1, 0 );
      }
   }

   if (s->verbosity >= 3)
      fprintf(stderr, "code lengths %d, ", s->numZ-nBytes );

   nBytes = s->numZ;
   selCtr = 0;
   gs = 0;
   while (1) {
      if (gs >= s->nMTF) break;
      ge = gs + 50 - 1; 
      if (ge >= s->nMTF) ge = s->nMTF-1;
      AssertH ( s->selector[selCtr] < nGroups, 3006 );

      if (nGroups == 6 && 50 == ge-gs+1) {
            uint16_t mtfv_i;
            uint8_t* s_len_sel_selCtr 
               = &(s->len[s->selector[selCtr]][0]);
            int32_t* s_code_sel_selCtr
               = &(s->code[s->selector[selCtr]][0]);

        for (uint8_t i = 0; i <= 49; i++)
        {
            mtfv_i = mtfv[gs+(i)];
            bsW ( s, s_len_sel_selCtr[mtfv_i], s_code_sel_selCtr[mtfv_i] );

        }

      } else {
         for (i = gs; i <= ge; i++) {
            bsW ( s, 
                  s->len  [s->selector[selCtr]] [mtfv[i]],
                  s->code [s->selector[selCtr]] [mtfv[i]] );
         }
      }


      gs = ge+1;
      selCtr++;
   }
   AssertH( selCtr == nSelectors, 3007 );

   if (s->verbosity >= 3)
      fprintf(stderr, "codes %d\n", s->numZ-nBytes );
}

void AppBzip2::BZ2_compressBlock(EState* s, uint8_t is_last_block)
{
    if (s->nblock > 0)
    {

        s->blockCRC = ~(s->blockCRC);
        s->combinedCRC = (s->combinedCRC << 1) | (s->combinedCRC >> 31);
        s->combinedCRC ^= s->blockCRC;
        if (s->blockNo > 1) s->numZ = 0;

        if (s->verbosity >= 2)
        {
            fprintf(stderr, "    block %d: crc = 0x%8x, "
                   "combined CRC = 0x%8x, size = %d\n",
                   s->blockNo, s->blockCRC, s->combinedCRC, s->nblock );
        }

        BZ2_blockSort(s);
    }

    s->zbits = (uint8_t*) (&((uint8_t*)s->arr2)[s->nblock]);

    if (s->blockNo == 1)
    {
        BZ2_bsInitWrite ( s );
        bsPutUChar(s, BZ_HDR_B);
        bsPutUChar(s, BZ_HDR_Z);
        bsPutUChar(s, BZ_HDR_h);
        bsPutUChar(s, (uint8_t)(BZ_HDR_0 + s->blockSize100k));
    }

   if (s->nblock > 0) {

      bsPutUChar ( s, 0x31 ); bsPutUChar ( s, 0x41 );
      bsPutUChar ( s, 0x59 ); bsPutUChar ( s, 0x26 );
      bsPutUChar ( s, 0x53 ); bsPutUChar ( s, 0x59 );
      bsPutUInt32 ( s, s->blockCRC );
      bsW(s,1,0);
      bsW ( s, 24, s->origPtr );
      generateMTFValues ( s );
      sendMTFValues ( s );
   }

   if (is_last_block) {

      bsPutUChar ( s, 0x17 ); bsPutUChar ( s, 0x72 );
      bsPutUChar ( s, 0x45 ); bsPutUChar ( s, 0x38 );
      bsPutUChar ( s, 0x50 ); bsPutUChar ( s, 0x90 );
      bsPutUInt32 ( s, s->combinedCRC );
      if (s->verbosity >= 2)
         fprintf(stderr, "    final combined CRC = 0x%x\n   ", s->combinedCRC );
      bsFinishWrite ( s );
   }
}

void AppBzip2::makeMaps_d(DState *s)
{
    int32_t i;
    s->nInUse = 0;

    for (i = 0; i < 256; i++)
      if (s->inUse[i]) {
         s->seqToUnseq[s->nInUse] = i;
         s->nInUse++;
      }
}

void AppBzip2::BZ_INITIALISE_CRC(uint32_t &crcVar) { crcVar = 0xffffffff; }

int32_t AppBzip2::BZ2_indexIntoF(int32_t indx, int32_t *cftab)
{
    int32_t nb, na, mid;
    nb = 0;
    na = 256;
    do
    {
        mid = (nb + na) >> 1;

        if (indx >= cftab[mid])
            nb = mid;
        else
            na = mid;
    }
    while (na - nb != 1);
    return nb;
}

int32_t AppBzip2::BZ2_decompress(DState *s)
{
    uint8_t uc;
    int32_t retVal;
    int32_t minLen, maxLen;
    bz_stream* strm = s->strm;
    int32_t i;
    int32_t j;
    int32_t t;
    int32_t alphaSize;
    int32_t nGroups;
    int32_t nSelectors;
    int32_t EOB;
    int32_t groupNo;
    int32_t groupPos;
    int32_t nextSym;
    int32_t nblockMAX;
    int32_t nblock;
    int32_t es;
    int32_t N;
    int32_t curr;
    int32_t zt;
    int32_t zn; 
    int32_t zvec;
    int32_t zj;
    int32_t gSel;
    int32_t gMinlen;
    int32_t *gLimit;
    int32_t *gBase;
    int32_t *gPerm;

    if (s->state == BZ_X_MAGIC_1)
    {
        s->save_i           = 0;
        s->save_j           = 0;
        s->save_t           = 0;
        s->save_alphaSize   = 0;
        s->save_nGroups     = 0;
        s->save_nSelectors  = 0;
        s->save_EOB         = 0;
        s->save_groupNo     = 0;
        s->save_groupPos    = 0;
        s->save_nextSym     = 0;
        s->save_nblockMAX   = 0;
        s->save_nblock      = 0;
        s->save_es          = 0;
        s->save_N           = 0;
        s->save_curr        = 0;
        s->save_zt          = 0;
        s->save_zn          = 0;
        s->save_zvec        = 0;
        s->save_zj          = 0;
        s->save_gSel        = 0;
        s->save_gMinlen     = 0;
        s->save_gLimit      = NULL;
        s->save_gBase       = NULL;
        s->save_gPerm       = NULL;
    }

    i           = s->save_i;
    j           = s->save_j;
    t           = s->save_t;
    alphaSize   = s->save_alphaSize;
    nGroups     = s->save_nGroups;
    nSelectors  = s->save_nSelectors;
    EOB         = s->save_EOB;
    groupNo     = s->save_groupNo;
    groupPos    = s->save_groupPos;
    nextSym     = s->save_nextSym;
    nblockMAX   = s->save_nblockMAX;
    nblock      = s->save_nblock;
    es          = s->save_es;
    N           = s->save_N;
    curr        = s->save_curr;
    zt          = s->save_zt;
    zn          = s->save_zn; 
    zvec        = s->save_zvec;
    zj          = s->save_zj;
    gSel        = s->save_gSel;
    gMinlen     = s->save_gMinlen;
    gLimit      = s->save_gLimit;
    gBase       = s->save_gBase;
    gPerm       = s->save_gPerm;
    retVal = BZ_OK;

    switch (s->state)
    {
    case BZ_X_MAGIC_1: s->state = BZ_X_MAGIC_1; 
        while (1)
        {
            if (s->bsLive >= 8)
            {
                uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }


        if (uc != BZ_HDR_B) { retVal = BZ_DATA_ERROR_MAGIC; goto save_state_and_return; };

   case BZ_X_MAGIC_2: s->state = BZ_X_MAGIC_2; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != BZ_HDR_Z) { retVal = BZ_DATA_ERROR_MAGIC; goto save_state_and_return; };

   case BZ_X_MAGIC_3: s->state = BZ_X_MAGIC_3; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != BZ_HDR_h) { retVal = BZ_DATA_ERROR_MAGIC; goto save_state_and_return; };

   case BZ_X_MAGIC_4: s->state = BZ_X_MAGIC_4; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         s->blockSize100k = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (s->blockSize100k < (BZ_HDR_0 + 1) || s->blockSize100k > (BZ_HDR_0 + 9))
        { retVal = BZ_DATA_ERROR_MAGIC; goto save_state_and_return; };
        s->blockSize100k -= BZ_HDR_0;




        if (s->smallDecompress)
        {

            s->ll16 = new uint16_t[s->blockSize100k*100000];
            s->ll4 = new uint8_t[(1 + s->blockSize100k*100000) >> 1];

            if (s->ll16 == NULL || s->ll4 == NULL)
            {
                retVal = BZ_MEM_ERROR;
                goto save_state_and_return;
            };
        }
        else
        {
            s->tt = new uint32_t[s->blockSize100k*100000*sizeof(int32_t)];
            if (s->tt == NULL) { retVal = BZ_MEM_ERROR; goto save_state_and_return; };
        }




   case BZ_X_BLKHDR_1: s->state = BZ_X_BLKHDR_1;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc == 0x17) goto endhdr_2;
        if (uc != 0x31) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_BLKHDR_2: s->state = BZ_X_BLKHDR_2;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != 0x41) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_BLKHDR_3: s->state = BZ_X_BLKHDR_3;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != 0x59) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_BLKHDR_4: s->state = BZ_X_BLKHDR_4;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != 0x26) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_BLKHDR_5: s->state = BZ_X_BLKHDR_5;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != 0x53) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_BLKHDR_6: s->state = BZ_X_BLKHDR_6;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        if (uc != 0x59) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };
        s->currBlockNo++;
        if (s->verbosity >= 2) fprintf(stderr, "\n    [%d: huff+mtf ", s->currBlockNo );
        s->storedBlockCRC = 0;

   case BZ_X_BCRC_1: s->state = BZ_X_BCRC_1;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->storedBlockCRC = (s->storedBlockCRC << 8) | ((uint32_t)uc);

   case BZ_X_BCRC_2: s->state = BZ_X_BCRC_2;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->storedBlockCRC = (s->storedBlockCRC << 8) | ((uint32_t)uc);

   case BZ_X_BCRC_3: s->state = BZ_X_BCRC_3;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->storedBlockCRC = (s->storedBlockCRC << 8) | ((uint32_t)uc);

   case BZ_X_BCRC_4: s->state = BZ_X_BCRC_4;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }



        s->storedBlockCRC = (s->storedBlockCRC << 8) | ((uint32_t)uc);

   case BZ_X_RANDBIT: s->state = BZ_X_RANDBIT;
   while (1) {
      if (s->bsLive >= 1) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1);
         s->bsLive -= 1;
         s->blockRandomised = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->origPtr = 0;

   case BZ_X_ORIGPTR_1: s->state = BZ_X_ORIGPTR_1;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->origPtr = (s->origPtr << 8) | ((int32_t)uc);

   case BZ_X_ORIGPTR_2: s->state = BZ_X_ORIGPTR_2;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->origPtr = (s->origPtr << 8) | ((int32_t)uc);

   case BZ_X_ORIGPTR_3: s->state = BZ_X_ORIGPTR_3;
   while (1) {
      if (s->bsLive >= 8) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1);
         s->bsLive -= 8;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

        s->origPtr = (s->origPtr << 8) | ((int32_t)uc);

        if (s->origPtr < 0) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

        if (s->origPtr > 10 + 100000*s->blockSize100k)
        { retVal = BZ_DATA_ERROR; goto save_state_and_return; };



      for (i = 0; i < 16; i++)
      {
   case BZ_X_MAPPING_1: s->state = BZ_X_MAPPING_1;
   while (1) {
      if (s->bsLive >= 1) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1);
         s->bsLive -= 1;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }


         if (uc == 1) 
            s->inUse16[i] = 1; else 
            s->inUse16[i] = 0;
      }

      for (i = 0; i < 256; i++) s->inUse[i] = 0;



      for (i = 0; i < 16; i++)
         if (s->inUse16[i])
            for (j = 0; j < 16; j++)
            {
   case BZ_X_MAPPING_2: s->state = BZ_X_MAPPING_2;
   while (1) {
      if (s->bsLive >= 1) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1);
         s->bsLive -= 1;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

               if (uc == 1) s->inUse[i * 16 + j] = 1;
            }
      makeMaps_d ( s );
      if (s->nInUse == 0) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };
      alphaSize = s->nInUse+2;



   case BZ_X_SELECTOR_1: s->state = BZ_X_SELECTOR_1;
   while (1) {
      if (s->bsLive >= 3) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-3)) & ((1 << 3)-1);
         s->bsLive -= 3;
         nGroups = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (nGroups < 2 || nGroups > 6) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_SELECTOR_2: s->state = BZ_X_SELECTOR_2;
   while (1) {
      if (s->bsLive >= 15) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-15)) & ((1 << 15)-1);
         s->bsLive -= 15;
         nSelectors = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (nSelectors < 1) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };
      for (i = 0; i < nSelectors; i++) {
         j = 0;
         while (1) {

   case BZ_X_SELECTOR_3: s->state = BZ_X_SELECTOR_3;
   while (1) {
      if (s->bsLive >= 1) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1);
         s->bsLive -= 1;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

            if (uc == 0) break;
            j++;
            if (j >= nGroups) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };
         }
         s->selectorMtf[i] = j;
      }

      {
         uint8_t pos[BZ_N_GROUPS], tmp, v;
         for (v = 0; v < nGroups; v++) pos[v] = v;
   
         for (i = 0; i < nSelectors; i++) {
            v = s->selectorMtf[i];
            tmp = pos[v];
            while (v > 0) { pos[v] = pos[v-1]; v--; }
            pos[0] = tmp;
            s->selector[i] = tmp;
         }
      }



      for (t = 0; t < nGroups; t++) {

   case BZ_X_CODING_1: s->state = BZ_X_CODING_1;
   while (1) {
      if (s->bsLive >= 5) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-5)) & ((1 << 5)-1);
         s->bsLive -= 5;
         curr = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

         for (i = 0; i < alphaSize; i++) {
            while (1) {
               if (curr < 1 || curr > 20) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_CODING_2: s->state = BZ_X_CODING_2;
   while (1) {
      if (s->bsLive >= 1) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1);
         s->bsLive -= 1;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

               if (uc == 0) break;

   case BZ_X_CODING_3: s->state = BZ_X_CODING_3;
   while (1) {
      if (s->bsLive >= 1) {
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1);
         s->bsLive -= 1;
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

               if (uc == 0) curr++; else curr--;
            }
            s->len[t][i] = curr;
         }
      }

      for (t = 0; t < nGroups; t++) {
         minLen = 32;
         maxLen = 0;
         for (i = 0; i < alphaSize; i++) {
            if (s->len[t][i] > maxLen) maxLen = s->len[t][i];
            if (s->len[t][i] < minLen) minLen = s->len[t][i];
         }
         BZ2_hbCreateDecodeTables ( 
            &(s->limit[t][0]), 
            &(s->base[t][0]), 
            &(s->perm[t][0]), 
            &(s->len[t][0]),
            minLen, maxLen, alphaSize
         );
         s->minLens[t] = minLen;
      }

      EOB      = s->nInUse+1;
      nblockMAX = 100000 * s->blockSize100k;
      groupNo  = -1;
      groupPos = 0;

      for (i = 0; i <= 255; i++) s->unzftab[i] = 0;

      {
         int32_t ii, jj, kk;
         kk = MTFA_SIZE-1;
         for (ii = 256 / MTFL_SIZE - 1; ii >= 0; ii--) {
            for (jj = MTFL_SIZE-1; jj >= 0; jj--) {
               s->mtfa[kk] = (uint8_t)(ii * MTFL_SIZE + jj);
               kk--;
            }
            s->mtfbase[ii] = kk + 1;
         }
      }

      nblock = 0;

{ 
   if (groupPos == 0) { 
      groupNo++; 
      if (groupNo >= nSelectors) { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
      groupPos = 50; 
      gSel = s->selector[groupNo]; 
      gMinlen = s->minLens[gSel]; 
      gLimit = &(s->limit[gSel][0]); 
      gPerm = &(s->perm[gSel][0]); 
      gBase = &(s->base[gSel][0]); } 
   groupPos--; 
   zn = gMinlen;




   case BZ_X_MTF_1: s->state = BZ_X_MTF_1; 
   while (1) { 
      if (s->bsLive >= zn) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-zn)) & ((1 << zn)-1); 
         s->bsLive -= zn; 
         zvec = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

   while (1) { 
      if (zn > 20 ) { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
      if (zvec <= gLimit[zn]) break; 
      zn++;

   case BZ_X_MTF_2: s->state = BZ_X_MTF_2; 
   while (1) { 
      if (s->bsLive >= 1) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1); 
         s->bsLive -= 1; 
         zj = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      zvec = (zvec << 1) | zj; }; 
   if (zvec - gBase[zn] < 0 || zvec - gBase[zn] >= BZ_MAX_ALPHA_SIZE) 
      { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
   nextSym = gPerm[zvec - gBase[zn]]; }

      while (1) {

         if (nextSym == EOB) break;

         if (nextSym == BZ_RUNA || nextSym == BZ_RUNB) {

            es = -1;
            N = 1;
            do {
               if (nextSym == BZ_RUNA) es = es + (0+1) * N; else
               if (nextSym == BZ_RUNB) es = es + (1+1) * N;
               N = N * 2;
{
   if (groupPos == 0) { 
      groupNo++; 
      if (groupNo >= nSelectors) { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
      groupPos = 50; 
      gSel = s->selector[groupNo]; 
      gMinlen = s->minLens[gSel]; 
      gLimit = &(s->limit[gSel][0]); 
      gPerm = &(s->perm[gSel][0]); 
      gBase = &(s->base[gSel][0]); } 
   groupPos--; 
   zn = gMinlen; 

   case BZ_X_MTF_3: s->state = BZ_X_MTF_3; 
   while (1) { 
      if (s->bsLive >= zn) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-zn)) & ((1 << zn)-1); 
         s->bsLive -= zn; 
         zvec = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

   while (1) { 
      if (zn > 20 ) { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
      if (zvec <= gLimit[zn]) break; 
      zn++; 

   case BZ_X_MTF_4: s->state = BZ_X_MTF_4; 
   while (1) { 
      if (s->bsLive >= 1) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1); 
         s->bsLive -= 1; 
         zj = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      zvec = (zvec << 1) | zj; }; 
   if (zvec - gBase[zn] < 0 || zvec - gBase[zn] >= BZ_MAX_ALPHA_SIZE) 
      { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
   nextSym = gPerm[zvec - gBase[zn]]; }


            }
               while (nextSym == BZ_RUNA || nextSym == BZ_RUNB);

            es++;
            uc = s->seqToUnseq[ s->mtfa[s->mtfbase[0]] ];
            s->unzftab[uc] += es;

            if (s->smallDecompress)
               while (es > 0) {
                  if (nblock >= nblockMAX) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };
                  s->ll16[nblock] = (uint16_t)uc;
                  nblock++;
                  es--;
               }
            else
               while (es > 0) {
                  if (nblock >= nblockMAX) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };
                  s->tt[nblock] = (uint32_t)uc;
                  nblock++;
                  es--;
               };

            continue;

         } else {

            if (nblock >= nblockMAX) { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

            {
               int32_t ii, jj, kk, pp, lno, off;
               uint32_t nn;
               nn = (uint32_t)(nextSym - 1);

               if (nn < MTFL_SIZE) {
                  pp = s->mtfbase[0];
                  uc = s->mtfa[pp+nn];
                  while (nn > 3) {
                     int32_t z = pp+nn;
                     s->mtfa[(z)  ] = s->mtfa[(z)-1];
                     s->mtfa[(z)-1] = s->mtfa[(z)-2];
                     s->mtfa[(z)-2] = s->mtfa[(z)-3];
                     s->mtfa[(z)-3] = s->mtfa[(z)-4];
                     nn -= 4;
                  }
                  while (nn > 0) { 
                     s->mtfa[(pp+nn)] = s->mtfa[(pp+nn)-1]; nn--; 
                  };
                  s->mtfa[pp] = uc;
               } else { 
                  lno = nn / MTFL_SIZE;
                  off = nn % MTFL_SIZE;
                  pp = s->mtfbase[lno] + off;
                  uc = s->mtfa[pp];
                  while (pp > s->mtfbase[lno]) { 
                     s->mtfa[pp] = s->mtfa[pp-1]; pp--; 
                  };
                  s->mtfbase[lno]++;
                  while (lno > 0) {
                     s->mtfbase[lno]--;
                     s->mtfa[s->mtfbase[lno]] 
                        = s->mtfa[s->mtfbase[lno-1] + MTFL_SIZE - 1];
                     lno--;
                  }
                  s->mtfbase[0]--;
                  s->mtfa[s->mtfbase[0]] = uc;
                  if (s->mtfbase[0] == 0) {
                     kk = MTFA_SIZE-1;
                     for (ii = 256 / MTFL_SIZE-1; ii >= 0; ii--) {
                        for (jj = MTFL_SIZE-1; jj >= 0; jj--) {
                           s->mtfa[kk] = s->mtfa[s->mtfbase[ii] + jj];
                           kk--;
                        }
                        s->mtfbase[ii] = kk + 1;
                     }
                  }
               }
            }

            s->unzftab[s->seqToUnseq[uc]]++;
            if (s->smallDecompress)
               s->ll16[nblock] = (uint16_t)(s->seqToUnseq[uc]); else
               s->tt[nblock]   = (uint32_t)(s->seqToUnseq[uc]);
            nblock++;

{
   if (groupPos == 0) { 
      groupNo++; 
      if (groupNo >= nSelectors) { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
      groupPos = 50; 
      gSel = s->selector[groupNo]; 
      gMinlen = s->minLens[gSel]; 
      gLimit = &(s->limit[gSel][0]); 
      gPerm = &(s->perm[gSel][0]); 
      gBase = &(s->base[gSel][0]); } 
   groupPos--; 
   zn = gMinlen; 

   case BZ_X_MTF_5: s->state = BZ_X_MTF_5;
   while (1) { 
      if (s->bsLive >= zn) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-zn)) & ((1 << zn)-1); 
         s->bsLive -= zn; 
         zvec = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

   while (1) { 
      if (zn > 20 ) { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
      if (zvec <= gLimit[zn]) break; 
      zn++; 

   case BZ_X_MTF_6: s->state = BZ_X_MTF_6; 
   while (1) { 
      if (s->bsLive >= 1) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-1)) & ((1 << 1)-1); 
         s->bsLive -= 1; 
         zj = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      zvec = (zvec << 1) | zj; }; 
   if (zvec - gBase[zn] < 0 || zvec - gBase[zn] >= BZ_MAX_ALPHA_SIZE) 
      { retVal = BZ_DATA_ERROR; goto save_state_and_return; }; 
   nextSym = gPerm[zvec - gBase[zn]]; }


            continue;
         }
      }

      if (s->origPtr < 0 || s->origPtr >= nblock)
    { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

      s->state_out_len = 0;
      s->state_out_ch  = 0;
      BZ_INITIALISE_CRC ( s->calculatedBlockCRC );
      s->state = BZ_X_OUTPUT;
      if (s->verbosity >= 2) VPrintf0 ( "rt+rld" );

      s->cftab[0] = 0;
      for (i = 1; i <= 256; i++) s->cftab[i] = s->unzftab[i-1];
      for (i = 1; i <= 256; i++) s->cftab[i] += s->cftab[i-1];



      if (s->smallDecompress)
      {
         for (i = 0; i <= 256; i++) s->cftabCopy[i] = s->cftab[i];
         for (i = 0; i < nblock; i++)
        {
            uc = (uint8_t)(s->ll16[i]);
            s->ll16[i] = (uint16_t)(s->cftabCopy[uc] & 0x0000ffff);

{ if (((i) & 0x1) == 0) 
        s->ll4[(i) >> 1] = (s->ll4[(i) >> 1] & 0xf0) | (s->cftabCopy[uc] >> 16); else 
        s->ll4[(i) >> 1] = (s->ll4[(i) >> 1] & 0x0f) | ((s->cftabCopy[uc] >> 16) << 4); }

            s->cftabCopy[uc]++;
         }



         i = s->origPtr;
        j = (((uint32_t)s->ll16[i]) |
            (((((uint32_t)(s->ll4[(i) >> 1])) >> (((i) << 2) & 0x4)) & 0xF) << 16));

         do {
            int32_t tmp = (((uint32_t)s->ll16[j]) |
                (((((uint32_t)(s->ll4[(j) >> 1])) >> (((j) << 2) & 0x4)) & 0xF) << 16));
            {
                s->ll16[j] = (uint16_t)(i & 0x0000ffff);

{ if (((j) & 0x1) == 0) \
        s->ll4[(j) >> 1] = (s->ll4[(j) >> 1] & 0xf0) | (i); else \
        s->ll4[(j) >> 1] = (s->ll4[(j) >> 1] & 0x0f) | ((i) << 4); }

            }
            i = j;
            j = tmp;
         }
            while (i != s->origPtr);






         s->tPos = s->origPtr;
         s->nblock_used = 0;
         if (s->blockRandomised)
         {
            s->rNToGo = 0;
            s->rTPos  = 0;
            s->k0 = BZ2_indexIntoF(s->tPos, s->cftab);
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
    ( ((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));

            s->nblock_used++;

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            s->k0 ^= ((s->rNToGo == 1) ? 1 : 0);
         } else {
            s->k0 = BZ2_indexIntoF(s->tPos, s->cftab);
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
        (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF)  << 16));
            s->nblock_used++;
         }

      } else {
         for (i = 0; i < nblock; i++) {
            uc = (uint8_t)(s->tt[i] & 0xff);
            s->tt[s->cftab[uc]] |= (i << 8);
            s->cftab[uc]++;
         }

         s->tPos = s->tt[s->origPtr] >> 8;
         s->nblock_used = 0;
         if (s->blockRandomised) {
            s->rNToGo = 0;
            s->rTPos  = 0;



            s->tPos = s->tt[s->tPos];
            s->k0 = (uint8_t)(s->tPos & 0xff);
            s->tPos >>= 8;

            s->nblock_used++;

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            s->k0 ^= ((s->rNToGo == 1) ? 1 : 0);
         } else {
            s->tPos = s->tt[s->tPos];
            s->k0 = (uint8_t)(s->tPos & 0xff);
            s->tPos >>= 8;

            s->nblock_used++;
         }

      }
    { retVal = BZ_OK; goto save_state_and_return; };



    endhdr_2:



   case BZ_X_ENDHDR_2: s->state = BZ_X_ENDHDR_2; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (uc != 0x72)     { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_ENDHDR_3: s->state = BZ_X_ENDHDR_3;
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (uc != 0x45)     { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_ENDHDR_4: s->state = BZ_X_ENDHDR_4; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (uc != 0x38)     { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_ENDHDR_5: s->state = BZ_X_ENDHDR_5; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (uc != 0x50)     { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

   case BZ_X_ENDHDR_6: s->state = BZ_X_ENDHDR_6; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      if (uc != 0x90)     { retVal = BZ_DATA_ERROR; goto save_state_and_return; };

      s->storedCombinedCRC = 0;

   case BZ_X_CCRC_1: s->state = BZ_X_CCRC_1; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      s->storedCombinedCRC = (s->storedCombinedCRC << 8) | ((uint32_t)uc);

   case BZ_X_CCRC_2: s->state = BZ_X_CCRC_2; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      s->storedCombinedCRC = (s->storedCombinedCRC << 8) | ((uint32_t)uc);

   case BZ_X_CCRC_3: s->state = BZ_X_CCRC_3; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      s->storedCombinedCRC = (s->storedCombinedCRC << 8) | ((uint32_t)uc);

   case BZ_X_CCRC_4: s->state = BZ_X_CCRC_4; 
   while (1) { 
      if (s->bsLive >= 8) { 
         uint32_t v; 
         v = (s->bsBuff >> (s->bsLive-8)) & ((1 << 8)-1); 
         s->bsLive -= 8; 
         uc = v; 
         break; } 
      if (s->strm->avail_in == 0) { retVal = 0; goto save_state_and_return; }; 
      s->bsBuff = (s->bsBuff << 8) | ((uint32_t)(*((uint8_t*)(s->strm->next_in)))); 
      s->bsLive += 8; 
      s->strm->next_in++; 
      s->strm->avail_in--; 
      s->strm->total_in_lo32++; 
      if (s->strm->total_in_lo32 == 0) 
         s->strm->total_in_hi32++; }

      s->storedCombinedCRC = (s->storedCombinedCRC << 8) | ((uint32_t)uc);

      s->state = BZ_X_IDLE;
    { retVal = BZ_STREAM_END; goto save_state_and_return; };

      default: AssertH ( 0, 4001 );
   }

   AssertH(0, 4002);

   save_state_and_return:

   s->save_i           = i;
   s->save_j           = j;
   s->save_t           = t;
   s->save_alphaSize   = alphaSize;
   s->save_nGroups     = nGroups;
   s->save_nSelectors  = nSelectors;
   s->save_EOB         = EOB;
   s->save_groupNo     = groupNo;
   s->save_groupPos    = groupPos;
   s->save_nextSym     = nextSym;
   s->save_nblockMAX   = nblockMAX;
   s->save_nblock      = nblock;
   s->save_es          = es;
   s->save_N           = N;
   s->save_curr        = curr;
   s->save_zt          = zt;
   s->save_zn          = zn;
   s->save_zvec        = zvec;
   s->save_zj          = zj;
   s->save_gSel        = gSel;
   s->save_gMinlen     = gMinlen;
   s->save_gLimit      = gLimit;
   s->save_gBase       = gBase;
   s->save_gPerm       = gPerm;
   return retVal;   
}

int AppBzip2::bz_config_ok()
{
    if (sizeof(int)   != 4) return 0;
    if (sizeof(short) != 2) return 0;
    if (sizeof(char)  != 1) return 0;
    return 1;
}

void AppBzip2::prepare_new_block(EState *s)
{
    int32_t i;
    s->nblock = 0;
    s->numZ = 0;
    s->state_out_pos = 0;
    BZ_INITIALISE_CRC ( s->blockCRC );
    for (i = 0; i < 256; i++) s->inUse[i] = 0;
    s->blockNo++;
}

void AppBzip2::init_RL(EState* s)
{
   s->state_in_ch  = 256;
   s->state_in_len = 0;
}

uint8_t AppBzip2::isempty_RL(EState* s)
{
    if (s->state_in_ch < 256 && s->state_in_len > 0)
        return 0;
    else
        return 1;
}

int AppBzip2::BZ2_bzCompressInit(bz_stream *strm, int blockSize100k, int verbosity, int workFactor)
{
    int32_t n;
    EState *s;
    if (!bz_config_ok()) return BZ_CONFIG_ERROR;

    if (strm == NULL || blockSize100k < 1 || blockSize100k > 9 ||
            workFactor < 0 || workFactor > 250)
    {
        return BZ_PARAM_ERROR;
    }

    if (workFactor == 0) workFactor = 30;
    //if (strm->bzfree == NULL) strm->bzfree = default_bzfree;
    s = new EState[sizeof(EState)];
    if (s == NULL) return BZ_MEM_ERROR;
    s->strm = strm;
    s->arr1 = NULL;
    s->arr2 = NULL;
    s->ftab = NULL;
    n = 100000 * blockSize100k;
    s->arr1 = new unsigned[n * sizeof(uint32_t)];
    s->arr2 = new unsigned[n + BZ_N_OVERSHOOT * sizeof(uint32_t)];
    s->ftab = new unsigned[65537 * sizeof(uint32_t)];

    if (s->arr1 == NULL || s->arr2 == NULL || s->ftab == NULL)
    {
#if 0
        if (s->arr1 != NULL) (strm->bzfree)(strm->opaque,(s->arr1));
        if (s->arr2 != NULL) (strm->bzfree)(strm->opaque,(s->arr2));
        if (s->ftab != NULL) (strm->bzfree)(strm->opaque,(s->ftab));
        if (s       != NULL) (strm->bzfree)(strm->opaque,(s));
#else
        if (s->arr1 != NULL) delete[] s->arr1;
        if (s->arr2 != NULL) delete[] s->arr2;
        if (s->ftab != NULL) delete[] s->ftab;
        if (s       != NULL) delete[] s;
#endif
        return BZ_MEM_ERROR;
    }

    s->blockNo = 0;
    s->state = BZ_S_INPUT;
    s->mode = BZ_M_RUNNING;
    s->combinedCRC = 0;
    s->blockSize100k = blockSize100k;
    s->nblockMAX = 100000 * blockSize100k - 19;
    s->verbosity = verbosity;
    s->workFactor = workFactor;
    s->block = (uint8_t*)s->arr2;
    s->mtfv = (uint16_t*)s->arr1;
    s->zbits = NULL;
    s->ptr = (uint32_t*)s->arr1;
    strm->state = s;
    strm->total_in_lo32 = 0;
    strm->total_in_hi32 = 0;
    strm->total_out_lo32 = 0;
    strm->total_out_hi32 = 0;
    init_RL(s);
    prepare_new_block(s);
    return BZ_OK;
}

void AppBzip2::add_pair_to_block(EState* s)
{
    int32_t i;
    uint8_t ch = (uint8_t)(s->state_in_ch);

    for (i = 0; i < s->state_in_len; i++)
        BZ_UPDATE_CRC(s->blockCRC, ch);
    
    s->inUse[s->state_in_ch] = 1;
   switch (s->state_in_len) {
      case 1:
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         break;
      case 2:
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         break;
      case 3:
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         break;
      default:
         s->inUse[s->state_in_len-4] = 1;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = (uint8_t)ch; s->nblock++;
         s->block[s->nblock] = ((uint8_t)(s->state_in_len-4));
         s->nblock++;
         break;
   }
}

void AppBzip2::flush_RL(EState* s)
{
    if (s->state_in_ch < 256) add_pair_to_block(s);
    init_RL(s);
}

uint8_t AppBzip2::copy_input_until_stop(EState* s)
{
    uint8_t progress_in = 0;

    if (s->mode == BZ_M_RUNNING)
    {
        while (1)
        {
            if (s->nblock >= s->nblockMAX) break;
            if (s->strm->avail_in == 0) break;
            progress_in = 1;
            uint32_t zchh = (uint32_t)((uint32_t)(*((uint8_t*)(s->strm->next_in))));

            if (zchh != s->state_in_ch && s->state_in_len == 1)
            {
                uint8_t ch = (uint8_t)(s->state_in_ch);
                BZ_UPDATE_CRC(s->blockCRC, ch);
                s->inUse[s->state_in_ch] = 1;
                s->block[s->nblock] = (uint8_t)ch;
                s->nblock++;
                s->state_in_ch = zchh;
            }
            else
   if (zchh != s->state_in_ch ||               
      s->state_in_len == 255) {                
      if (s->state_in_ch < 256)                
         add_pair_to_block (s);             
      s->state_in_ch = zchh;                   
      s->state_in_len = 1;                     
   } else {                                    
      s->state_in_len++;                       
   }                                           



         s->strm->next_in++;
         s->strm->avail_in--;
         s->strm->total_in_lo32++;
         if (s->strm->total_in_lo32 == 0) s->strm->total_in_hi32++;
      }

   } else {

      while (1) {
         if (s->nblock >= s->nblockMAX) break;
         if (s->strm->avail_in == 0) break;
         if (s->avail_in_expect == 0) break;
         progress_in = 1;

   uint32_t zchh = (uint32_t)(  (uint32_t)(*((uint8_t*)(s->strm->next_in)))    );                 
   if (zchh != s->state_in_ch &&                 
       s->state_in_len == 1) {                   
      uint8_t ch = (uint8_t)(s->state_in_ch);        
      BZ_UPDATE_CRC( s->blockCRC, ch );         
      s->inUse[s->state_in_ch] = 1;         
      s->block[s->nblock] = (uint8_t)ch;         
      s->nblock++;                             
      s->state_in_ch = zchh;                  
   }                                         
   else                                     
    if (zchh != s->state_in_ch ||               
      s->state_in_len == 255) {                
      if (s->state_in_ch < 256)                
         add_pair_to_block (s);             
      s->state_in_ch = zchh;                   
      s->state_in_len = 1;                     
    } else {                                    
      s->state_in_len++;                       
    }                          

         s->strm->next_in++;
         s->strm->avail_in--;
         s->strm->total_in_lo32++;
         if (s->strm->total_in_lo32 == 0) s->strm->total_in_hi32++;
         s->avail_in_expect--;
      }
    }
    return progress_in;
}

uint8_t AppBzip2::copy_output_until_stop(EState* s)
{
    uint8_t progress_out = 0;

    while (1)
    {
        if (s->strm->avail_out == 0) break;
        if (s->state_out_pos >= s->numZ) break;
        progress_out = 1;
        *(s->strm->next_out) = s->zbits[s->state_out_pos];
        s->state_out_pos++;
        s->strm->avail_out--;
        s->strm->next_out++;
        s->strm->total_out_lo32++;
        if (s->strm->total_out_lo32 == 0) s->strm->total_out_hi32++;
    }

    return progress_out;
}

uint8_t AppBzip2::handle_compress(bz_stream* strm)
{
    uint8_t progress_in  = 0;
    uint8_t progress_out = 0;
    EState* s = (EState *)strm->state;
   
    while (1)
    {
        if (s->state == BZ_S_OUTPUT)
        {
         progress_out |= copy_output_until_stop ( s );
         if (s->state_out_pos < s->numZ) break;
         if (s->mode == BZ_M_FINISHING && 
             s->avail_in_expect == 0 &&
             isempty_RL(s)) break;
         prepare_new_block ( s );
         s->state = BZ_S_INPUT;
         if (s->mode == BZ_M_FLUSHING && 
             s->avail_in_expect == 0 &&
             isempty_RL(s)) break;
      }

      if (s->state == BZ_S_INPUT) {
         progress_in |= copy_input_until_stop ( s );
         if (s->mode != BZ_M_RUNNING && s->avail_in_expect == 0) {
            flush_RL ( s );
            BZ2_compressBlock ( s, (uint8_t)(s->mode == BZ_M_FINISHING) );
            s->state = BZ_S_OUTPUT;
         }
         else
         if (s->nblock >= s->nblockMAX) {
            BZ2_compressBlock ( s, 0 );
            s->state = BZ_S_OUTPUT;
         }
         else
         if (s->strm->avail_in == 0) {
            break;
         }
      }

   }

   return progress_in || progress_out;
}

int AppBzip2::BZ2_bzCompress(bz_stream *strm, int action)
{
    uint8_t progress;
    EState* s;
    if (strm == NULL) return BZ_PARAM_ERROR;
    s = (EState *)strm->state;
    if (s == NULL) return BZ_PARAM_ERROR;
    if (s->strm != strm) return BZ_PARAM_ERROR;

    preswitch:
    switch (s->mode)
    {
    case BZ_M_IDLE:
        return BZ_SEQUENCE_ERROR;
    case BZ_M_RUNNING:
         if (action == BZ_RUN) {
            progress = handle_compress ( strm );
            return progress ? BZ_RUN_OK : BZ_PARAM_ERROR;
         } 
         else
	 if (action == BZ_FLUSH) {
            s->avail_in_expect = strm->avail_in;
            s->mode = BZ_M_FLUSHING;
            goto preswitch;
         }
         else
         if (action == BZ_FINISH) {
            s->avail_in_expect = strm->avail_in;
            s->mode = BZ_M_FINISHING;
            goto preswitch;
         }
         else 
            return BZ_PARAM_ERROR;

      case BZ_M_FLUSHING:
         if (action != BZ_FLUSH) return BZ_SEQUENCE_ERROR;
         if (s->avail_in_expect != s->strm->avail_in) 
            return BZ_SEQUENCE_ERROR;
         progress = handle_compress ( strm );
         if (s->avail_in_expect > 0 || !isempty_RL(s) ||
             s->state_out_pos < s->numZ) return BZ_FLUSH_OK;
         s->mode = BZ_M_RUNNING;
         return BZ_RUN_OK;

      case BZ_M_FINISHING:
         if (action != BZ_FINISH) return BZ_SEQUENCE_ERROR;
         if (s->avail_in_expect != s->strm->avail_in) 
            return BZ_SEQUENCE_ERROR;
         progress = handle_compress ( strm );
         if (!progress) return BZ_SEQUENCE_ERROR;
         if (s->avail_in_expect > 0 || !isempty_RL(s) ||
             s->state_out_pos < s->numZ) return BZ_FINISH_OK;
         s->mode = BZ_M_IDLE;
         return BZ_STREAM_END;
   }
   return BZ_OK;
}

int AppBzip2::BZ2_bzCompressEnd(bz_stream *strm)
{
    EState *s;
    if (strm == NULL) return BZ_PARAM_ERROR;
    s = (EState *)strm->state;
    if (s == NULL) return BZ_PARAM_ERROR;
    if (s->strm != strm) return BZ_PARAM_ERROR;
#if 0
    if (s->arr1 != NULL) (strm->bzfree)(strm->opaque,(s->arr1));
    if (s->arr2 != NULL) (strm->bzfree)(strm->opaque,(s->arr2));
    if (s->ftab != NULL) (strm->bzfree)(strm->opaque,(s->ftab));
    (strm->bzfree)(strm->opaque,(strm->state));
#else
    if (s->arr1 != NULL) delete[] s->arr1;
    if (s->arr2 != NULL) delete[] s->arr2;
    if (s->ftab != NULL) delete[] s->ftab;
    delete[] strm->state;
#endif
    strm->state = NULL;   
    return BZ_OK;
}

int AppBzip2::BZ2_bzDecompressInit(bz_stream* strm, int verbosity, int small)
{
    DState *s;
    if (!bz_config_ok()) return BZ_CONFIG_ERROR;
    if (strm == NULL) return BZ_PARAM_ERROR;
    if (small != 0 && small != 1) return BZ_PARAM_ERROR;
    if (verbosity < 0 || verbosity > 4) return BZ_PARAM_ERROR;
    //if (strm->bzfree == NULL) strm->bzfree = default_bzfree;
    s = new DState[sizeof(DState)];
    if (s == NULL) return BZ_MEM_ERROR;
    s->strm                  = strm;
    strm->state              = s;
    s->state                 = BZ_X_MAGIC_1;
    s->bsLive                = 0;
    s->bsBuff                = 0;
    s->calculatedCombinedCRC = 0;
    strm->total_in_lo32      = 0;
    strm->total_in_hi32      = 0;
    strm->total_out_lo32     = 0;
    strm->total_out_hi32     = 0;
    s->smallDecompress       = (uint8_t)small;
    s->ll4 = NULL;
    s->ll16 = NULL;
    s->tt = NULL;
    s->currBlockNo           = 0;
    s->verbosity             = verbosity;
    return BZ_OK;
}

void AppBzip2::unRLE_obuf_to_output_FAST(DState* s)
{
    uint8_t k1;

    if (s->blockRandomised)
    {
        while (1)
        {
            while (1)
            {
                if (s->strm->avail_out == 0) return;
                if (s->state_out_len == 0) break;
                *( (uint8_t*)(s->strm->next_out) ) = s->state_out_ch;
                BZ_UPDATE_CRC ( s->calculatedBlockCRC, s->state_out_ch );
                s->state_out_len--;
                s->strm->next_out++;
                s->strm->avail_out--;
                s->strm->total_out_lo32++;
                if (s->strm->total_out_lo32 == 0) s->strm->total_out_hi32++;
            }
   
            if (s->nblock_used == s->save_nblock+1) return;
               
   
            s->state_out_len = 1;
            s->state_out_ch = s->k0;

            s->tPos = s->tt[s->tPos];
            k1 = (uint8_t)(s->tPos & 0xff);
            s->tPos >>= 8;



        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            k1 ^= ((s->rNToGo == 1) ? 1 : 0);
            s->nblock_used++;
            if (s->nblock_used == s->save_nblock+1) continue;
            if (k1 != s->k0) { s->k0 = k1; continue; };
   
            s->state_out_len = 2;

            s->tPos = s->tt[s->tPos];
        k1 = (uint8_t)(s->tPos & 0xff);
        s->tPos >>= 8;


        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

         k1 ^= ((s->rNToGo == 1) ? 1 : 0);
        s->nblock_used++;
         if (s->nblock_used == s->save_nblock+1) continue;
         if (k1 != s->k0) { s->k0 = k1; continue; };
   
         s->state_out_len = 3;



        s->tPos = s->tt[s->tPos];
        k1 = (uint8_t)(s->tPos & 0xff);
        s->tPos >>= 8;

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

         k1 ^= ((s->rNToGo == 1) ? 1 : 0);
        s->nblock_used++;
         if (s->nblock_used == s->save_nblock+1) continue;
         if (k1 != s->k0) { s->k0 = k1; continue; };
         s->tPos = s->tt[s->tPos];
        k1 = (uint8_t)(s->tPos & 0xff);
        s->tPos >>= 8;  

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

         k1 ^= ((s->rNToGo == 1) ? 1 : 0);
        s->nblock_used++;
         s->state_out_len = ((int32_t)k1) + 4;

        s->tPos = s->tt[s->tPos];
        s->k0 = (uint8_t)(s->tPos & 0xff);
        s->tPos >>= 8;


        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

         s->k0 ^= ((s->rNToGo == 1) ? 1 : 0);
        s->nblock_used++;
      }

   } else
    {
      uint32_t        c_calculatedBlockCRC = s->calculatedBlockCRC;
      uint8_t         c_state_out_ch       = s->state_out_ch;
      int32_t         c_state_out_len      = s->state_out_len;
      int32_t         c_nblock_used        = s->nblock_used;
      int32_t         c_k0                 = s->k0;
      uint32_t*       c_tt                 = s->tt;
      uint32_t        c_tPos               = s->tPos;
      char*         cs_next_out          = s->strm->next_out;
      unsigned int  cs_avail_out         = s->strm->avail_out;
      uint32_t       avail_out_INIT = cs_avail_out;
      int32_t        s_save_nblockPP = s->save_nblock+1;
      unsigned int total_out_lo32_old;

      while (1) {
         if (c_state_out_len > 0) {
            while (1) {
               if (cs_avail_out == 0) goto return_notr;
               if (c_state_out_len == 1) break;
               *( (uint8_t*)(cs_next_out) ) = c_state_out_ch;
               BZ_UPDATE_CRC ( c_calculatedBlockCRC, c_state_out_ch );
               c_state_out_len--;
               cs_next_out++;
               cs_avail_out--;
            }
            s_state_out_len_eq_one:
            {
               if (cs_avail_out == 0) { 
                  c_state_out_len = 1; goto return_notr;
               };
               *( (uint8_t*)(cs_next_out) ) = c_state_out_ch;
               BZ_UPDATE_CRC ( c_calculatedBlockCRC, c_state_out_ch );
               cs_next_out++;
               cs_avail_out--;
            }
         }

            if (c_nblock_used == s_save_nblockPP)
            {
                c_state_out_len = 0;
                goto return_notr;
            };

            c_state_out_ch = c_k0;
            c_tPos = c_tt[c_tPos];
            k1 = (uint8_t)(c_tPos & 0xff);
            c_tPos >>= 8;
            c_nblock_used++;

            if (k1 != c_k0)
            {
                c_k0 = k1;
                goto s_state_out_len_eq_one; 
            };

            if (c_nblock_used == s_save_nblockPP) 
                goto s_state_out_len_eq_one;
   
            c_state_out_len = 2;

            c_tPos = c_tt[c_tPos];
            k1 = (uint8_t)(c_tPos & 0xff);
            c_tPos >>= 8;

            c_nblock_used++;
            if (c_nblock_used == s_save_nblockPP) continue;
            if (k1 != c_k0) { c_k0 = k1; continue; };
            c_state_out_len = 3;

            c_tPos = c_tt[c_tPos];
            k1 = (uint8_t)(c_tPos & 0xff);
            c_tPos >>= 8;

            c_nblock_used++;
            if (c_nblock_used == s_save_nblockPP) continue;
            if (k1 != c_k0) { c_k0 = k1; continue; };

            c_tPos = c_tt[c_tPos];
            k1 = (uint8_t)(c_tPos & 0xff);
            c_tPos >>= 8;

            c_nblock_used++;
            c_state_out_len = ((int32_t)k1) + 4;

            c_tPos = c_tt[c_tPos];
            c_k0 = (uint8_t)(c_tPos & 0xff);
            c_tPos >>= 8;

            c_nblock_used++;
        }

return_notr:
        total_out_lo32_old = s->strm->total_out_lo32;
        s->strm->total_out_lo32 += (avail_out_INIT - cs_avail_out);

        if (s->strm->total_out_lo32 < total_out_lo32_old)
            s->strm->total_out_hi32++;

        s->calculatedBlockCRC = c_calculatedBlockCRC;
        s->state_out_ch       = c_state_out_ch;
        s->state_out_len      = c_state_out_len;
        s->nblock_used        = c_nblock_used;
        s->k0                 = c_k0;
        s->tt                 = c_tt;
        s->tPos               = c_tPos;
        s->strm->next_out     = cs_next_out;
        s->strm->avail_out    = cs_avail_out;
    }
}

void AppBzip2::unRLE_obuf_to_output_SMALL(DState* s)
{
    uint8_t k1;

    if (s->blockRandomised)
    {
        while (1)
        {
            while (1)
            {
                if (s->strm->avail_out == 0) return;
                if (s->state_out_len == 0) break;
                *( (uint8_t*)(s->strm->next_out) ) = s->state_out_ch;
                BZ_UPDATE_CRC ( s->calculatedBlockCRC, s->state_out_ch );
                s->state_out_len--;
                s->strm->next_out++;
                s->strm->avail_out--;
                s->strm->total_out_lo32++;
                if (s->strm->total_out_lo32 == 0) s->strm->total_out_hi32++;
            }

            if (s->nblock_used == s->save_nblock+1) return;
             

  
            s->state_out_len = 1;
            s->state_out_ch = s->k0;
            k1 = BZ2_indexIntoF( s->tPos, s->cftab );
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
             (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            k1 ^= ((s->rNToGo == 1) ? 1 : 0);
            s->nblock_used++;
            if (s->nblock_used == s->save_nblock+1) continue;
            if (k1 != s->k0) { s->k0 = k1; continue; };
   
            s->state_out_len = 2;
            k1 = BZ2_indexIntoF ( s->tPos, s->cftab );
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
             (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            k1 ^= ((s->rNToGo == 1) ? 1 : 0);
            s->nblock_used++;
            if (s->nblock_used == s->save_nblock+1) continue;
            if (k1 != s->k0) { s->k0 = k1; continue; };
   
            s->state_out_len = 3;
            k1 = BZ2_indexIntoF ( s->tPos, s->cftab );
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
    (  ((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            k1 ^= ((s->rNToGo == 1) ? 1 : 0);
            s->nblock_used++;
            if (s->nblock_used == s->save_nblock+1) continue;
            if (k1 != s->k0) { s->k0 = k1; continue; };
   
            k1 = BZ2_indexIntoF ( s->tPos, s->cftab );
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
    ( ((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF)  << 16));

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            k1 ^= ((s->rNToGo == 1) ? 1 : 0);
            s->nblock_used++;
            s->state_out_len = ((int32_t)k1) + 4;
            s->k0 = BZ2_indexIntoF ( s->tPos, s->cftab );
            s->tPos = (((uint32_t)s->ll16[s->tPos]) |
    ( ((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));

        if (s->rNToGo == 0) { s->rNToGo = BZ2_rNums[s->rTPos]; s->rTPos++;
      if (s->rTPos == 512) s->rTPos = 0; } s->rNToGo--;

            s->k0 ^= ((s->rNToGo == 1) ? 1 : 0);
            s->nblock_used++;
        }

   } else {

      while (1) {
         while (1) {
            if (s->strm->avail_out == 0) return;
            if (s->state_out_len == 0) break;
            *( (uint8_t*)(s->strm->next_out) ) = s->state_out_ch;
            BZ_UPDATE_CRC ( s->calculatedBlockCRC, s->state_out_ch );
            s->state_out_len--;
            s->strm->next_out++;
            s->strm->avail_out--;
            s->strm->total_out_lo32++;
            if (s->strm->total_out_lo32 == 0) s->strm->total_out_hi32++;
         }
   
         if (s->nblock_used == s->save_nblock+1) return;
 


         s->state_out_len = 1;
         s->state_out_ch = s->k0;
         k1 = BZ2_indexIntoF(s->tPos, s->cftab);
         s->tPos = (((uint32_t)s->ll16[s->tPos]) |
            (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));
         s->nblock_used++;
         if (s->nblock_used == s->save_nblock+1) continue;
         if (k1 != s->k0) { s->k0 = k1; continue; };
   
         s->state_out_len = 2;
         k1 = BZ2_indexIntoF ( s->tPos, s->cftab );
         s->tPos = (((uint32_t)s->ll16[s->tPos]) |
            (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));
         s->nblock_used++;
         if (s->nblock_used == s->save_nblock+1) continue;
         if (k1 != s->k0) { s->k0 = k1; continue; };
   
         s->state_out_len = 3;
         k1 = BZ2_indexIntoF ( s->tPos, s->cftab );
         s->tPos = (((uint32_t)s->ll16[s->tPos]) |
            (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));
         s->nblock_used++;
         if (s->nblock_used == s->save_nblock+1) continue;
         if (k1 != s->k0) { s->k0 = k1; continue; };
   
         k1 = BZ2_indexIntoF ( s->tPos, s->cftab );
         s->tPos = (((uint32_t)s->ll16[s->tPos]) |
            (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF)  << 16));
         s->nblock_used++;
         s->state_out_len = ((int32_t)k1) + 4;
         s->k0 = BZ2_indexIntoF ( s->tPos, s->cftab );
         s->tPos = (((uint32_t)s->ll16[s->tPos]) |
            (((((uint32_t)(s->ll4[(s->tPos) >> 1])) >> (((s->tPos) << 2) & 0x4)) & 0xF) << 16));
         s->nblock_used++;
      }

   }
}

int AppBzip2::BZ2_bzDecompress(bz_stream *strm)
{
    DState *s;
    if (strm == NULL) return BZ_PARAM_ERROR;
    s = (DState *)strm->state;
    if (s == NULL) return BZ_PARAM_ERROR;
    if (s->strm != strm) return BZ_PARAM_ERROR;

    while (1)
    {
      if (s->state == BZ_X_IDLE) return BZ_SEQUENCE_ERROR;
      if (s->state == BZ_X_OUTPUT) {
         if (s->smallDecompress)
            unRLE_obuf_to_output_SMALL ( s ); else
            unRLE_obuf_to_output_FAST  ( s );
         if (s->nblock_used == s->save_nblock+1 && s->state_out_len == 0)
         {
            s->calculatedBlockCRC = ~(s->calculatedBlockCRC);

            if (s->verbosity >= 3) 
               fprintf(stderr, " {0x%x, 0x%x}", s->storedBlockCRC, 
                          s->calculatedBlockCRC );
            if (s->verbosity >= 2) VPrintf0 ( "]" );
            if (s->calculatedBlockCRC != s->storedBlockCRC)
               return BZ_DATA_ERROR;
            s->calculatedCombinedCRC 
               = (s->calculatedCombinedCRC << 1) | 
                    (s->calculatedCombinedCRC >> 31);
            s->calculatedCombinedCRC ^= s->calculatedBlockCRC;
            s->state = BZ_X_BLKHDR_1;
         } else {
            return BZ_OK;
         }
      }
      if (s->state >= BZ_X_MAGIC_1) {
         int32_t r = BZ2_decompress ( s );
         if (r == BZ_STREAM_END) {
            if (s->verbosity >= 3)
               fprintf(stderr, "\n    combined CRCs: stored = 0x%x, computed = 0x%x", 
                          s->storedCombinedCRC, s->calculatedCombinedCRC );
            if (s->calculatedCombinedCRC != s->storedCombinedCRC)
               return BZ_DATA_ERROR;
            return r;
         }
         if (s->state != BZ_X_OUTPUT) return r;
      }
    }

    AssertH(0, 6001);
    return 0;
}

int AppBzip2::BZ2_bzDecompressEnd(bz_stream *strm)
{
    DState* s;
    if (strm == NULL) return BZ_PARAM_ERROR;
    s = (DState *)strm->state;
    if (s == NULL) return BZ_PARAM_ERROR;
    if (s->strm != strm) return BZ_PARAM_ERROR;
#if 0
    if (s->tt   != NULL) (strm->bzfree)(strm->opaque,(s->tt));
    if (s->ll16 != NULL) (strm->bzfree)(strm->opaque,(s->ll16));
    if (s->ll4  != NULL) (strm->bzfree)(strm->opaque,(s->ll4));
#else
    if (s->tt   != NULL) delete[] s->tt;
    if (s->ll16 != NULL) delete[] s->ll16;
    if (s->ll4  != NULL) delete[] s->ll4;
#endif
    //(strm->bzfree)(strm->opaque,(strm->state));
    delete[] strm->state;
    strm->state = NULL;
    return BZ_OK;
}

uint8_t AppBzip2::myfeof(FILE *f)
{
    int32_t c = fgetc(f);
    if (c == EOF) return 1;
    ungetc(c, f);
    return 0;
}

BZFILE *AppBzip2::BZ2_bzWriteOpen(int *bzerror, FILE *f, int blockSize100k,
                int verbosity, int workFactor)
{
    int32_t ret;
    bzFile* bzf = NULL;

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;


    if (f == NULL ||
       (blockSize100k < 1 || blockSize100k > 9) ||
       (workFactor < 0 || workFactor > 250) ||
       (verbosity < 0 || verbosity > 4))
    {
        if (bzerror != NULL) *bzerror = BZ_PARAM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_PARAM_ERROR;

        return NULL;
    };

    if (ferror(f))
    {
        if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
        return NULL;
    };

    //bzf = (bzFile *)malloc ( sizeof(bzFile) );
    bzf = new bzFile;

    if (bzf == NULL)
    {
        if (bzerror != NULL) *bzerror = BZ_MEM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_MEM_ERROR;
        return NULL;
    };

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;
    bzf->initialisedOk = 0;
    bzf->bufN          = 0;
    bzf->handle        = f;
    bzf->writing       = 1;
    //bzf->strm.bzfree   = NULL;
    bzf->strm.opaque   = NULL;
    if (workFactor == 0) workFactor = 30;
    ret = BZ2_bzCompressInit(&(bzf->strm), blockSize100k, verbosity, workFactor);

    if (ret != BZ_OK)
    {
        if (bzerror != NULL) *bzerror = ret;
        if (bzf != NULL) bzf->lastErr = ret;

        //free(bzf);
        delete bzf;
        return NULL;
    };

   bzf->strm.avail_in = 0;
   bzf->initialisedOk = 1;
   return bzf;   
}

void AppBzip2::BZ2_bzWrite(int *bzerror, BZFILE* b, void *buf, int len)
{
    int32_t n, n2, ret;
    bzFile* bzf = (bzFile*)b;

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;

    if (bzf == NULL || buf == NULL || len < 0)
    {
        if (bzerror != NULL) *bzerror = BZ_PARAM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_PARAM_ERROR;
        return;
    };

    if (!(bzf->writing))
    {
        if (bzerror != NULL) *bzerror = BZ_SEQUENCE_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_SEQUENCE_ERROR;
        return;
    };

    if (ferror(bzf->handle))
    {
        if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
        return;
    };

    if (len == 0)
    {
        if (bzerror != NULL) *bzerror = BZ_OK;
        if (bzf != NULL) bzf->lastErr = BZ_OK;
        return;
    };

    bzf->strm.avail_in = len;
    bzf->strm.next_in  = (char *)buf;

    while (1)
    {
        bzf->strm.avail_out = BZ_MAX_UNUSED;
        bzf->strm.next_out = bzf->buf;
        ret = BZ2_bzCompress ( &(bzf->strm), BZ_RUN );

        if (ret != BZ_RUN_OK)
        {
        if (bzerror != NULL) *bzerror = ret;
        if (bzf != NULL) bzf->lastErr = ret;
            return;
        };

        if (bzf->strm.avail_out < BZ_MAX_UNUSED)
        {
         n = BZ_MAX_UNUSED - bzf->strm.avail_out;
         n2 = fwrite ( (void*)(bzf->buf), sizeof(uint8_t), 
                       n, bzf->handle );
         if (n != n2 || ferror(bzf->handle))
         {
        if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
            return;
         };
      }

         if (bzf->strm.avail_in == 0)
         {
        if (bzerror != NULL) *bzerror = BZ_OK;
        if (bzf != NULL) bzf->lastErr = BZ_OK;
            return;
         };
   }
}

void AppBzip2::BZ2_bzWriteClose64(int *bzerror,
    BZFILE *b, int abandon, unsigned int *nbytes_in_lo32,
    unsigned int *nbytes_in_hi32, unsigned int *nbytes_out_lo32, unsigned int *nbytes_out_hi32)
{
    int32_t n, n2, ret;
    bzFile* bzf = (bzFile*)b;

    if (bzf == NULL)
    {
        if (bzerror != NULL) *bzerror = BZ_OK;
        if (bzf != NULL) bzf->lastErr = BZ_OK;
        return;
    };
    if (!(bzf->writing))
    {
        if (bzerror != NULL) *bzerror = BZ_SEQUENCE_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_SEQUENCE_ERROR;
        return;
    };
    if (ferror(bzf->handle))
    {
        if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
        return;
    };

    if (nbytes_in_lo32 != NULL) *nbytes_in_lo32 = 0;
    if (nbytes_in_hi32 != NULL) *nbytes_in_hi32 = 0;
    if (nbytes_out_lo32 != NULL) *nbytes_out_lo32 = 0;
    if (nbytes_out_hi32 != NULL) *nbytes_out_hi32 = 0;

    if ((!abandon) && bzf->lastErr == BZ_OK)
    {
        while (1)
        {
            bzf->strm.avail_out = BZ_MAX_UNUSED;
            bzf->strm.next_out = bzf->buf;
            ret = BZ2_bzCompress ( &(bzf->strm), BZ_FINISH );

            if (ret != BZ_FINISH_OK && ret != BZ_STREAM_END)
            {
                if (bzerror != NULL) *bzerror = ret;
                if (bzf != NULL) bzf->lastErr = ret;
                return;
            };

            if (bzf->strm.avail_out < BZ_MAX_UNUSED)
            {
                n = BZ_MAX_UNUSED - bzf->strm.avail_out;
                n2 = fwrite ( (void*)(bzf->buf), sizeof(uint8_t), 
                          n, bzf->handle );
                if (n != n2 || ferror(bzf->handle))
                {
                    if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
                    if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
                    return;
                };
            }

            if (ret == BZ_STREAM_END) break;
        }
    }

    if ( !abandon && !ferror ( bzf->handle ) )
    {
        fflush ( bzf->handle );
        if (ferror(bzf->handle))
        {
            if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
            if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
            return;
        };
    }

   if (nbytes_in_lo32 != NULL)
      *nbytes_in_lo32 = bzf->strm.total_in_lo32;
   if (nbytes_in_hi32 != NULL)
      *nbytes_in_hi32 = bzf->strm.total_in_hi32;
   if (nbytes_out_lo32 != NULL)
      *nbytes_out_lo32 = bzf->strm.total_out_lo32;
   if (nbytes_out_hi32 != NULL)
      *nbytes_out_hi32 = bzf->strm.total_out_hi32;

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;
    BZ2_bzCompressEnd ( &(bzf->strm) );
    delete bzf;
   //free ( bzf );
}

void AppBzip2::BZ2_bzWriteClose(int *bzerror, BZFILE *b, int abandon, unsigned int *nbytes_in,
                    unsigned int *nbytes_out)
{
    BZ2_bzWriteClose64(bzerror, b, abandon, nbytes_in, NULL, nbytes_out, NULL);
}

BZFILE *AppBzip2::BZ2_bzReadOpen(int *bzerror, FILE *f, int verbosity, int small, void *unused,
            int nUnused)
{
    bzFile* bzf = NULL;
    int     ret;

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;

    if (f == NULL || 
       (small != 0 && small != 1) ||
       (verbosity < 0 || verbosity > 4) ||
       (unused == NULL && nUnused != 0) ||
       (unused != NULL && (nUnused < 0 || nUnused > BZ_MAX_UNUSED)))
    {
        if (bzerror != NULL) *bzerror = BZ_PARAM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_PARAM_ERROR;
        return NULL;
    };

    if (ferror(f))
    {
        if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
        return NULL;
    };

    //bzf = (bzFile *)malloc ( sizeof(bzFile) );
    bzf = new bzFile;

    if (bzf == NULL) 
    {
        if (bzerror != NULL) *bzerror = BZ_MEM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_MEM_ERROR;
        return NULL;
    };

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;


    bzf->initialisedOk = 0;
    bzf->handle        = f;
    bzf->bufN          = 0;
    bzf->writing       = 0;
    //bzf->strm.bzfree   = NULL;
    bzf->strm.opaque   = NULL;
   
    while (nUnused > 0)
    {
        bzf->buf[bzf->bufN] = *((uint8_t*)(unused)); bzf->bufN++;
        unused = ((void*)( 1 + ((uint8_t*)(unused))  ));
        nUnused--;
    }

    ret = BZ2_bzDecompressInit(&(bzf->strm), verbosity, small);

    if (ret != BZ_OK)
    {
        if (bzerror != NULL) *bzerror = ret;
        if (bzf != NULL) bzf->lastErr = ret;
        //free(bzf);
        delete bzf;
        return NULL;
    };

   bzf->strm.avail_in = bzf->bufN;
   bzf->strm.next_in  = bzf->buf;

   bzf->initialisedOk = 1;
   return bzf;   
}

void AppBzip2::BZ2_bzReadClose(int *bzerror, BZFILE *b)
{
    bzFile* bzf = (bzFile*)b;

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;

    if (bzf == NULL)
    {
        if (bzerror != NULL) *bzerror = BZ_OK;
        if (bzf != NULL) bzf->lastErr = BZ_OK;
        return;
    };

    if (bzf->writing)
    {
        if (bzerror != NULL) *bzerror = BZ_SEQUENCE_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_SEQUENCE_ERROR;
        return;
    };

    if (bzf->initialisedOk)
        (void)BZ2_bzDecompressEnd ( &(bzf->strm) );

    //free ( bzf );
    delete bzf;
}

int AppBzip2::BZ2_bzRead(int *bzerror, BZFILE* b, void *buf, int len)
{
    int32_t   n, ret;
    bzFile* bzf = (bzFile*)b;

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;

    if (bzf == NULL || buf == NULL || len < 0)
    {
        if (bzerror != NULL) *bzerror = BZ_PARAM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_PARAM_ERROR;
        return 0;
    };

    if (bzf->writing)
    {
        if (bzerror != NULL) *bzerror = BZ_SEQUENCE_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_SEQUENCE_ERROR;
        return 0;
    };

    if (len == 0)
    {
        if (bzerror != NULL) *bzerror = BZ_OK;
        if (bzf != NULL) bzf->lastErr = BZ_OK;
        return 0;
    };

    bzf->strm.avail_out = len;
    bzf->strm.next_out = (char *)buf;

    while (1)
    {
        if (ferror(bzf->handle)) 
        {
            if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
            if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
            return 0;
        };

        if (bzf->strm.avail_in == 0 && !myfeof(bzf->handle))
        {
            n = fread(bzf->buf, sizeof(uint8_t), BZ_MAX_UNUSED, bzf->handle);

            if (ferror(bzf->handle))
            {
                if (bzerror != NULL) *bzerror = BZ_IO_ERROR;
                if (bzf != NULL) bzf->lastErr = BZ_IO_ERROR;
                return 0;
            };

            bzf->bufN = n;
            bzf->strm.avail_in = bzf->bufN;
            bzf->strm.next_in = bzf->buf;
        }

        ret = BZ2_bzDecompress ( &(bzf->strm) );

        if (ret != BZ_OK && ret != BZ_STREAM_END)
        {
                if (bzerror != NULL) *bzerror = ret;
                if (bzf != NULL) bzf->lastErr = ret;
            return 0;
        };

        if (ret == BZ_OK && myfeof(bzf->handle) && 
            bzf->strm.avail_in == 0 && bzf->strm.avail_out > 0)
        {
            if (bzerror != NULL) *bzerror = BZ_UNEXPECTED_EOF;
            if (bzf != NULL) bzf->lastErr = BZ_UNEXPECTED_EOF;
            return 0;
        };

        if (ret == BZ_STREAM_END)
        {
            if (bzerror != NULL) *bzerror = BZ_STREAM_END;
            if (bzf != NULL) bzf->lastErr = BZ_STREAM_END;
            return len - bzf->strm.avail_out;
        };

        if (bzf->strm.avail_out == 0)
        {
            if (bzerror != NULL) *bzerror = BZ_OK;
            if (bzf != NULL) bzf->lastErr = BZ_OK;
            return len;
        };
    }

    return 0;
}

void AppBzip2::BZ2_bzReadGetUnused(int *bzerror, BZFILE* b, void **unused, int *nUnused)
{
    bzFile* bzf = (bzFile*)b;

    if (bzf == NULL)
    {
        if (bzerror != NULL) *bzerror = BZ_PARAM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_PARAM_ERROR;
        return;
    };

    if (bzf->lastErr != BZ_STREAM_END)
    {
        if (bzerror != NULL) *bzerror = BZ_SEQUENCE_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_SEQUENCE_ERROR;
        return;
    };
    if (unused == NULL || nUnused == NULL)
    {
        if (bzerror != NULL) *bzerror = BZ_PARAM_ERROR;
        if (bzf != NULL) bzf->lastErr = BZ_PARAM_ERROR;
        return;
    };

    if (bzerror != NULL) *bzerror = BZ_OK;
    if (bzf != NULL) bzf->lastErr = BZ_OK;
    *nUnused = bzf->strm.avail_in;
    *unused = bzf->strm.next_in;
}

int AppBzip2::BZ2_bzBuffToBuffCompress(char *dest, unsigned int *destLen, char *source, 
                  unsigned int sourceLen, int blockSize100k, int verbosity, int workFactor)
{
    bz_stream strm;
    int ret;

    if (dest == NULL || destLen == NULL || 
       source == NULL ||
       blockSize100k < 1 || blockSize100k > 9 ||
       verbosity < 0 || verbosity > 4 ||
       workFactor < 0 || workFactor > 250) 
      return BZ_PARAM_ERROR;

   if (workFactor == 0) workFactor = 30;
   //strm.bzfree = NULL;
   strm.opaque = NULL;
   ret = BZ2_bzCompressInit(&strm, blockSize100k, verbosity, workFactor);
   if (ret != BZ_OK) return ret;
   strm.next_in = source;
   strm.next_out = dest;
   strm.avail_in = sourceLen;
   strm.avail_out = *destLen;
   ret = BZ2_bzCompress ( &strm, BZ_FINISH );
   if (ret == BZ_FINISH_OK) goto output_overflow;
   if (ret != BZ_STREAM_END) goto errhandler;
   *destLen -= strm.avail_out;   
   BZ2_bzCompressEnd ( &strm );
   return BZ_OK;
output_overflow:
   BZ2_bzCompressEnd ( &strm );
   return BZ_OUTBUFF_FULL;
errhandler:
   BZ2_bzCompressEnd ( &strm );
   return ret;
}

int AppBzip2::BZ2_bzBuffToBuffDecompress(char *dest, unsigned int* destLen,
                      char *source, unsigned int sourceLen, int small, int verbosity)
{
   bz_stream strm;
   int ret;

   if (dest == NULL || destLen == NULL || 
       source == NULL ||
       (small != 0 && small != 1) ||
       verbosity < 0 || verbosity > 4) 
          return BZ_PARAM_ERROR;

   //strm.bzfree = NULL;
   strm.opaque = NULL;
   ret = BZ2_bzDecompressInit ( &strm, verbosity, small );
   if (ret != BZ_OK) return ret;

   strm.next_in = source;
   strm.next_out = dest;
   strm.avail_in = sourceLen;
   strm.avail_out = *destLen;

   ret = BZ2_bzDecompress(&strm);
   if (ret == BZ_OK) goto output_overflow_or_eof;
   if (ret != BZ_STREAM_END) goto errhandler;

   *destLen -= strm.avail_out;
   BZ2_bzDecompressEnd ( &strm );
   return BZ_OK;

   output_overflow_or_eof:
   if (strm.avail_out > 0) {
      BZ2_bzDecompressEnd ( &strm );
      return BZ_UNEXPECTED_EOF;
   } else {
      BZ2_bzDecompressEnd ( &strm );
      return BZ_OUTBUFF_FULL;
   };      

   errhandler:
   BZ2_bzDecompressEnd ( &strm );
   return ret; 
}

BZFILE *AppBzip2::bzopen_or_bzdopen(const char *path, int fd, const char *mode, int open_mode)
{
    int    bzerr;
    char   unused[BZ_MAX_UNUSED];
    int    blockSize100k = 9;
    int    writing       = 0;
    char   mode2[10]     = "";
    FILE   *fp           = NULL;
    BZFILE *bzfp         = NULL;
    int    verbosity     = 0;
    int    workFactor    = 30;
    int    smallMode     = 0;
    int    nUnused       = 0; 

   if (mode == NULL) return NULL;
   while (*mode) {
      switch (*mode) {
      case 'r':
         writing = 0; break;
      case 'w':
         writing = 1; break;
      case 's':
         smallMode = 1; break;
      default:
         if (isdigit((int)(*mode))) {
            blockSize100k = *mode-BZ_HDR_0;
         }
      }
      mode++;
   }
   strcat(mode2, writing ? "w" : "r" );
   strcat(mode2,"b");

   if (open_mode==0) {
      if (path==NULL || strcmp(path,"")==0) {
        fp = (writing ? stdout : stdin);
      } else {
        fp = fopen(path,mode2);
      }
   } else {
      fp = fdopen(fd,mode2);
   }
   if (fp == NULL) return NULL;

   if (writing) {
      if (blockSize100k < 1) blockSize100k = 1;
      if (blockSize100k > 9) blockSize100k = 9; 
      bzfp = BZ2_bzWriteOpen(&bzerr,fp,blockSize100k,
                             verbosity,workFactor);
   } else {
      bzfp = BZ2_bzReadOpen(&bzerr,fp,verbosity,smallMode,
                            unused,nUnused);
   }
   if (bzfp == NULL) {
      if (fp != stdin && fp != stdout) fclose(fp);
      return NULL;
   }
   return bzfp;
}

BZFILE *AppBzip2::BZ2_bzopen(const char *path, const char *mode)
{
   return bzopen_or_bzdopen(path,-1,mode,0);
}

BZFILE *AppBzip2::BZ2_bzdopen(int fd, const char *mode)
{
    return bzopen_or_bzdopen(NULL, fd, mode, 1);
}

int AppBzip2::BZ2_bzread(BZFILE* b, void* buf, int len)
{
   int bzerr, nread;
   if (((bzFile*)b)->lastErr == BZ_STREAM_END) return 0;
   nread = BZ2_bzRead(&bzerr,b,buf,len);
   if (bzerr == BZ_OK || bzerr == BZ_STREAM_END) {
      return nread;
   } else {
      return -1;
   }
}

int AppBzip2::BZ2_bzwrite(BZFILE* b, void* buf, int len)
{
    int bzerr;

    BZ2_bzWrite(&bzerr,b,buf,len);

    if(bzerr == BZ_OK)
    {
        return len;
    }
    else
    {
        return -1;
    }
}

int AppBzip2::BZ2_bzflush(BZFILE *b)
{
    return 0;
}

void AppBzip2::BZ2_bzclose(BZFILE* b)
{
    int bzerr;
    FILE *fp = ((bzFile *)b)->handle;
    if (b==NULL) {return;}

    if(((bzFile*)b)->writing)
    {
        BZ2_bzWriteClose(&bzerr,b,0,NULL,NULL);

        if(bzerr != BZ_OK)
        {
            BZ2_bzWriteClose(NULL,b,1,NULL,NULL);
        }
    }
    else
    {
        BZ2_bzReadClose(&bzerr,b);
    }

    if(fp!=stdin && fp!=stdout)
    {
        fclose(fp);
    }
}

static const char *bzerrorstrings[] = {
       "OK"
      ,"SEQUENCE_ERROR"
      ,"PARAM_ERROR"
      ,"MEM_ERROR"
      ,"DATA_ERROR"
      ,"DATA_ERROR_MAGIC"
      ,"IO_ERROR"
      ,"UNEXPECTED_EOF"
      ,"OUTBUFF_FULL"
      ,"CONFIG_ERROR"
      ,"???"   
      ,"???"   
      ,"???"   
      ,"???"   
      ,"???"   
      ,"???"   
};

const char *AppBzip2::BZ2_bzerror(BZFILE *b, int *errnum)
{
    int err = ((bzFile *)b)->lastErr;
    if(err>0) err = 0;
    *errnum = err;
    return bzerrorstrings[err*-1];
}

void AppBzip2::uInt64_from_UInt32s(UInt64* n, uint32_t lo32, uint32_t hi32)
{
    n->b[7] = (uint8_t)((hi32 >> 24) & 0xFF);
    n->b[6] = (uint8_t)((hi32 >> 16) & 0xFF);
    n->b[5] = (uint8_t)((hi32 >> 8)  & 0xFF);
    n->b[4] = (uint8_t) (hi32        & 0xFF);
    n->b[3] = (uint8_t)((lo32 >> 24) & 0xFF);
    n->b[2] = (uint8_t)((lo32 >> 16) & 0xFF);
    n->b[1] = (uint8_t)((lo32 >> 8)  & 0xFF);
    n->b[0] = (uint8_t) (lo32        & 0xFF);
}

double AppBzip2::uInt64_to_double(UInt64 *n)
{
    int32_t  i;
    double base = 1.0;
    double sum  = 0.0;

    for (i = 0; i < 8; i++)
    {
        sum  += base * (double)(n->b[i]);
        base *= 256.0;
    }

    return sum;
}

uint8_t AppBzip2::uInt64_isZero(UInt64* n)
{
    int32_t i;

    for (i = 0; i < 8; i++)
        if (n->b[i] != 0)
            return 0;

    return 1;
}

int32_t AppBzip2::uInt64_qrm10(UInt64* n)
{
    uint32_t rem, tmp;
    int32_t  i;
    rem = 0;

    for (i = 7; i >= 0; i--)
    {
        tmp = rem * 256 + n->b[i];
        n->b[i] = tmp / 10;
        rem = tmp % 10;
    }

    return rem;
}

void AppBzip2::uInt64_toAscii(char* outbuf, UInt64* n)
{
   int32_t  i, q;
   uint8_t  buf[32];
   int32_t  nBuf   = 0;
   UInt64 n_copy = *n;
   do {
      q = uInt64_qrm10 ( &n_copy );
      buf[nBuf] = q + '0';
      nBuf++;
   } while (!uInt64_isZero(&n_copy));
   outbuf[nBuf] = 0;
   for (i = 0; i < nBuf; i++) 
      outbuf[i] = buf[nBuf-i-1];
}

void AppBzip2::compressStream(FILE *stream, FILE *zStream)
{
    BZFILE* bzf = NULL;
    uint8_t ibuf[5000];
    int32_t nIbuf;
    uint32_t nbytes_in_lo32, nbytes_in_hi32;
    uint32_t nbytes_out_lo32, nbytes_out_hi32;
    int32_t bzerr, bzerr_dummy, ret;
    if (ferror(stream)) goto errhandler_io;
    if (ferror(zStream)) goto errhandler_io;
    bzf = BZ2_bzWriteOpen(&bzerr, zStream, blockSize100k, verbosity, workFactor);
    if (bzerr != BZ_OK) goto errhandler;
    if (verbosity >= 2) fprintf ( stderr, "\n" );

    while (1)
    {
        if (myfeof(stream)) break;
        nIbuf = fread ( ibuf, sizeof(uint8_t), 5000, stream );
        if (ferror(stream)) goto errhandler_io;
        if (nIbuf > 0) BZ2_bzWrite ( &bzerr, bzf, (void*)ibuf, nIbuf );
        if (bzerr != BZ_OK) goto errhandler;
    }

    BZ2_bzWriteClose64(&bzerr, bzf, 0, &nbytes_in_lo32, &nbytes_in_hi32,
                        &nbytes_out_lo32, &nbytes_out_hi32);

    if (bzerr != BZ_OK) goto errhandler;
    if (ferror(zStream)) goto errhandler_io;
    ret = fflush ( zStream );
    if (ret == EOF) goto errhandler_io;

    if (zStream != stdout)
    {
        ret = fclose(zStream);
        outputHandleJustInCase = NULL;
        if (ret == EOF) goto errhandler_io;
    }

    outputHandleJustInCase = NULL;
    if (ferror(stream)) goto errhandler_io;
    ret = fclose ( stream );
    if (ret == EOF) goto errhandler_io;

   if (verbosity >= 1) {
      if (nbytes_in_lo32 == 0 && nbytes_in_hi32 == 0) {
	 fprintf ( stderr, " no data compressed.\n");
      } else {
	 char   buf_nin[32], buf_nout[32];
	 UInt64 nbytes_in,   nbytes_out;
	 double nbytes_in_d, nbytes_out_d;
	 uInt64_from_UInt32s ( &nbytes_in, 
			       nbytes_in_lo32, nbytes_in_hi32 );
	 uInt64_from_UInt32s ( &nbytes_out, 
			       nbytes_out_lo32, nbytes_out_hi32 );
	 nbytes_in_d  = uInt64_to_double ( &nbytes_in );
	 nbytes_out_d = uInt64_to_double ( &nbytes_out );
	 uInt64_toAscii ( buf_nin, &nbytes_in );
	 uInt64_toAscii ( buf_nout, &nbytes_out );
	 fprintf ( stderr, "%6.3f:1, %6.3f bits/byte, "
		   "%5.2f%% saved, %s in, %s out.\n",
		   nbytes_in_d / nbytes_out_d,
		   (8.0 * nbytes_out_d) / nbytes_in_d,
		   100.0 * (1.0 - nbytes_out_d / nbytes_in_d),
		   buf_nin,
		   buf_nout
		 );
      }
   }

   return;

   errhandler:
   BZ2_bzWriteClose64 ( &bzerr_dummy, bzf, 1, 
                        &nbytes_in_lo32, &nbytes_in_hi32,
                        &nbytes_out_lo32, &nbytes_out_hi32 );
   switch (bzerr) {
      case BZ_CONFIG_ERROR:
         configError(); break;
      case BZ_MEM_ERROR:
         outOfMemory (); break;
      case BZ_IO_ERROR:
         errhandler_io:
         ioError(); break;
      default:
         panic ( "compress:unexpected error" );
   }

    panic("compress:end");
}

uint8_t AppBzip2::uncompressStream(FILE *zStream, FILE *stream)
{
    BZFILE* bzf = NULL;
    int32_t bzerr, bzerr_dummy, ret, nread, streamNo, i;
    uint8_t obuf[5000];
    uint8_t unused[BZ_MAX_UNUSED];
    int32_t nUnused;
    uint8_t *unusedTmp;
    nUnused = 0;
    streamNo = 0;
    if (ferror(stream)) goto errhandler_io;
    if (ferror(zStream)) goto errhandler_io;

    while (1) {

      bzf = BZ2_bzReadOpen ( 
               &bzerr, zStream, verbosity, 
               (int)smallMode, unused, nUnused
            );
      if (bzf == NULL || bzerr != BZ_OK) goto errhandler;
      streamNo++;

      while (bzerr == BZ_OK) {
         nread = BZ2_bzRead ( &bzerr, bzf, obuf, 5000 );
         if (bzerr == BZ_DATA_ERROR_MAGIC) goto trycat;
         if ((bzerr == BZ_OK || bzerr == BZ_STREAM_END) && nread > 0)
            fwrite ( obuf, sizeof(uint8_t), nread, stream );
         if (ferror(stream)) goto errhandler_io;
      }
      if (bzerr != BZ_STREAM_END) goto errhandler;

      BZ2_bzReadGetUnused ( &bzerr, bzf, (void**)(&unusedTmp), &nUnused );
      if (bzerr != BZ_OK) panic ( "decompress:bzReadGetUnused" );

      for (i = 0; i < nUnused; i++) unused[i] = unusedTmp[i];

      BZ2_bzReadClose ( &bzerr, bzf );
      if (bzerr != BZ_OK) panic ( "decompress:bzReadGetUnused" );

      if (nUnused == 0 && myfeof(zStream)) break;
   }

   closeok:
   if (ferror(zStream)) goto errhandler_io;
   ret = fclose ( zStream );
   if (ret == EOF) goto errhandler_io;

   if (ferror(stream)) goto errhandler_io;
   ret = fflush ( stream );
   if (ret != 0) goto errhandler_io;
   if (stream != stdout) {
      ret = fclose ( stream );
      outputHandleJustInCase = NULL;
      if (ret == EOF) goto errhandler_io;
   }
   outputHandleJustInCase = NULL;
   if (verbosity >= 2) fprintf ( stderr, "\n    " );
   return 1;

   trycat: 
   if (forceOverwrite) {
      rewind(zStream);
      while (1) {
      	 if (myfeof(zStream)) break;
      	 nread = fread ( obuf, sizeof(uint8_t), 5000, zStream );
      	 if (ferror(zStream)) goto errhandler_io;
      	 if (nread > 0) fwrite ( obuf, sizeof(uint8_t), nread, stream );
      	 if (ferror(stream)) goto errhandler_io;
      }
      goto closeok;
   }
  
   errhandler:
   BZ2_bzReadClose ( &bzerr_dummy, bzf );
   switch (bzerr) {
      case BZ_CONFIG_ERROR:
         configError(); break;
      case BZ_IO_ERROR:
         errhandler_io:
         ioError(); break;
      case BZ_DATA_ERROR:
         crcError();
      case BZ_MEM_ERROR:
         outOfMemory();
      case BZ_UNEXPECTED_EOF:
         compressedStreamEOF();
      case BZ_DATA_ERROR_MAGIC:
         if (zStream != stdin) fclose(zStream);
         if (stream != stdout) fclose(stream);
         if (streamNo == 1) {
            return 0;
         } else {
            if (noisy)
            fprintf ( stderr, 
                      "\n%s: %s: trailing garbage after EOF ignored\n",
                      progName, inName );
            return 1;       
         }
      default:
         panic ( "decompress:unexpected error" );
   }

   panic ( "decompress:end" );
   return 1;
}

uint8_t AppBzip2::testStream(FILE *zStream)
{
    BZFILE *bzf = NULL;
    int32_t bzerr, bzerr_dummy, ret, streamNo, i;
    uint8_t obuf[5000];
    uint8_t unused[BZ_MAX_UNUSED];
    int32_t nUnused;
    uint8_t *unusedTmp;
    nUnused = 0;
    streamNo = 0;
    if (ferror(zStream)) goto errhandler_io;

    while (1)
    {
        bzf = BZ2_bzReadOpen(&bzerr, zStream, verbosity, (int)smallMode, unused, nUnused);
        if (bzf == NULL || bzerr != BZ_OK) goto errhandler;
        streamNo++;

        while (bzerr == BZ_OK)
        {
            BZ2_bzRead ( &bzerr, bzf, obuf, 5000 );
            if (bzerr == BZ_DATA_ERROR_MAGIC) goto errhandler;
        }

        if (bzerr != BZ_STREAM_END) goto errhandler;
        BZ2_bzReadGetUnused(&bzerr, bzf, (void**)(&unusedTmp), &nUnused);
      if (bzerr != BZ_OK) panic ( "test:bzReadGetUnused" );

      for (i = 0; i < nUnused; i++) unused[i] = unusedTmp[i];

      BZ2_bzReadClose ( &bzerr, bzf );
      if (bzerr != BZ_OK) panic ( "test:bzReadGetUnused" );
      if (nUnused == 0 && myfeof(zStream)) break;

   }

   if (ferror(zStream)) goto errhandler_io;
   ret = fclose ( zStream );
   if (ret == EOF) goto errhandler_io;

   if (verbosity >= 2) fprintf ( stderr, "\n    " );
   return 1;

   errhandler:
   BZ2_bzReadClose ( &bzerr_dummy, bzf );
   if (verbosity == 0) 
      fprintf ( stderr, "%s: %s: ", progName, inName );
   switch (bzerr) {
      case BZ_CONFIG_ERROR:
         configError(); break;
      case BZ_IO_ERROR:
         errhandler_io:
         ioError(); break;
      case BZ_DATA_ERROR:
         fprintf ( stderr,
                   "data integrity (CRC) error in data\n" );
         return 0;
      case BZ_MEM_ERROR:
         outOfMemory();
      case BZ_UNEXPECTED_EOF:
         fprintf ( stderr,
                   "file ends unexpectedly\n" );
         return 0;
      case BZ_DATA_ERROR_MAGIC:
         if (zStream != stdin) fclose(zStream);
         if (streamNo == 1) {
          fprintf ( stderr, 
                    "bad magic number (file not created by bzip2)\n" );
            return 0;
         } else {
            if (noisy)
            fprintf ( stderr, 
                      "trailing garbage after EOF ignored\n" );
            return 1;       
         }
      default:
         panic ( "test:unexpected error" );
   }

   panic ( "test:end" );
   return 1;
}

void AppBzip2::setExit(int32_t v)
{
    if (v > exitValue) exitValue = v;
}

void AppBzip2::cadvise()
{
   if (noisy)
   fprintf (
      stderr,
      "\nIt is possible that the compressed file(s) have become corrupted.\n"
        "You can use the -tvv option to test integrity of such files.\n\n"
        "You can use the `bzip2recover' program to attempt to recover\n"
        "data from undamaged sections of corrupted files.\n\n"
    );
}

void AppBzip2::showFileNames()
{
    if (noisy)
        fprintf(stderr, "\tInput file = %s, output file = %s\n", inName, outName);
}

void AppBzip2::cleanUpAndFail(int32_t ec)
{
    int retVal;
    struct stat statBuf;

    if (srcMode == SM_F2F && opMode != OM_TEST && deleteOutputOnInterrupt )
    {
        retVal = stat ( inName, &statBuf );

        if (retVal == 0)
        {
            if (noisy)
            fprintf ( stderr, 
                      "%s: Deleting output file %s, if it exists.\n",
                      progName, outName );
         if (outputHandleJustInCase != NULL)
            fclose ( outputHandleJustInCase );
         retVal = remove ( outName );
         if (retVal != 0)
            fprintf ( stderr,
                      "%s: WARNING: deletion of output file "
                      "(apparently) failed.\n",
                      progName );
      } else {
         fprintf ( stderr,
                   "%s: WARNING: deletion of output file suppressed\n",
                    progName );
         fprintf ( stderr,
                   "%s:    since input file no longer exists.  Output file\n",
                   progName );
         fprintf ( stderr,
                   "%s:    `%s' may be incomplete.\n",
                   progName, outName );
         fprintf ( stderr, 
                   "%s:    I suggest doing an integrity test (bzip2 -tv)"
                   " of it.\n",
                   progName );
      }
   }

    if (noisy && numFileNames > 0 && numFilesProcessed < numFileNames)
    {
        fprintf ( stderr, 
                "%s: WARNING: some files have not been processed:\n"
                "%s:    %d specified on command line, %d not processed yet.\n\n",
                progName, progName,
                numFileNames, numFileNames - numFilesProcessed );
    }
    setExit(ec);
    //exit(exitValue);
    throw "exitValue";
}

void AppBzip2::panic(const char *s)
{
    fprintf(stderr,
             "\n%s: PANIC -- internal consistency error:\n"
             "\t%s\n"
             "\tThis is a BUG.  Please report it to me at:\n"
             "\tjseward@acm.org\n",
             progName, s );
    showFileNames();
    cleanUpAndFail( 3 );
}

void AppBzip2::crcError()
{
    fprintf(stderr, "\n%s: Data integrity error when decompressing.\n", progName);
    showFileNames();
    cadvise();
    cleanUpAndFail( 2 );
}

void AppBzip2::compressedStreamEOF()
{
    if (noisy)
    {
        fprintf(stderr,
	        "\n%s: Compressed file ends unexpectedly;\n\t"
	        "perhaps it is corrupted?  *Possible* reason follows.\n",
	        progName);

        perror(progName);
        showFileNames();
        cadvise();
    }
    cleanUpAndFail( 2 );
}

void AppBzip2::ioError()
{
    fprintf(stderr,
             "\n%s: I/O or other error, bailing out.  "
             "Possible reason follows.\n",
             progName);
    perror(progName);
    showFileNames();
    cleanUpAndFail(1);
}

#if 0
static 
void mySignalCatcher(int n)
{
   fprintf ( stderr,
             "\n%s: Control-C or similar caught, quitting.\n",
             progName );
   cleanUpAndFail(1);
}
#endif

#if 0
static 
void mySIGSEGVorSIGBUScatcher(int n)
{
   if (opMode == OM_Z)
      fprintf ( 
      stderr,
      "\n%s: Caught a SIGSEGV or SIGBUS whilst compressing.\n"
      "\n"
      "   Possible causes are (most likely first):\n"
      "   (1) This computer has unreliable memory or cache hardware\n"
      "       (a surprisingly common problem; try a different machine.)\n"
      "   (2) A bug in the compiler used to create this executable\n"
      "       (unlikely, if you didn't compile bzip2 yourself.)\n"
      "   (3) A real bug in bzip2 -- I hope this should never be the case.\n"
      "   The user's manual, Section 4.3, has more info on (1) and (2).\n"
      "   \n"
      "   If you suspect this is a bug in bzip2, or are unsure about (1)\n"
      "   or (2), feel free to report it to me at: jseward@acm.org.\n"
      "   Section 4.3 of the user's manual describes the info a useful\n"
      "   bug report should have.  If the manual is available on your\n"
      "   system, please try and read it before mailing me.  If you don't\n"
      "   have the manual or can't be bothered to read it, mail me anyway.\n"
      "\n",
      progName );
      else
      fprintf ( 
      stderr,
      "\n%s: Caught a SIGSEGV or SIGBUS whilst decompressing.\n"
      "\n"
      "   Possible causes are (most likely first):\n"
      "   (1) The compressed data is corrupted, and bzip2's usual checks\n"
      "       failed to detect this.  Try bzip2 -tvv my_file.bz2.\n"
      "   (2) This computer has unreliable memory or cache hardware\n"
      "       (a surprisingly common problem; try a different machine.)\n"
      "   (3) A bug in the compiler used to create this executable\n"
      "       (unlikely, if you didn't compile bzip2 yourself.)\n"
      "   (4) A real bug in bzip2 -- I hope this should never be the case.\n"
      "   The user's manual, Section 4.3, has more info on (2) and (3).\n"
      "   \n"
      "   If you suspect this is a bug in bzip2, or are unsure about (2)\n"
      "   or (3), feel free to report it to me at: jseward@acm.org.\n"
      "   Section 4.3 of the user's manual describes the info a useful\n"
      "   bug report should have.  If the manual is available on your\n"
      "   system, please try and read it before mailing me.  If you don't\n"
      "   have the manual or can't be bothered to read it, mail me anyway.\n"
      "\n",
      progName );

   showFileNames();
   if (opMode == OM_Z)
      cleanUpAndFail( 3 ); else
      { cadvise(); cleanUpAndFail( 2 ); }
}
#endif

void AppBzip2::outOfMemory()
{
    fprintf(stderr, "\n%s: couldn't allocate enough memory\n", progName );
    showFileNames();
    cleanUpAndFail(1);
}

void AppBzip2::configError()
{
   fprintf ( stderr,
             "bzip2: I'm not configured correctly for this platform!\n"
             "\tI require Int32, Int16 and Char to have sizes\n"
             "\tof 4, 2 and 1 bytes to run properly, and they don't.\n"
             "\tProbably you can fix this by defining them correctly,\n"
             "\tand recompiling.  Bye!\n" );
   setExit(3);
   //exit(exitValue);
    throw "exitValue";
}


void AppBzip2::pad(char *s)
{
   int32_t i;
   if ( (int32_t)strlen(s) >= longestFileName ) return;
   for (i = 1; i <= longestFileName - (int32_t)strlen(s); i++)
      fprintf ( stderr, " " );
}

void AppBzip2::copyFileName(char* to, const char* from ) 
{
   if ( strlen(from) > FILE_NAME_LEN-10 )  {
      fprintf (
         stderr,
         "bzip2: file name\n`%s'\n"
         "is suspiciously (more than %d chars) long.\n"
         "Try using a reasonable file name instead.  Sorry! :-)\n",
         from, FILE_NAME_LEN-10
      );
      setExit(1);
        throw "exitValue";
      //exit(exitValue);
   }

  strncpy(to,from,FILE_NAME_LEN-10);
  to[FILE_NAME_LEN-10]='\0';
}

uint8_t AppBzip2::fileExists(char* name)
{
    FILE *tmp = fopen ( name, "rb" );
    uint8_t exists = (tmp != NULL);
    if (tmp != NULL) fclose ( tmp );
    return exists;
}

FILE *AppBzip2::fopen_output_safely(char* name, const char* mode)
{
#if 0
   FILE*     fp;
   int fh;
   fh = open(name, O_WRONLY|O_CREAT|O_EXCL, S_IWUSR|S_IRUSR);
   if (fh == -1) return NULL;
   fp = fdopen(fh, mode);
   if (fp == NULL) close(fh);
   return fp;
#else
    return fopen(name, mode);
#endif
}

uint8_t AppBzip2::notAStandardFile(char* name)
{
    int i;
    struct stat statBuf;
    i = lstat ( name, &statBuf );
    if (i != 0) return 1;
    if (S_ISREG(statBuf.st_mode)) return 0;
    return 1;
}

int32_t AppBzip2::countHardLinks(char *name)
{  
    int i;
    struct stat statBuf;
    i = lstat ( name, &statBuf );
    if (i != 0) return 0;
    return (statBuf.st_nlink - 1);
}



void AppBzip2::ERROR_IF_NOT_ZERO(int i)
{
    if (i != 0)
        ioError();
}

void AppBzip2::saveInputFileMetaInfo(char *srcName)
{
   int retVal;
   retVal = stat( srcName, &fileMetaInfo );
   ERROR_IF_NOT_ZERO ( retVal );
}

#if 0
void AppBzip2::applySavedMetaInfoToOutputFile(char *dstName)
{
    int retVal;
    struct utimbuf uTimBuf;
    uTimBuf.actime = fileMetaInfo.st_atime;
    uTimBuf.modtime = fileMetaInfo.st_mtime;
    retVal = chmod ( dstName, fileMetaInfo.st_mode );
    ERROR_IF_NOT_ZERO ( retVal );
    retVal = utime ( dstName, &uTimBuf );
    ERROR_IF_NOT_ZERO ( retVal );
    retVal = chown ( dstName, fileMetaInfo.st_uid, fileMetaInfo.st_gid );
}
#endif

uint8_t AppBzip2::containsDubiousChars(char *name)
{
    return 0;
}

uint8_t AppBzip2::hasSuffix(const char* s, const char* suffix)
{
    int32_t ns = strlen(s);
    int32_t nx = strlen(suffix);
    if (ns < nx) return 0;
    if (strcmp(s + ns - nx, suffix) == 0) return 1;
    return 0;
}

uint8_t AppBzip2::mapSuffix(char* name, const char* oldSuffix, const char* newSuffix)
{
    if (!hasSuffix(name,oldSuffix)) return 0;
    name[strlen(name)-strlen(oldSuffix)] = 0;
    strcat(name, newSuffix);
    return 1;
}

void AppBzip2::compress(char *name)
{
    FILE *inStr;
    FILE *outStr;
    int32_t n, i;
    struct stat statBuf;
    deleteOutputOnInterrupt = 0;

   if (name == NULL && srcMode != SM_I2O)
      panic ( "compress: bad modes\n" );

   switch (srcMode) {
      case SM_I2O: 
         copyFileName ( inName, "(stdin)" );
         copyFileName ( outName, "(stdout)" ); 
         break;
      case SM_F2F: 
         copyFileName ( inName, name );
         copyFileName ( outName, name );
         strcat ( outName, ".bz2" ); 
         break;
      case SM_F2O: 
         copyFileName ( inName, name );
         copyFileName ( outName, "(stdout)" ); 
         break;
   }

   if ( srcMode != SM_I2O && containsDubiousChars ( inName ) ) {
      if (noisy)
      fprintf ( stderr, "%s: There are no files matching `%s'.\n",
                progName, inName );
      setExit(1);
      return;
   }
    if ( srcMode != SM_I2O && !fileExists ( inName ) ) {
        fprintf(stderr, "%s: Can't open input file %s.\n", progName, inName);
        setExit(1);
        return;
    }
   for (i = 0; i < BZ_N_SUFFIX_PAIRS; i++) {
      if (hasSuffix(inName, zSuffix[i])) {
         if (noisy)
         fprintf ( stderr, 
                   "%s: Input file %s already has %s suffix.\n",
                   progName, inName, zSuffix[i] );
         setExit(1);
         return;
      }
   }
   if ( srcMode == SM_F2F || srcMode == SM_F2O ) {
      stat(inName, &statBuf);
      if ( S_ISDIR(statBuf.st_mode) ) {
         fprintf( stderr,
                  "%s: Input file %s is a directory.\n",
                  progName,inName);
         setExit(1);
         return;
      }
   }
   if ( srcMode == SM_F2F && !forceOverwrite && notAStandardFile ( inName )) {
      if (noisy)
      fprintf ( stderr, "%s: Input file %s is not a normal file.\n",
                progName, inName );
      setExit(1);
      return;
   }
   if ( srcMode == SM_F2F && fileExists ( outName ) ) {
      if (forceOverwrite) {
	 remove(outName);
      } else {
	 fprintf ( stderr, "%s: Output file %s already exists.\n",
		   progName, outName );
	 setExit(1);
	 return;
      }
   }
   if ( srcMode == SM_F2F && !forceOverwrite &&
        (n=countHardLinks ( inName )) > 0) {
      fprintf ( stderr, "%s: Input file %s has %d other link%s.\n",
                progName, inName, n, n > 1 ? "s" : "" );
      setExit(1);
      return;
   }

   if ( srcMode == SM_F2F ) {
      saveInputFileMetaInfo ( inName );
   }

   switch ( srcMode ) {

      case SM_I2O:
         inStr = stdin;
         outStr = stdout;
#if 0
         if ( isatty ( fileno ( stdout ) ) ) {
            fprintf ( stderr,
                      "%s: I won't write compressed data to a terminal.\n",
                      progName );
            fprintf ( stderr, "%s: For help, type: `%s --help'.\n",
                              progName, progName );
            setExit(1);
            return;
         };
#endif
         break;

      case SM_F2O:
         inStr = fopen ( inName, "rb" );
         outStr = stdout;
#if 0
         if ( isatty ( fileno ( stdout ) ) ) {
            fprintf ( stderr,
                      "%s: I won't write compressed data to a terminal.\n",
                      progName );
            fprintf ( stderr, "%s: For help, type: `%s --help'.\n",
                              progName, progName );
            if ( inStr != NULL ) fclose ( inStr );
            setExit(1);
            return;
         };
#endif
         if (inStr == NULL)
            {
                fprintf(stderr, "%s: Can't open input file %s.\n", progName, inName);
                setExit(1);
                return;
            };
         break;

      case SM_F2F:
         inStr = fopen ( inName, "rb" );
         outStr = fopen_output_safely ( outName, "wb" );
         if ( outStr == NULL) {
            fprintf(stderr, "%s: Can't create output file %s.\n", progName, outName);
            if ( inStr != NULL ) fclose ( inStr );
            setExit(1);
            return;
         }
         if ( inStr == NULL ) {
            fprintf(stderr, "%s: Can't open input file %s.\n", progName, inName);
            if ( outStr != NULL ) fclose ( outStr );
            setExit(1);
            return;
         };
         break;

      default:
         panic ( "compress: bad srcMode" );
         break;
   }

   if (verbosity >= 1) {
      fprintf ( stderr,  "  %s: ", inName );
      pad ( inName );
      fflush ( stderr );
   }

   outputHandleJustInCase = outStr;
   deleteOutputOnInterrupt = 1;
   compressStream ( inStr, outStr );
   outputHandleJustInCase = NULL;

   if ( srcMode == SM_F2F ) {
      //applySavedMetaInfoToOutputFile ( outName );
      deleteOutputOnInterrupt = 0;
      if ( !keepInputFiles ) {
         int retVal = remove ( inName );
         ERROR_IF_NOT_ZERO ( retVal );
      }
   }

   deleteOutputOnInterrupt = 0;
}

void AppBzip2::uncompress(char *name)
{
    FILE *inStr;
    FILE *outStr;
    int32_t n, i;
    uint8_t magicNumberOK;
    uint8_t cantGuess;
    struct stat statBuf;
    deleteOutputOnInterrupt = 0;

    if (name == NULL && srcMode != SM_I2O)
        panic("uncompress: bad modes\n");

    cantGuess = 0;

    switch (srcMode)
    {
        case SM_I2O: 
            copyFileName(inName, "(stdin)");
            copyFileName(outName, "(stdout)");
            break;
        case SM_F2F: 
            copyFileName(inName, name);
            copyFileName(outName, name);
         for (i = 0; i < BZ_N_SUFFIX_PAIRS; i++)
            if (mapSuffix(outName,zSuffix[i],unzSuffix[i]))
               goto zzz; 
         cantGuess = 1;
         strcat ( outName, ".out" );
         break;
      case SM_F2O: 
         copyFileName ( inName, name );
         copyFileName ( outName, "(stdout)" ); 
         break;
   }

   zzz:
   if ( srcMode != SM_I2O && containsDubiousChars ( inName ) ) {
      if (noisy)
      fprintf ( stderr, "%s: There are no files matching `%s'.\n",
                progName, inName );
      setExit(1);
      return;
   }
   if ( srcMode != SM_I2O && !fileExists ( inName ) ) {
      fprintf(stderr, "%s: Can't open input file %s.\n",
                progName, inName);
      setExit(1);
      return;
   }
   if ( srcMode == SM_F2F || srcMode == SM_F2O ) {
      stat(inName, &statBuf);
      if ( S_ISDIR(statBuf.st_mode) ) {
         fprintf( stderr,
                  "%s: Input file %s is a directory.\n",
                  progName,inName);
         setExit(1);
         return;
      }
   }
   if ( srcMode == SM_F2F && !forceOverwrite && notAStandardFile ( inName )) {
      if (noisy)
      fprintf ( stderr, "%s: Input file %s is not a normal file.\n",
                progName, inName );
      setExit(1);
      return;
   }
   if (cantGuess) {
      if (noisy)
      fprintf ( stderr, 
                "%s: Can't guess original name for %s -- using %s\n",
                progName, inName, outName );
   }   
   if ( srcMode == SM_F2F && fileExists ( outName ) ) {
      if (forceOverwrite) {
	remove(outName);
      } else {
        fprintf ( stderr, "%s: Output file %s already exists.\n",
                  progName, outName );
        setExit(1);
        return;
      }
   }
   if ( srcMode == SM_F2F && !forceOverwrite &&
        (n=countHardLinks ( inName ) ) > 0) {
      fprintf ( stderr, "%s: Input file %s has %d other link%s.\n",
                progName, inName, n, n > 1 ? "s" : "" );
      setExit(1);
      return;
   }

    if (srcMode == SM_F2F)
    {
        saveInputFileMetaInfo(inName);
    }

    switch (srcMode)
    {
    case SM_I2O:
         inStr = stdin;
         outStr = stdout;
#if 0
         if ( isatty ( fileno ( stdin ) ) ) {
            fprintf ( stderr,
                      "%s: I won't read compressed data from a terminal.\n",
                      progName );
            fprintf ( stderr, "%s: For help, type: `%s --help'.\n",
                              progName, progName );
            setExit(1);
            return;
         };
#endif
         break;

      case SM_F2O:
         inStr = fopen ( inName, "rb" );
         outStr = stdout;
         if ( inStr == NULL ) {
            fprintf ( stderr, "%s: Can't open input file %s.\n",
                      progName, inName);
            if ( inStr != NULL ) fclose ( inStr );
            setExit(1);
            return;
         };
         break;

      case SM_F2F:
         inStr = fopen ( inName, "rb" );
         outStr = fopen_output_safely ( outName, "wb" );
         if ( outStr == NULL) {
            fprintf ( stderr, "%s: Can't create output file %s.\n",
                      progName, outName);
            if ( inStr != NULL ) fclose ( inStr );
            setExit(1);
            return;
         }
         if ( inStr == NULL ) {
            fprintf ( stderr, "%s: Can't open input file %s.\n",
                      progName, inName);
            if ( outStr != NULL ) fclose ( outStr );
            setExit(1);
            return;
         };
         break;

      default:
         panic ( "uncompress: bad srcMode" );
         break;
   }

   if (verbosity >= 1) {
      fprintf ( stderr, "  %s: ", inName );
      pad ( inName );
      fflush ( stderr );
   }

   outputHandleJustInCase = outStr;
   deleteOutputOnInterrupt = 1;
   magicNumberOK = uncompressStream ( inStr, outStr );
   outputHandleJustInCase = NULL;

   if ( magicNumberOK ) {
      if ( srcMode == SM_F2F ) {
         //applySavedMetaInfoToOutputFile ( outName );
         deleteOutputOnInterrupt = 0;
         if ( !keepInputFiles ) {
            int retVal = remove ( inName );
            ERROR_IF_NOT_ZERO ( retVal );
         }
      }
   } else {
      unzFailsExist = 1;
      deleteOutputOnInterrupt = 0;
      if ( srcMode == SM_F2F ) {
         int retVal = remove ( outName );
         ERROR_IF_NOT_ZERO ( retVal );
      }
   }
   deleteOutputOnInterrupt = 0;

   if ( magicNumberOK ) {
      if (verbosity >= 1)
         fprintf ( stderr, "done\n" );
   } else {
      setExit(2);
      if (verbosity >= 1)
         fprintf ( stderr, "not a bzip2 file.\n" ); else
         fprintf ( stderr,
                   "%s: %s is not a bzip2 file.\n",
                   progName, inName );
   }

}

void AppBzip2::testf(char *name)
{
    FILE *inStr;
    uint8_t allOK;
    struct stat statBuf;
    deleteOutputOnInterrupt = 0;

    if (name == NULL && srcMode != SM_I2O)
        panic("testf: bad modes\n");

    copyFileName(outName, "(none)");

    switch (srcMode)
    {
    case SM_I2O: copyFileName ( inName, "(stdin)" ); break;
    case SM_F2F: copyFileName ( inName, name ); break;
    case SM_F2O: copyFileName ( inName, name ); break;
    }

    if (srcMode != SM_I2O && containsDubiousChars(inName))
    {
        if (noisy)
            fprintf(stderr, "%s: There are no files matching `%s'.\n", progName, inName);

        setExit(1);
        return;
    }

    if (srcMode != SM_I2O && !fileExists(inName))
    {
        fprintf(stderr, "%s: Can't open input %s.\n", progName, inName);
        setExit(1);
        return;
    }

    if ( srcMode != SM_I2O )
    {
        stat(inName, &statBuf);

        if ( S_ISDIR(statBuf.st_mode) )
        {
            fprintf(stderr, "%s: Input file %s is a directory.\n", progName,inName);
            setExit(1);
            return;
        }
    }

    switch (srcMode)
    {
    case SM_I2O:
#if 0
         if ( isatty ( fileno ( stdin ) ) ) {
            fprintf ( stderr,
                      "%s: I won't read compressed data from a terminal.\n",
                      progName );
            fprintf ( stderr, "%s: For help, type: `%s --help'.\n",
                              progName, progName );
            setExit(1);
            return;
         };
#endif
         inStr = stdin;
         break;

      case SM_F2O: case SM_F2F:
         inStr = fopen ( inName, "rb" );
         if ( inStr == NULL ) {
            fprintf ( stderr, "%s: Can't open input file %s.\n",
                      progName, inName);
            setExit(1);
            return;
         };
         break;

      default:
         panic ( "testf: bad srcMode" );
         break;
   }

   if (verbosity >= 1) {
      fprintf ( stderr, "  %s: ", inName );
      pad ( inName );
      fflush ( stderr );
   }

    outputHandleJustInCase = NULL;
    allOK = testStream(inStr);
    if (allOK && verbosity >= 1) fprintf(stderr, "ok\n");
    if (!allOK) testFailsExist = 1;
}

void AppBzip2::redundant(char* flag)
{
    fprintf(stderr, "%s: %s is redundant in versions 0.9.5 and above\n", progName, flag);
}

#if 0
static void *myMalloc(int32_t n)
{
    return malloc(n);
   void* p;

   p = malloc ( (size_t)n );
   if (p == NULL) outOfMemory ();
   return p;
}
#endif

Cell *AppBzip2::mkCell()
{
    Cell *c;
    c = new Cell;
    //c = (Cell*)malloc(sizeof(Cell));
    c->name = NULL;
    c->link = NULL;
    return c;
}

Cell *AppBzip2::snocString(Cell *root, char *name)
{
    if (root == NULL)
    {
        Cell *tmp = mkCell();
        //tmp->name = (char*)malloc(5 + strlen(name));
        tmp->name = new char[5 + strlen(name)];
        strcpy ( tmp->name, name );
        return tmp;
    }
    else
    {
        Cell *tmp = root;
        while (tmp->link != NULL) tmp = tmp->link;
        tmp->link = snocString ( tmp->link, name );
        return root;
    }
}

void AppBzip2::addFlagsFromEnvVar(Cell **argList, const char *varName)
{
   int32_t i, j, k;
   char *envbase, *p;

   envbase = getenv(varName);
   if (envbase != NULL) {
      p = envbase;
      i = 0;
      while (1) {
         if (p[i] == 0) break;
         p += i;
         i = 0;
         while (isspace((int32_t)(p[0]))) p++;
         while (p[i] != 0 && !isspace((int32_t)(p[i]))) i++;
         if (i > 0) {
            k = i; if (k > FILE_NAME_LEN-10) k = FILE_NAME_LEN-10;
            for (j = 0; j < k; j++) tmpName[j] = p[j];
            tmpName[k] = 0;
            *argList = snocString(*argList, tmpName);
         }
      }
   }
}

void AppBzip2::usage(char *fullProgName)
{
   fprintf (
      stderr,
      "bzip2, a block-sorting file compressor.  "
      "Version %s.\n"
      "\n   usage: %s [flags and input files in any order]\n"
      "\n"
      "   -h --help           print this message\n"
      "   -d --decompress     force decompression\n"
      "   -z --compress       force compression\n"
      "   -k --keep           keep (don't delete) input files\n"
      "   -f --force          overwrite existing output files\n"
      "   -t --test           test compressed file integrity\n"
      "   -c --stdout         output to standard out\n"
      "   -q --quiet          suppress noncritical error messages\n"
      "   -v --verbose        be verbose (a 2nd -v gives more)\n"
      "   -L --license        display software version & license\n"
      "   -V --version        display software version & license\n"
      "   -s --small          use less memory (at most 2500k)\n"
      "   -1 .. -9            set block size to 100k .. 900k\n"
      "   --fast              alias for -1\n"
      "   --best              alias for -9\n"
      "\n"
      "   If invoked as `bzip2', default action is to compress.\n"
      "              as `bunzip2',  default action is to decompress.\n"
      "              as `bzcat', default action is to decompress to stdout.\n"
      "\n"
      "   If no file names are given, bzip2 compresses or decompresses\n"
      "   from standard input to standard output.  You can combine\n"
      "   short flags, so `-v -4' means the same as -v4 or -4v, &c.\n"
      "\n",

      BZ2_bzlibVersion(),
      fullProgName
   );
}

void AppBzip2::license()
{
    fprintf(stderr,

    "bzip2, a block-sorting file compressor.  "
    "Version %s.\n"
    "   \n"
    "   Copyright (C) 1996-2002 by Julian Seward.\n"
    "   \n"
    "   This program is free software; you can redistribute it and/or modify\n"
    "   it under the terms set out in the LICENSE file, which is included\n"
    "   in the bzip2-1.0 source distribution.\n"
    "   \n"
    "   This program is distributed in the hope that it will be useful,\n"
    "   but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "   LICENSE file for more details.\n"
    "   \n",
    BZ2_bzlibVersion());
}

int AppBzip2::run(int argc, char **argv)
{
    int32_t i, j;
    char *tmp;
    Cell *argList;
    Cell *aa;
    uint8_t decode;

    if (sizeof(int32_t) != 4 || sizeof(uint32_t) != 4  ||
       sizeof(int16_t) != 2 || sizeof(uint16_t) != 2  ||
       sizeof(char)  != 1 || sizeof(uint8_t)  != 1)
    {
        configError();
    }

    outputHandleJustInCase = NULL;
    smallMode      = 0;
    keepInputFiles = 0;
    forceOverwrite = 0;
    noisy          = 1;
    verbosity      = 0;
    blockSize100k  = 9;
    testFailsExist = 0;
    unzFailsExist  = 0;
    numFileNames   = 0;
    numFilesProcessed       = 0;
    workFactor              = 30;
    deleteOutputOnInterrupt = 0;
    exitValue               = 0;
    i = j = 0;
#if 0
    signal (SIGSEGV, mySIGSEGVorSIGBUScatcher);
    signal (SIGBUS,  mySIGSEGVorSIGBUScatcher);
#endif
    copyFileName ( inName,  "(none)" );
    copyFileName ( outName, "(none)" );
    copyFileName ( progNameReally, argv[0] );
    progName = &progNameReally[0];

    for (tmp = &progNameReally[0]; *tmp != '\0'; tmp++)
        if (*tmp == '/') progName = tmp + 1;

    argList = NULL;
    addFlagsFromEnvVar(&argList,  "BZIP2");
    addFlagsFromEnvVar(&argList,  "BZIP");

    for (i = 1; i <= argc-1; i++)
        argList = snocString(argList, argv[i]);

    longestFileName = 7;
    numFileNames = 0;
    decode = 1;

    for (aa = argList; aa != NULL; aa = aa->link)
    {
        if (strcmp(aa->name, "--") == 0)
        { decode = 0; continue; }

        if (aa->name[0] == '-' && decode) continue;
        numFileNames++;

        if (longestFileName < (int32_t)strlen(aa->name) )
            longestFileName = (int32_t)strlen(aa->name);
    }

    if (numFileNames == 0)
        srcMode = SM_I2O; else srcMode = SM_F2F;

    opMode = OM_Z;

    if ((strstr ( progName, "unzip" ) != 0) ||
        (strstr ( progName, "UNZIP" ) != 0) )
      opMode = OM_UNZ;

   if ( (strstr ( progName, "z2cat" ) != 0) ||
        (strstr ( progName, "Z2CAT" ) != 0) ||
        (strstr ( progName, "zcat" ) != 0)  ||
        (strstr ( progName, "ZCAT" ) != 0) )  {
      opMode = OM_UNZ;
      srcMode = (numFileNames == 0) ? SM_I2O : SM_F2O;
   }

    for (aa = argList; aa != NULL; aa = aa->link)
    {
        if (strcmp(aa->name, "--") == 0)
            break;

        if (aa->name[0] == '-' && aa->name[1] != '-') {
         for (j = 1; aa->name[j] != '\0'; j++) {
            switch (aa->name[j]) {
               case 'c': srcMode          = SM_F2O; break;
               case 'd': opMode           = OM_UNZ; break;
               case 'z': opMode           = OM_Z; break;
               case 'f': forceOverwrite   = 1; break;
               case 't': opMode           = OM_TEST; break;
               case 'k': keepInputFiles   = 1; break;
               case 's': smallMode        = 1; break;
               case 'q': noisy            = 0; break;
               case '1': blockSize100k    = 1; break;
               case '2': blockSize100k    = 2; break;
               case '3': blockSize100k    = 3; break;
               case '4': blockSize100k    = 4; break;
               case '5': blockSize100k    = 5; break;
               case '6': blockSize100k    = 6; break;
               case '7': blockSize100k    = 7; break;
               case '8': blockSize100k    = 8; break;
               case '9': blockSize100k    = 9; break;
               case 'V':
               case 'L': license();            break;
               case 'v': verbosity++; break;
               case 'h': usage ( progName );
                         //exit ( 0 );
                         return 0;
                         break;
               default:  fprintf ( stderr, "%s: Bad flag `%s'\n",
                                   progName, aa->name );
                         usage ( progName );
                         //exit ( 1 );
                         return 1;
                         break;
            }
         }
      }
   }
   
    for (aa = argList; aa != NULL; aa = aa->link)
    {
      if (strcmp(aa->name, "--") == 0) break;
      if (strcmp(aa->name, "--stdout") == 0)   srcMode          = SM_F2O;  else
      if (strcmp(aa->name, "--decompress") == 0) opMode           = OM_UNZ;  else
      if (strcmp(aa->name, "--compress") == 0)          opMode           = OM_Z;    else
      if (strcmp(aa->name, "--force") == 0)             forceOverwrite   = 1;    else
      if (strcmp(aa->name, "--test") == 0)              opMode           = OM_TEST; else
      if (strcmp(aa->name, "--keep") == 0)              keepInputFiles   = 1;    else
      if (strcmp(aa->name, "--small") == 0)             smallMode        = 1;    else
      if (strcmp(aa->name, "--quiet") == 0)             noisy            = 0;   else
      if (strcmp(aa->name, "--version") == 0)           license();                  else
      if (strcmp(aa->name, "--license") == 0)           license();                  else
      if (strcmp(aa->name, "--exponential") == 0)       workFactor = 1;             else 
      if (strcmp(aa->name, "--repetitive-best") == 0)   redundant(aa->name);        else
      if (strcmp(aa->name, "--repetitive-fast") == 0)   redundant(aa->name);        else
      if (strcmp(aa->name, "--fast") == 0)              blockSize100k = 1;          else
      if (strcmp(aa->name, "--best") == 0)              blockSize100k = 9;          else
      if (strcmp(aa->name, "--verbose") == 0)           verbosity++;                else
      if (strcmp(aa->name, "--help") == 0)              { usage ( progName ); return 0; }
         else
         if (strncmp ( aa->name, "--", 2) == 0) {
            fprintf ( stderr, "%s: Bad flag `%s'\n", progName, aa->name );
            usage ( progName );
            //exit ( 1 );
            return 1;
         }
   }

   if (verbosity > 4) verbosity = 4;
   if (opMode == OM_Z && smallMode && blockSize100k > 2) 
      blockSize100k = 2;

   if (opMode == OM_TEST && srcMode == SM_F2O) {
      fprintf ( stderr, "%s: -c and -t cannot be used together.\n",
                progName );
      //exit ( 1 );
        return 1;
   }

   if (srcMode == SM_F2O && numFileNames == 0)
      srcMode = SM_I2O;

   if (opMode != OM_Z) blockSize100k = 0;

#if 0
   if (srcMode == SM_F2F) {
      signal (SIGINT,  mySignalCatcher);
      signal (SIGTERM, mySignalCatcher);
      signal (SIGHUP,  mySignalCatcher);
   }
#endif

   if (opMode == OM_Z) {
     if (srcMode == SM_I2O) {
        compress ( NULL );
     } else {
        decode = 1;
        for (aa = argList; aa != NULL; aa = aa->link) {
           if (strcmp(aa->name, "--") == 0) { decode = 0; continue; }
           if (aa->name[0] == '-' && decode) continue;
           numFilesProcessed++;
           compress ( aa->name );
        }
     }
   } 
   else

   if (opMode == OM_UNZ) {
      unzFailsExist = 0;
      if (srcMode == SM_I2O) {
         uncompress ( NULL );
      } else {
         decode = 1;
         for (aa = argList; aa != NULL; aa = aa->link) {
            if ( strcmp(aa->name, "--") == 0 ) { decode = 0; continue; }
            if (aa->name[0] == '-' && decode) continue;
            numFilesProcessed++;
            uncompress ( aa->name );
         }      
      }
      if (unzFailsExist) { 
         setExit(2); 
         //exit(exitValue);
         return exitValue;
      }
   } 

   else {
      testFailsExist = 0;
      if (srcMode == SM_I2O) {
         testf ( NULL );
      } else {
         decode = 1;
         for (aa = argList; aa != NULL; aa = aa->link) {
	    if (  strcmp(aa->name, "--") == 0 ) { decode = 0; continue; }
            if (aa->name[0] == '-' && decode) continue;
            numFilesProcessed++;
            testf ( aa->name );
	 }
      }
      if (testFailsExist && noisy) {
         fprintf ( stderr,
           "\n"
           "You can use the `bzip2recover' program to attempt to recover\n"
           "data from undamaged sections of corrupted files.\n\n"
         );
         setExit(2);
         //exit(exitValue);
         return exitValue;
      }
   }

    aa = argList;

    while (aa != NULL)
    {
        Cell* aa2 = aa->link;

        if (aa->name != NULL)
            //free(aa->name);
            delete[] aa->name;

        delete aa;
      //free(aa);
      aa = aa2;
   }

   return exitValue;
}

int main(int argc, char **argv)
{
    AppBzip2 app;
    return app.run(argc, argv);
}


