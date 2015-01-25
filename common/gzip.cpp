#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <malloc.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#define HAVE_CONFIG_H 1
#define HAVE_DIRENT_H 1
#define HAVE_FCNTL_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIMITS_H 1
#define HAVE_LSTAT 1
#define HAVE_MEMORY_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_TIME_H 1
#define HAVE_UNISTD_H 1
#define HAVE_UTIME 1
#define HAVE_UTIME_H 1
#define PACKAGE "gzip"
#define PACKAGE_BUGREPORT "bug-gzip@gnu.org"
#define PACKAGE_NAME "gzip"
#define PACKAGE_STRING "gzip 1.3.5"
#define PACKAGE_TARNAME "gzip"
#define PACKAGE_VERSION "1.3.5"
#define RETSIGTYPE void
#define STDC_HEADERS 1
#define VERSION "1.3.5"
#ifndef _ALL_SOURCE
#endif
#define _FILE_OFFSET_BITS 64
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

#if defined(__MSDOS__) && !defined(MSDOS)
#  define MSDOS
#endif

#if defined(__OS2__) && !defined(OS2)
#  define OS2
#endif

#if defined(OS2) && defined(MSDOS)
#  undef MSDOS
#endif

#if defined(ATARI) || defined(atarist)
#  ifndef STDC_HEADERS
#    define STDC_HEADERS
#    define HAVE_UNISTD_H
#    define HAVE_DIRENT_H
#  endif
#  define ASMV
#  define OS_CODE  0x05
#  ifdef TOSFS
#    define PATH_SEP2 '\\'
#    define PATH_SEP3 ':'
#    define MAX_PATH_LEN  128
#    define NO_MULTIPLE_DOTS
#    define MAX_EXT_CHARS 3
#    define Z_SUFFIX "z"
#    define casemap(c) tolow(c) 
#  endif
#endif

#ifdef MACOS
#  define PATH_SEP ':'
#  define DYN_ALLOC
#  define PROTO
#  define NO_STDIN_FSTAT
#  define chmod(file, mode) (0)
#  define OPEN(name, flags, mode) open(name, flags)
#  define OS_CODE  0x07
#  ifdef MPW
#    define isatty(fd) ((fd) <= 2)
#  endif
#endif

#ifdef __50SERIES
#  define PATH_SEP '>'
#  define STDC_HEADERS
#  define NO_STDIN_FSTAT 
#  define NO_SIZE_CHECK 
#  define RECORD_IO  1
#  define casemap(c)  tolow(c)
#  define put_char(c) put_byte((c) & 0x7F)
#  define get_char(c) ascii2pascii(get_byte())
#  define OS_CODE  0x0F 
#  ifdef SIGTERM
#    undef SIGTERM     
#  endif
#endif

#if defined(pyr) && !defined(NOMEMCPY) 
#  define NOMEMCPY 
#endif

#ifdef TOPS20
#  define OS_CODE  0x0a
#endif

#ifndef unix
#  define NO_ST_INO
#endif

#ifndef OS_CODE
#  define OS_CODE  0x03
#endif

#ifndef PATH_SEP
#  define PATH_SEP '/'
#endif

#ifndef casemap
#  define casemap(c) (c)
#endif

#ifndef OPTIONS_VAR
#  define OPTIONS_VAR "GZIP"
#endif

#ifndef Z_SUFFIX
#  define Z_SUFFIX ".gz"
#endif

#ifdef MAX_EXT_CHARS
#  define MAX_SUFFIX  MAX_EXT_CHARS
#else
#  define MAX_SUFFIX  30
#endif

#ifndef MAKE_LEGAL_NAME
#  ifdef NO_MULTIPLE_DOTS
#    define MAKE_LEGAL_NAME(name)   make_simple_name(name)
#  else
#    define MAKE_LEGAL_NAME(name)
#  endif
#endif

#ifndef MIN_PART
#  define MIN_PART 3
#endif

#ifndef EXPAND
#  define EXPAND(argc,argv)
#endif

#ifndef RECORD_IO
#  define RECORD_IO 0
#endif

#ifndef SET_BINARY_MODE
#  define SET_BINARY_MODE(fd)
#endif

#ifndef OPEN
#  define OPEN(name, flags, mode) open(name, flags, mode)
#endif

#ifndef get_char
#  define get_char() get_byte()
#endif

#ifndef put_char
#  define put_char(c) put_byte(c)
#endif


#if defined(__STDC__) || defined(PROTO)
#  define OF(args)  args
#else
#  define OF(args)  ()
#endif

#ifdef __STDC__
   typedef void *voidp;
#else
   typedef char *voidp;
#endif

#  define memzero(s, n)     memset ((voidp)(s), 0, (n))

#ifndef RETSIGTYPE
#  define RETSIGTYPE void
#endif

#define local static

typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;
#define OK      0
#define ERROR   1
#define WARNING 2
#define STORED      0
#define COMPRESSED  1
#define PACKED      2
#define LZHED       3
#define DEFLATED    8
#define MAX_METHODS 9
extern int method;


#ifndef	INBUFSIZ
#  ifdef SMALL_MEM
#    define INBUFSIZ  0x2000
#  else
#    define INBUFSIZ  0x8000
#  endif
#endif
#define INBUF_EXTRA  64   

#ifndef	OUTBUFSIZ
#  ifdef SMALL_MEM
#    define OUTBUFSIZ   8192
#  else
#    define OUTBUFSIZ  16384
#  endif
#endif
#define OUTBUF_EXTRA 2048

#ifndef DIST_BUFSIZE
#  ifdef SMALL_MEM
#    define DIST_BUFSIZE 0x2000
#  else
#    define DIST_BUFSIZE 0x8000
#  endif
#endif

#define near

#ifdef DYN_ALLOC
#  define EXTERN(type, array)  extern type * near array
#  define DECLARE(type, array, size)  type * near array
#  define ALLOC(type, array, size) { \
      array = (type*)fcalloc((size_t)(((size)+1L)/2), 2*sizeof(type)); \
      if (array == NULL) error("insufficient memory"); \
   }
#  define FREE(array) {if (array != NULL) fcfree(array), array=NULL;}
#else
#  define EXTERN(type, array)  extern type array[]
#  define DECLARE(type, array, size)  type array[size]
#  define ALLOC(type, array, size)
#  define FREE(array)
#endif

EXTERN(uch, inbuf);          
EXTERN(uch, outbuf);         
EXTERN(ush, d_buf);          
EXTERN(uch, window);         
#define tab_suffix window
#ifndef MAXSEG_64K
#  define tab_prefix prev    
#  define head (prev+WSIZE)  
   EXTERN(ush, tab_prefix);  
#else
#  define tab_prefix0 prev
#  define head tab_prefix1
   EXTERN(ush, tab_prefix0); 
   EXTERN(ush, tab_prefix1); 
#endif

extern unsigned insize; 
extern unsigned inptr;  
extern unsigned outcnt;
extern int rsync; 

extern off_t bytes_in;
extern off_t bytes_out;
extern off_t header_bytes;

extern int  ifd;       
extern int  ofd;        
extern char ifname[];   
extern char ofname[];   
extern char *progname;  

extern time_t time_stamp; /* original time stamp (modification time) */
extern off_t ifile_size; /* input file size, -1 for devices (debug only) */

typedef int file_t;     /* Do not use stdio */
#define NO_FILE  (-1)   /* in memory compression */


#define	PACK_MAGIC     "\037\036" /* Magic header for packed files */
#define	GZIP_MAGIC     "\037\213" /* Magic header for gzip files, 1F 8B */
#define	OLD_GZIP_MAGIC "\037\236" /* Magic header for gzip 0.5 = freeze 1.x */
#define	LZH_MAGIC      "\037\240" /* Magic header for SCO LZH Compress files*/
#define PKZIP_MAGIC    "\120\113\003\004" /* Magic header for pkzip files */

/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define CONTINUATION 0x02 /* bit 1 set: continuation of multi-part gzip file */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define ENCRYPTED    0x20 /* bit 5 set: file is encrypted */
#define RESERVED     0xC0 /* bit 6,7:   reserved */

/* internal file attribute */
#define UNKNOWN 0xffff
#define BINARY  0
#define ASCII   1

#ifndef WSIZE
#  define WSIZE 0x8000     /* window size--must be a power of two, and */
#endif                     /*  at least 32K for zip's deflate method */

#define MIN_MATCH  3
#define MAX_MATCH  258
/* The minimum and maximum match lengths */

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
/* Minimum amount of lookahead, except at the end of the input file.
 * See deflate.c for comments about the MIN_MATCH+1.
 */

#define MAX_DIST  (WSIZE-MIN_LOOKAHEAD)
/* In order to simplify the code, particularly on 16 bit machines, match
 * distances are limited to MAX_DIST instead of WSIZE.
 */

extern int decrypt;        /* flag to turn on decryption */
extern int exit_code;      /* program exit code */
extern int verbose;        /* be verbose (-v) */
extern int quiet;          /* be quiet (-q) */
extern int level;          /* compression level */
extern int test;           /* check .z file integrity */
extern int to_stdout;      /* output to stdout (-c) */
extern int save_orig_name; /* set if original name must be saved */

#define get_byte()  (inptr < insize ? inbuf[inptr++] : fill_inbuf(0))
#define try_byte()  (inptr < insize ? inbuf[inptr++] : fill_inbuf(1))

/* put_byte is used for the compressed output, put_ubyte for the
 * uncompressed output. However unlzw() uses window for its
 * suffix table instead of its output buffer, so it does not use put_ubyte
 * (to be cleaned up).
 */
#define put_byte(c) {outbuf[outcnt++]=(uch)(c); if (outcnt==OUTBUFSIZ)\
   flush_outbuf();}
#define put_ubyte(c) {window[outcnt++]=(uch)(c); if (outcnt==WSIZE)\
   flush_window();}

/* Output a 16 bit value, lsb first */
#define put_short(w) \
{ if (outcnt < OUTBUFSIZ-2) { \
    outbuf[outcnt++] = (uch) ((w) & 0xff); \
    outbuf[outcnt++] = (uch) ((ush)(w) >> 8); \
  } else { \
    put_byte((uch)((w) & 0xff)); \
    put_byte((uch)((ush)(w) >> 8)); \
  } \
}

/* Output a 32 bit value to the bit stream, lsb first */
#define put_long(n) { \
    put_short((n) & 0xffff); \
    put_short(((ulg)(n)) >> 16); \
}

#define seekable()    0  /* force sequential output */
#define translate_eol 0  /* no option -a yet */

#define tolow(c)  (isupper (c) ? tolower (c) : (c))  /* force to lower case */

/* Macros for getting two-byte and four-byte header values */
#define SH(p) ((ush)(uch)((p)[0]) | ((ush)(uch)((p)[1]) << 8))
#define LG(p) ((ulg)(SH(p)) | ((ulg)(SH((p)+2)) << 16))

/* Diagnostic functions */
#ifdef DEBUG
#  define Assert(cond,msg) {if(!(cond)) error(msg);}
#  define Trace(x) fprintf x
#  define Tracev(x) {if (verbose) fprintf x ;}
#  define Tracevv(x) {if (verbose>1) fprintf x ;}
#  define Tracec(c,x) {if (verbose && (c)) fprintf x ;}
#  define Tracecv(c,x) {if (verbose>1 && (c)) fprintf x ;}
#else
#  define Assert(cond,msg)
#  define Trace(x)
#  define Tracev(x)
#  define Tracevv(x)
#  define Tracec(c,x)
#  define Tracecv(c,x)
#endif

#define WARN(msg) {if (!quiet) fprintf msg ; \
		   if (exit_code == OK) exit_code = WARNING;}

	/* in zip.c: */
extern int zip        OF((int in, int out));
extern int file_read  OF((char *buf,  unsigned size));

	/* in unzip.c */
extern int unzip      OF((int in, int out));
extern int check_zipfile OF((int in));

	/* in unpack.c */
extern int unpack     OF((int in, int out));

	/* in unlzh.c */
extern int unlzh      OF((int in, int out));

	/* in gzip.c */
RETSIGTYPE abort_gzip_signal OF((void));

        /* in deflate.c */
void lm_init OF((int pack_level, ush *flags));
off_t deflate OF((void));

        /* in trees.c */
void ct_init     OF((ush *attr, int *method));
int  ct_tally    OF((int dist, int lc));
off_t flush_block OF((char *buf, ulg stored_len, int pad, int eof));

        /* in bits.c */
void     bi_init    OF((file_t zipfile));
void     send_bits  OF((int value, int length));
unsigned bi_reverse OF((unsigned value, int length));
void     bi_windup  OF((void));
void     copy_block OF((char *buf, unsigned len, int header));
extern   int (*read_buf) OF((char *buf, unsigned size));

	/* in util.c: */
extern int copy           OF((int in, int out));
extern ulg  updcrc        OF((uch *s, unsigned n));
extern void clear_bufs    OF((void));
extern int  fill_inbuf    OF((int eof_ok));
extern void flush_outbuf  OF((void));
extern void flush_window  OF((void));
extern void write_buf     OF((int fd, voidp buf, unsigned cnt));
extern char *strlwr       OF((char *s));
extern char *base_name    OF((char *fname));
extern int xunlink        OF((char *fname));
extern void make_simple_name OF((char *name));
extern char *add_envopt   OF((int *argcp, char ***argvp, char *env));
extern void error         OF((char *m));
extern void warning       OF((char *m));
extern void read_error    OF((void));
extern void write_error   OF((void));
extern void display_ratio OF((off_t num, off_t den, FILE *file));
extern void fprint_off    OF((FILE *, off_t, int));
extern voidp xmalloc      OF((unsigned int size));

	/* in inflate.c */
extern int inflate OF((void));

	/* in yesno.c */
extern int yesno OF((void));


/* crypt.h (dummy version) -- do not perform encryption
 * Hardly worth copyrighting :-)
 */

#ifdef CRYPT
#  undef CRYPT      /* dummy version */
#endif

#define RAND_HEAD_LEN  12  /* length of encryption random header */

#define zencode
#define zdecode


#ifdef RCSID
static char rcsid[] = "$Id: bits.c,v 0.9 1993/06/11 10:16:58 jloup Exp $";
#endif

/* ===========================================================================
 * Local data used by the "bit string" routines.
 */

local file_t zfile; /* output gzip file */

local unsigned short bi_buf;
/* Output buffer. bits are inserted starting at the bottom (least significant
 * bits).
 */

#define Buf_size (8 * 2*sizeof(char))
/* Number of bits used within bi_buf. (bi_buf might be implemented on
 * more than 16 bits on some systems.)
 */

local int bi_valid;
/* Number of valid bits in bi_buf.  All bits above the last valid bit
 * are always zero.
 */

int (*read_buf) OF((char *buf, unsigned size));
/* Current input function. Set to mem_read for in-memory compression */

#ifdef DEBUG
  off_t bits_sent;   /* bit length of the compressed data */
#endif

/* ===========================================================================
 * Initialize the bit string routines.
 */
void bi_init (file_t zipfile)
{
    zfile  = zipfile;
    bi_buf = 0;
    bi_valid = 0;
#ifdef DEBUG
    bits_sent = 0L;
#endif

    /* Set the defaults for file compression. They are set by memcompress
     * for in-memory compression.
     */
    if (zfile != NO_FILE) {
	read_buf  = file_read;
    }
}

/* ===========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
void send_bits(int value, int length)
{
#ifdef DEBUG
    Tracev((stderr," l %2d v %4x ", length, value));
    Assert(length > 0 && length <= 15, "invalid length");
    bits_sent += (off_t)length;
#endif
    /* If not enough room in bi_buf, use (valid) bits from bi_buf and
     * (16 - bi_valid) bits from value, leaving (width - (16-bi_valid))
     * unused bits in value.
     */
    if (bi_valid > (int)Buf_size - length) {
        bi_buf |= (value << bi_valid);
        put_short(bi_buf);
        bi_buf = (ush)value >> (Buf_size - bi_valid);
        bi_valid += length - Buf_size;
    } else {
        bi_buf |= value << bi_valid;
        bi_valid += length;
    }
}

/* ===========================================================================
 * Reverse the first len bits of a code, using straightforward code (a faster
 * method would use a table)
 * IN assertion: 1 <= len <= 15
 */
unsigned bi_reverse(unsigned code, int len)
{
    register unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

/* ===========================================================================
 * Write out any remaining bits in an incomplete byte.
 */
void bi_windup()
{
    if (bi_valid > 8) {
        put_short(bi_buf);
    } else if (bi_valid > 0) {
        put_byte(bi_buf);
    }
    bi_buf = 0;
    bi_valid = 0;
#ifdef DEBUG
    bits_sent = (bits_sent+7) & ~7;
#endif
}

/* ===========================================================================
 * Copy a stored block to the zip file, storing first the length and its
 * one's complement if requested.
 */
void copy_block(char *buf, unsigned len, int header)
{
    bi_windup();              /* align on byte boundary */

    if (header) {
        put_short((ush)len);   
        put_short((ush)~len);
#ifdef DEBUG
        bits_sent += 2*16;
#endif
    }
#ifdef DEBUG
    bits_sent += (off_t)len<<3;
#endif
    while (len--) {
#ifdef CRYPT
        int t;
	if (key) zencode(*buf, t);
#endif
	put_byte(*buf++);
    }
}
/* crypt.c (dummy version) -- do not perform encryption
 * Hardly worth copyrighting :-)
 */
#ifdef RCSID
static char rcsid[] = "$Id: crypt.c,v 0.6 1993/03/22 09:48:47 jloup Exp $";
#endif


#ifndef BITS
#  define BITS 16
#endif
#define INIT_BITS 9              /* Initial number of bits per code */

#define	LZW_MAGIC  "\037\235"   /* Magic header for lzw files, 1F 9D */

#define BIT_MASK    0x1f 
#define BLOCK_MODE  0x80
#define LZW_RESERVED 0x60
#define	CLEAR  256       
#define FIRST  (CLEAR+1)

extern int maxbits;
extern int block_mode;

extern int lzw    OF((int in, int out));
extern int unlzw  OF((int in, int out));


#ifdef RCSID
static char rcsid[] = "$Id: deflate.c,v 0.15 1993/06/24 10:53:53 jloup Exp $";
#endif

#ifdef SMALL_MEM
#   define HASH_BITS  13
#endif
#ifdef MEDIUM_MEM
#   define HASH_BITS  14
#endif
#ifndef HASH_BITS
#   define HASH_BITS  15
#endif

#if (WSIZE<<1) > (1<<BITS)
   error: cannot overlay window with tab_suffix and prev with tab_prefix0
#endif
#if HASH_BITS > BITS-1
   error: cannot overlay head with tab_prefix1
#endif

#define HASH_SIZE (unsigned)(1<<HASH_BITS)
#define HASH_MASK (HASH_SIZE-1)
#define WMASK     (WSIZE-1)

#define NIL 0

#define FAST 4
#define SLOW 2

#ifndef TOO_FAR
#  define TOO_FAR 4096
#endif

#ifndef RSYNC_WIN
#  define RSYNC_WIN 4096
#endif

#define RSYNC_SUM_MATCH(sum) ((sum) % RSYNC_WIN == 0)

typedef ush Pos;
typedef unsigned IPos;

ulg window_size = (ulg)2*WSIZE;


long block_start;

local unsigned ins_h;

#define H_SHIFT  ((HASH_BITS+MIN_MATCH-1)/MIN_MATCH)

unsigned int near prev_length;

      unsigned near strstart;      /* start of string to insert */
      unsigned near match_start;   /* start of matching string */
local int           eofile;        /* flag set at end of input file */
local unsigned      lookahead;     /* number of valid bytes ahead in window */

unsigned near max_chain_length;
/* To speed up deflation, hash chains are never searched beyond this length.
 * A higher limit improves compression ratio but degrades the speed.
 */

local unsigned int max_lazy_match;
/* Attempt to find a better match only when the current match is strictly
 * smaller than this value. This mechanism is used only for compression
 * levels >= 4.
 */
#define max_insert_length  max_lazy_match
/* Insert new strings in the hash table only if the match length
 * is not greater than this length. This saves time but degrades compression.
 * max_insert_length is used only for compression levels <= 3.
 */

local int compr_level;
/* compression level (1..9) */

unsigned near good_match;
/* Use a faster search when the previous match is longer than this */

local ulg rsync_sum;  /* rolling sum of rsync window */
local ulg rsync_chunk_end; /* next rsync sequence point */

/* Values for max_lazy_match, good_match and max_chain_length, depending on
 * the desired pack level (0..9). The values given below have been tuned to
 * exclude worst case performance for pathological files. Better values may be
 * found for specific files.
 */

typedef struct config {
   ush good_length; /* reduce lazy search above this match length */
   ush max_lazy;    /* do not perform lazy search above this match length */
   ush nice_length; /* quit search above this match length */
   ush max_chain;
} config;

#ifdef  FULL_SEARCH
# define nice_match MAX_MATCH
#else
  int near nice_match; /* Stop searching when current match exceeds this */
#endif

local config configuration_table[10] = {
/*      good lazy nice chain */
/* 0 */ {0,    0,  0,    0},  /* store only */
/* 1 */ {4,    4,  8,    4},  /* maximum speed, no lazy matches */
/* 2 */ {4,    5, 16,    8},
/* 3 */ {4,    6, 32,   32},

/* 4 */ {4,    4, 16,   16},  /* lazy matches */
/* 5 */ {8,   16, 32,   32},
/* 6 */ {8,   16, 128, 128},
/* 7 */ {8,   32, 128, 256},
/* 8 */ {32, 128, 258, 1024},
/* 9 */ {32, 258, 258, 4096}}; /* maximum compression */

/* Note: the deflate() code requires max_lazy >= MIN_MATCH and max_chain >= 4
 * For deflate_fast() (levels <= 3) good is ignored and lazy has a different
 * meaning.
 */

#define EQUAL 0
/* result of memcmp for equal strings */

/* ===========================================================================
 *  Prototypes for local functions.
 */
local void fill_window   OF((void));
local off_t deflate_fast OF((void));

      int  longest_match OF((IPos cur_match));
#ifdef ASMV
      void match_init OF((void)); /* asm code initialization */
#endif

#ifdef DEBUG
local  void check_match OF((IPos start, IPos match, int length));
#endif

/* ===========================================================================
 * Update a hash value with the given input byte
 * IN  assertion: all calls to to UPDATE_HASH are made with consecutive
 *    input characters, so that a running hash key can be computed from the
 *    previous key instead of complete recalculation each time.
 */
#define UPDATE_HASH(h,c) (h = (((h)<<H_SHIFT) ^ (c)) & HASH_MASK)


#define INSERT_STRING(s, match_head) \
   (UPDATE_HASH(ins_h, window[(s) + MIN_MATCH-1]), \
    prev[(s) & WMASK] = match_head = head[ins_h], \
    head[ins_h] = (s))


void lm_init (int pack_level, ush *flags)
{
    register unsigned j;

    if (pack_level < 1 || pack_level > 9) error((char *)"bad pack level");
    compr_level = pack_level;

#if defined(MAXSEG_64K) && HASH_BITS == 15
    for (j = 0;  j < HASH_SIZE; j++) head[j] = NIL;
#else
    memzero((char*)head, HASH_SIZE*sizeof(*head));
#endif
    rsync_chunk_end = 0xFFFFFFFFUL;
    rsync_sum = 0;

    max_lazy_match   = configuration_table[pack_level].max_lazy;
    good_match       = configuration_table[pack_level].good_length;
#ifndef FULL_SEARCH
    nice_match       = configuration_table[pack_level].nice_length;
#endif
    max_chain_length = configuration_table[pack_level].max_chain;
    if (pack_level == 1) {
       *flags |= FAST;
    } else if (pack_level == 9) {
       *flags |= SLOW;
    }
    strstart = 0;
    block_start = 0L;
#ifdef ASMV
    match_init();
#endif
    lookahead = read_buf((char*)window,
			 sizeof(int) <= 2 ? (unsigned)WSIZE : 2*WSIZE);

    if (lookahead == 0 || lookahead == (unsigned)EOF) {
       eofile = 1, lookahead = 0;
       return;
    }
    eofile = 0;
    /* Make sure that we always have enough lookahead. This is important
     * if input comes from a device such as a tty.
     */
    while (lookahead < MIN_LOOKAHEAD && !eofile) fill_window();

    ins_h = 0;
    for (j=0; j<MIN_MATCH-1; j++) UPDATE_HASH(ins_h, window[j]);

}


#ifndef ASMV

int longest_match(IPos cur_match)
{
    unsigned chain_length = max_chain_length;   /* max hash chain length */
    register uch *scan = window + strstart;     /* current string */
    register uch *match;                        /* matched string */
    register int len;                           /* length of current match */
    int best_len = prev_length;                 /* best match length so far */
    IPos limit = strstart > (IPos)MAX_DIST ? strstart - (IPos)MAX_DIST : NIL;

#if HASH_BITS < 8 || MAX_MATCH != 258
   error: Code too clever
#endif

#ifdef UNALIGNED_OK

    register uch *strend = window + strstart + MAX_MATCH - 1;
    register ush scan_start = *(ush*)scan;
    register ush scan_end   = *(ush*)(scan+best_len-1);
#else
    register uch *strend = window + strstart + MAX_MATCH;
    register uch scan_end1  = scan[best_len-1];
    register uch scan_end   = scan[best_len];
#endif

    if (prev_length >= good_match) {
        chain_length >>= 2;
    }
    Assert(strstart <= window_size-MIN_LOOKAHEAD, "insufficient lookahead");

    do {
        Assert(cur_match < strstart, "no future");
        match = window + cur_match;

        /* Skip to next match if the match length cannot increase
         * or if the match length is less than 2:
         */
#if (defined(UNALIGNED_OK) && MAX_MATCH == 258)
        /* This code assumes sizeof(unsigned short) == 2. Do not use
         * UNALIGNED_OK if your compiler uses a different size.
         */
        if (*(ush*)(match+best_len-1) != scan_end ||
            *(ush*)match != scan_start) continue;

        /* It is not necessary to compare scan[2] and match[2] since they are
         * always equal when the other bytes match, given that the hash keys
         * are equal and that HASH_BITS >= 8. Compare 2 bytes at a time at
         * strstart+3, +5, ... up to strstart+257. We check for insufficient
         * lookahead only every 4th comparison; the 128th check will be made
         * at strstart+257. If MAX_MATCH-2 is not a multiple of 8, it is
         * necessary to put more guard bytes at the end of the window, or
         * to check more often for insufficient lookahead.
         */
        scan++, match++;
        do {
        } while (*(ush*)(scan+=2) == *(ush*)(match+=2) &&
                 *(ush*)(scan+=2) == *(ush*)(match+=2) &&
                 *(ush*)(scan+=2) == *(ush*)(match+=2) &&
                 *(ush*)(scan+=2) == *(ush*)(match+=2) &&
                 scan < strend);
        /* The funny "do {}" generates better code on most compilers */

        /* Here, scan <= window+strstart+257 */
        Assert(scan <= window+(unsigned)(window_size-1), "wild scan");
        if (*scan == *match) scan++;

        len = (MAX_MATCH - 1) - (int)(strend-scan);
        scan = strend - (MAX_MATCH-1);

#else /* UNALIGNED_OK */

        if (match[best_len]   != scan_end  ||
            match[best_len-1] != scan_end1 ||
            *match            != *scan     ||
            *++match          != scan[1])      continue;

        /* The check at best_len-1 can be removed because it will be made
         * again later. (This heuristic is not always a win.)
         * It is not necessary to compare scan[2] and match[2] since they
         * are always equal when the other bytes match, given that
         * the hash keys are equal and that HASH_BITS >= 8.
         */
        scan += 2, match++;

        /* We check for insufficient lookahead only every 8th comparison;
         * the 256th check will be made at strstart+258.
         */
        do {
        } while (*++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 scan < strend);

        len = MAX_MATCH - (int)(strend - scan);
        scan = strend - MAX_MATCH;

#endif /* UNALIGNED_OK */

        if (len > best_len) {
            match_start = cur_match;
            best_len = len;
            if (len >= nice_match) break;
#ifdef UNALIGNED_OK
            scan_end = *(ush*)(scan+best_len-1);
#else
            scan_end1  = scan[best_len-1];
            scan_end   = scan[best_len];
#endif
        }
    } while ((cur_match = prev[cur_match & WMASK]) > limit
	     && --chain_length != 0);

    return best_len;
}
#endif /* ASMV */

#ifdef DEBUG
/* ===========================================================================
 * Check that the match at match_start is indeed a match.
 */
local void check_match(IPos start, IPos match, int length)
{
    /* check that the match is indeed a match */
    if (memcmp((char*)window + match,
                (char*)window + start, length) != EQUAL) {
        fprintf(stderr,
            " start %d, match %d, length %d\n",
            start, match, length);
        error("invalid match");
    }
    if (verbose > 1) {
        fprintf(stderr,"\\[%d,%d]", start-match, length);
        do { putc(window[start++], stderr); } while (--length != 0);
    }
}
#else
#  define check_match(start, match, length)
#endif

/* ===========================================================================
 * Fill the window when the lookahead becomes insufficient.
 * Updates strstart and lookahead, and sets eofile if end of input file.
 * IN assertion: lookahead < MIN_LOOKAHEAD && strstart + lookahead > 0
 * OUT assertions: at least one byte has been read, or eofile is set;
 *    file reads are performed for at least two bytes (required for the
 *    translate_eol option).
 */
local void fill_window()
{
    register unsigned n, m;
    unsigned more = (unsigned)(window_size - (ulg)lookahead - (ulg)strstart);
    /* Amount of free space at the end of the window. */

    /* If the window is almost full and there is insufficient lookahead,
     * move the upper half to the lower one to make room in the upper half.
     */
    if (more == (unsigned)EOF) {
        /* Very unlikely, but possible on 16 bit machine if strstart == 0
         * and lookahead == 1 (input done one byte at time)
         */
        more--;
    } else if (strstart >= WSIZE+MAX_DIST) {
        /* By the IN assertion, the window is not empty so we can't confuse
         * more == 0 with more == 64K on a 16 bit machine.
         */
        Assert(window_size == (ulg)2*WSIZE, "no sliding with BIG_MEM");

        memcpy((char*)window, (char*)window+WSIZE, (unsigned)WSIZE);
        match_start -= WSIZE;
        strstart    -= WSIZE; /* we now have strstart >= MAX_DIST: */
	if (rsync_chunk_end != 0xFFFFFFFFUL)
	    rsync_chunk_end -= WSIZE;

        block_start -= (long) WSIZE;

        for (n = 0; n < HASH_SIZE; n++) {
            m = head[n];
            head[n] = (Pos)(m >= WSIZE ? m-WSIZE : NIL);
        }
        for (n = 0; n < WSIZE; n++) {
            m = prev[n];
            prev[n] = (Pos)(m >= WSIZE ? m-WSIZE : NIL);
            /* If n is not on any hash chain, prev[n] is garbage but
             * its value will never be used.
             */
        }
        more += WSIZE;
    }
    /* At this point, more >= 2 */
    if (!eofile) {
        n = read_buf((char*)window+strstart+lookahead, more);
        if (n == 0 || n == (unsigned)EOF) {
            eofile = 1;
        } else {
            lookahead += n;
        }
    }
}

local void rsync_roll(unsigned start, unsigned num)
{
    unsigned i;

    if (start < RSYNC_WIN) {
	/* before window fills. */
	for (i = start; i < RSYNC_WIN; i++) {
	    if (i == start + num) return;
	    rsync_sum += (ulg)window[i];
	}
	num -= (RSYNC_WIN - start);
	start = RSYNC_WIN;
    }

    /* buffer after window full */
    for (i = start; i < start+num; i++) {
	/* New character in */
	rsync_sum += (ulg)window[i];
	/* Old character out */
	rsync_sum -= (ulg)window[i - RSYNC_WIN];
	if (rsync_chunk_end == 0xFFFFFFFFUL && RSYNC_SUM_MATCH(rsync_sum))
	    rsync_chunk_end = i;
    }
}

/* ===========================================================================
 * Set rsync_chunk_end if window sum matches magic value.
 */
#define RSYNC_ROLL(s, n) \
   do { if (rsync) rsync_roll((s), (n)); } while(0)

/* ===========================================================================
 * Flush the current block, with given end-of-file flag.
 * IN assertion: strstart is set to the end of the current match.
 */
#define FLUSH_BLOCK(eof) \
   flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : \
                (char*)NULL, (long)strstart - block_start, flush-1, (eof))

/* ===========================================================================
 * Processes a new input file and return its compressed length. This
 * function does not perform lazy evaluationof matches and inserts
 * new strings in the dictionary only for unmatched strings or for short
 * matches. It is used only for the fast compression options.
 */
local off_t deflate_fast()
{
    IPos hash_head; /* head of the hash chain */
    int flush;      /* set if current block must be flushed, 2=>and padded  */
    unsigned match_length = 0;  /* length of best match */

    prev_length = MIN_MATCH-1;
    while (lookahead != 0) {
        /* Insert the string window[strstart .. strstart+2] in the
         * dictionary, and set hash_head to the head of the hash chain:
         */
        INSERT_STRING(strstart, hash_head);

        /* Find the longest match, discarding those <= prev_length.
         * At this point we have always match_length < MIN_MATCH
         */
        if (hash_head != NIL && strstart - hash_head <= MAX_DIST &&
	    strstart <= window_size - MIN_LOOKAHEAD) {
            /* To simplify the code, we prevent matches with the string
             * of window index 0 (in particular we have to avoid a match
             * of the string with itself at the start of the input file).
             */
            match_length = longest_match (hash_head);
            /* longest_match() sets match_start */
            if (match_length > lookahead) match_length = lookahead;
        }
        if (match_length >= MIN_MATCH) {
            check_match(strstart, match_start, match_length);

            flush = ct_tally(strstart-match_start, match_length - MIN_MATCH);

            lookahead -= match_length;

	    RSYNC_ROLL(strstart, match_length);
	    /* Insert new strings in the hash table only if the match length
             * is not too large. This saves time but degrades compression.
             */
            if (match_length <= max_insert_length) {
                match_length--; /* string at strstart already in hash table */
                do {
                    strstart++;
                    INSERT_STRING(strstart, hash_head);
                    /* strstart never exceeds WSIZE-MAX_MATCH, so there are
                     * always MIN_MATCH bytes ahead. If lookahead < MIN_MATCH
                     * these bytes are garbage, but it does not matter since
                     * the next lookahead bytes will be emitted as literals.
                     */
                } while (--match_length != 0);
	        strstart++; 
            } else {
	        strstart += match_length;
	        match_length = 0;
	        ins_h = window[strstart];
	        UPDATE_HASH(ins_h, window[strstart+1]);
#if MIN_MATCH != 3
                Call UPDATE_HASH() MIN_MATCH-3 more times
#endif
            }
        } else {
            Tracevv((stderr,"%c",window[strstart]));
            flush = ct_tally (0, window[strstart]);
	    RSYNC_ROLL(strstart, 1);
            lookahead--;
	    strstart++; 
        }
	if (rsync && strstart > rsync_chunk_end) {
	    rsync_chunk_end = 0xFFFFFFFFUL;
	    flush = 2;
	} 
        if (flush) FLUSH_BLOCK(0), block_start = strstart;

        while (lookahead < MIN_LOOKAHEAD && !eofile) fill_window();

    }
    return FLUSH_BLOCK(1);
}

off_t deflate()
{
    IPos hash_head;      
    IPos prev_match;     
    int flush;           
    int match_available = 0;
    register unsigned match_length = MIN_MATCH-1;

    if (compr_level <= 3) return deflate_fast();
    while (lookahead != 0) {
    
        INSERT_STRING(strstart, hash_head);

        /* Find the longest match, discarding those <= prev_length.
         */
        prev_length = match_length, prev_match = match_start;
        match_length = MIN_MATCH-1;

        if (hash_head != NIL && prev_length < max_lazy_match &&
            strstart - hash_head <= MAX_DIST &&
	    strstart <= window_size - MIN_LOOKAHEAD) {
            /* To simplify the code, we prevent matches with the string
             * of window index 0 (in particular we have to avoid a match
             * of the string with itself at the start of the input file).
             */
            match_length = longest_match (hash_head);
            /* longest_match() sets match_start */
            if (match_length > lookahead) match_length = lookahead;

            /* Ignore a length 3 match if it is too distant: */
            if (match_length == MIN_MATCH && strstart-match_start > TOO_FAR){
                /* If prev_match is also MIN_MATCH, match_start is garbage
                 * but we will ignore the current match anyway.
                 */
                match_length--;
            }
        }
        /* If there was a match at the previous step and the current
         * match is not better, output the previous match:
         */
        if (prev_length >= MIN_MATCH && match_length <= prev_length) {

            check_match(strstart-1, prev_match, prev_length);

            flush = ct_tally(strstart-1-prev_match, prev_length - MIN_MATCH);

            /* Insert in hash table all strings up to the end of the match.
             * strstart-1 and strstart are already inserted.
             */
            lookahead -= prev_length-1;
            prev_length -= 2;
	    RSYNC_ROLL(strstart, prev_length+1);
            do {
                strstart++;
                INSERT_STRING(strstart, hash_head);
                /* strstart never exceeds WSIZE-MAX_MATCH, so there are
                 * always MIN_MATCH bytes ahead. If lookahead < MIN_MATCH
                 * these bytes are garbage, but it does not matter since the
                 * next lookahead bytes will always be emitted as literals.
                 */
            } while (--prev_length != 0);
            match_available = 0;
            match_length = MIN_MATCH-1;
            strstart++;

	    if (rsync && strstart > rsync_chunk_end) {
		rsync_chunk_end = 0xFFFFFFFFUL;
		flush = 2;
	    }
            if (flush) FLUSH_BLOCK(0), block_start = strstart;
        } else if (match_available) {
            /* If there was no match at the previous position, output a
             * single literal. If there was a match but the current match
             * is longer, truncate the previous match to a single literal.
             */
            Tracevv((stderr,"%c",window[strstart-1]));
	    flush = ct_tally (0, window[strstart-1]);
	    if (rsync && strstart > rsync_chunk_end) {
		rsync_chunk_end = 0xFFFFFFFFUL;
		flush = 2;
	    }
            if (flush) FLUSH_BLOCK(0), block_start = strstart;
	    RSYNC_ROLL(strstart, 1);
            strstart++;
            lookahead--;
        } else {
            /* There is no previous match to compare with, wait for
             * the next step to decide.
             */
	    if (rsync && strstart > rsync_chunk_end) {
		/* Reset huffman tree */
		rsync_chunk_end = 0xFFFFFFFFUL;
		flush = 2;
		FLUSH_BLOCK(0), block_start = strstart;
	    }
            match_available = 1;
	    RSYNC_ROLL(strstart, 1);
            strstart++;
            lookahead--;
        }
        Assert (strstart <= bytes_in && lookahead <= bytes_in, "a bit too far");

        /* Make sure that we always have enough lookahead, except
         * at the end of the input file. We need MAX_MATCH bytes
         * for the next match, plus MIN_MATCH bytes to insert the
         * string following the next match.
         */
        while (lookahead < MIN_LOOKAHEAD && !eofile) fill_window();
    }
    if (match_available) ct_tally (0, window[strstart-1]);

    return FLUSH_BLOCK(1); /* eof */
}
/* Getopt for GNU.
   NOTE: getopt is now part of the C library, so if you don't know what
   "Keep this file name-space clean" means, talk to drepper@gnu.org
   before changing it!
   Copyright (C) 1987,88,89,90,91,92,93,94,95,96,98,99,2000,2001
   	Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* This tells Alpha OSF/1 not to define a getopt prototype in <stdio.h>.
   Ditto for AIX 3.2 and <stdlib.h>.  */
#ifndef _NO_PROTO
# define _NO_PROTO
#endif

#if !defined __STDC__ || !__STDC__
/* This is a separate conditional since some stdc systems
   reject `defined (const)'.  */
# ifndef const
#  define const
# endif
#endif

/* This needs to come after some library #include
   to get __GNU_LIBRARY__ defined.  */

#  define _(msgid)	(msgid)

/* This version of `getopt' appears to the caller like standard Unix `getopt'
   but it behaves differently for the user, since it allows the user
   to intersperse the options with the other arguments.

   As `getopt' works, it permutes the elements of ARGV so that,
   when it is done, all the options precede everything else.  Thus
   all application programs are extended to handle flexible argument order.

   Setting the environment variable POSIXLY_CORRECT disables permutation.
   Then the behavior is completely standard.

   GNU application programs can use a third alternative mode in which
   they can distinguish the relative order of options and other arguments.  */

/* Declarations for getopt.
   Copyright (C) 1989-1994, 1996-1999, 2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _GETOPT_H

#ifndef __need_getopt
# define _GETOPT_H 1
#endif


#ifdef	__cplusplus
extern "C" {
#endif

/* For communication from `getopt' to the caller.
   When `getopt' finds an option that takes an argument,
   the argument value is returned here.
   Also, when `ordering' is RETURN_IN_ORDER,
   each non-option ARGV-element is returned here.  */

extern char *optarg;

/* Index in ARGV of the next element to be scanned.
   This is used for communication to and from the caller
   and for communication between successive calls to `getopt'.

   On entry to `getopt', zero means this is the first call; initialize.

   When `getopt' returns -1, this is the index of the first of the
   non-option elements that the caller should itself scan.

   Otherwise, `optind' communicates from one call to the next
   how much of ARGV has been scanned so far.  */

extern int optind;

/* Callers store zero here to inhibit the error message `getopt' prints
   for unrecognized options.  */

extern int opterr;

/* Set to an option character which was unrecognized.  */

extern int optopt;

/* Describe the long-named options requested by the application.
   The LONG_OPTIONS argument to getopt_long or getopt_long_only is a vector
   of `struct option' terminated by an element containing a name which is
   zero.

   The field `has_arg' is:
   no_argument		(or 0) if the option does not take an argument,
   required_argument	(or 1) if the option requires an argument,
   optional_argument 	(or 2) if the option takes an optional argument.

   If the field `flag' is not NULL, it points to a variable that is set
   to the value given in the field `val' when the option is found, but
   left unchanged if the option is not found.

   To have a long-named option do something other than set an `int' to
   a compiled-in constant, such as set a value from `optarg', set the
   option's `flag' field to zero and its `val' field to a nonzero
   value (the equivalent single-letter option character, if there is
   one).  For long options that have a zero `flag' field, `getopt'
   returns the contents of the `val' field.  */

struct option
{
# if (defined __STDC__ && __STDC__) || defined __cplusplus
  const char *name;
# else
  char *name;
# endif
  /* has_arg can't be an enum because some compilers complain about
     type mismatches in all the code that assumes it is an int.  */
  int has_arg;
  int *flag;
  int val;
};

/* Names for the values of the `has_arg' field of `struct option'.  */

# define no_argument		0
# define required_argument	1
# define optional_argument	2


/* Get definitions and prototypes for functions to process the
   arguments in ARGV (ARGC of them, minus the program name) for
   options given in OPTS.

   Return the option character from OPTS just read.  Return -1 when
   there are no more options.  For unrecognized options, or options
   missing arguments, `optopt' is set to the option letter, and '?' is
   returned.

   The OPTS string is a list of characters which are recognized option
   letters, optionally followed by colons, specifying that that letter
   takes an argument, to be placed in `optarg'.

   If a letter in OPTS is followed by two colons, its argument is
   optional.  This behavior is specific to the GNU `getopt'.

   The argument `--' causes premature termination of argument
   scanning, explicitly telling `getopt' that there are no more
   options.

   If OPTS begins with `--', then non-option arguments are treated as
   arguments to the option '\0'.  This behavior is specific to the GNU
   `getopt'.  */

#if (defined __STDC__ && __STDC__) || defined __cplusplus
# ifdef __GNU_LIBRARY__
/* Many other libraries have conflicting prototypes for getopt, with
   differences in the consts, in stdlib.h.  To avoid compilation
   errors, only prototype getopt for the GNU C library.  */
extern int getopt (int __argc, char *const *__argv, const char *__shortopts);
# else /* not __GNU_LIBRARY__ */
extern int getopt ();
# endif /* __GNU_LIBRARY__ */

# ifndef __need_getopt
extern int getopt_long (int __argc, char *const *__argv, const char *__shortopts,
		        const struct option *__longopts, int *__longind);
extern int getopt_long_only (int __argc, char *const *__argv,
			     const char *__shortopts,
		             const struct option *__longopts, int *__longind);

/* Internal only.  Users should not call this directly.  */
extern int _getopt_internal (int __argc, char *const *__argv,
			     const char *__shortopts,
		             const struct option *__longopts, int *__longind,
			     int __long_only);
# endif
#else /* not __STDC__ */
extern int getopt ();
# ifndef __need_getopt
extern int getopt_long ();
extern int getopt_long_only ();

extern int _getopt_internal ();
# endif
#endif /* __STDC__ */

#ifdef	__cplusplus
}
#endif

/* Make sure we later can get all the definitions and declarations.  */
#undef __need_getopt

#endif /* getopt.h */


/* For communication from `getopt' to the caller.
   When `getopt' finds an option that takes an argument,
   the argument value is returned here.
   Also, when `ordering' is RETURN_IN_ORDER,
   each non-option ARGV-element is returned here.  */

char *optarg;

/* Index in ARGV of the next element to be scanned.
   This is used for communication to and from the caller
   and for communication between successive calls to `getopt'.

   On entry to `getopt', zero means this is the first call; initialize.

   When `getopt' returns -1, this is the index of the first of the
   non-option elements that the caller should itself scan.

   Otherwise, `optind' communicates from one call to the next
   how much of ARGV has been scanned so far.  */

/* 1003.2 says this must be 1 before any call.  */
int optind = 1;

/* Formerly, initialization of getopt depended on optind==0, which
   causes problems with re-calling getopt as programs generally don't
   know that. */

int __getopt_initialized;

/* The next char to be scanned in the option-element
   in which the last option character we returned was found.
   This allows us to pick up the scan where we left off.

   If this is zero, or a null string, it means resume the scan
   by advancing to the next ARGV-element.  */

static char *nextchar;

/* Callers store zero here to inhibit the error message
   for unrecognized options.  */

int opterr = 1;

/* Set to an option character which was unrecognized.
   This must be initialized on some systems to avoid linking in the
   system's own getopt implementation.  */

int optopt = '?';

static enum
{
  REQUIRE_ORDER, PERMUTE, RETURN_IN_ORDER
} ordering;

static char *posixly_correct;

#ifndef getenv
extern char *getenv ();
#endif

static char *
my_index (const char *str, int chr)
{
  while (*str)
    {
      if (*str == chr)
	return (char *) str;
      str++;
    }
  return 0;
}

#ifdef __GNUC__
# if (!defined __STDC__ || !__STDC__) && !defined strlen
extern int strlen (const char *);
# endif /* not __STDC__ */
#endif /* __GNUC__ */

static int first_nonopt;
static int last_nonopt;

#ifdef _LIBC

#ifdef USE_NONOPTION_FLAGS
extern char *__getopt_nonoption_flags;

static int nonoption_flags_max_len;
static int nonoption_flags_len;
#endif

static int original_argc;
static char *const *original_argv;

/* Make sure the environment variable bash 2.0 puts in the environment
   is valid for the getopt call we must make sure that the ARGV passed
   to getopt is that one passed to the process.  */
static void
__attribute__ ((unused))
store_args_and_env (int argc, char *const *argv)
{
  /* XXX This is no good solution.  We should rather copy the args so
     that we can compare them later.  But we must not use malloc(3).  */
  original_argc = argc;
  original_argv = argv;
}
# ifdef text_set_element
text_set_element (__libc_subinit, store_args_and_env);
# endif /* text_set_element */

# ifdef USE_NONOPTION_FLAGS
#  define SWAP_FLAGS(ch1, ch2) \
  if (nonoption_flags_len > 0)						      \
    {									      \
      char __tmp = __getopt_nonoption_flags[ch1];			      \
      __getopt_nonoption_flags[ch1] = __getopt_nonoption_flags[ch2];	      \
      __getopt_nonoption_flags[ch2] = __tmp;				      \
    }
# else
#  define SWAP_FLAGS(ch1, ch2)
# endif
#else	/* !_LIBC */
# define SWAP_FLAGS(ch1, ch2)
#endif	/* _LIBC */

/* Exchange two adjacent subsequences of ARGV.
   One subsequence is elements [first_nonopt,last_nonopt)
   which contains all the non-options that have been skipped so far.
   The other is elements [last_nonopt,optind), which contains all
   the options processed since those non-options were skipped.

   `first_nonopt' and `last_nonopt' are relocated so that they describe
   the new indices of the non-options in ARGV after they are moved.  */

#if defined __STDC__ && __STDC__
static void exchange (char **);
#endif

static void
exchange (char **argv)
{
  int bottom = first_nonopt;
  int middle = last_nonopt;
  int top = optind;
  char *tem;

  /* Exchange the shorter segment with the far end of the longer segment.
     That puts the shorter segment into the right place.
     It leaves the longer segment in the right place overall,
     but it consists of two parts that need to be swapped next.  */

#if defined _LIBC && defined USE_NONOPTION_FLAGS
  /* First make sure the handling of the `__getopt_nonoption_flags'
     string can work normally.  Our top argument must be in the range
     of the string.  */
  if (nonoption_flags_len > 0 && top >= nonoption_flags_max_len)
    {
      /* We must extend the array.  The user plays games with us and
	 presents new arguments.  */
      char *new_str = malloc (top + 1);
      if (new_str == NULL)
	nonoption_flags_len = nonoption_flags_max_len = 0;
      else
	{
	  memset (__mempcpy (new_str, __getopt_nonoption_flags,
			     nonoption_flags_max_len),
		  '\0', top + 1 - nonoption_flags_max_len);
	  nonoption_flags_max_len = top + 1;
	  __getopt_nonoption_flags = new_str;
	}
    }
#endif

  while (top > middle && middle > bottom)
    {
      if (top - middle > middle - bottom)
	{
	  /* Bottom segment is the short one.  */
	  int len = middle - bottom;
	  register int i;

	  /* Swap it with the top part of the top segment.  */
	  for (i = 0; i < len; i++)
	    {
	      tem = argv[bottom + i];
	      argv[bottom + i] = argv[top - (middle - bottom) + i];
	      argv[top - (middle - bottom) + i] = tem;
	      SWAP_FLAGS (bottom + i, top - (middle - bottom) + i);
	    }
	  /* Exclude the moved bottom segment from further swapping.  */
	  top -= len;
	}
      else
	{
	  /* Top segment is the short one.  */
	  int len = top - middle;
	  register int i;

	  /* Swap it with the bottom part of the bottom segment.  */
	  for (i = 0; i < len; i++)
	    {
	      tem = argv[bottom + i];
	      argv[bottom + i] = argv[middle + i];
	      argv[middle + i] = tem;
	      SWAP_FLAGS (bottom + i, middle + i);
	    }
	  /* Exclude the moved top segment from further swapping.  */
	  bottom += len;
	}
    }

  /* Update records for the slots the non-options now occupy.  */

  first_nonopt += (optind - last_nonopt);
  last_nonopt = optind;
}

/* Initialize the internal data when the first call is made.  */

#if defined __STDC__ && __STDC__
static const char *_getopt_initialize (int, char *const *, const char *);
#endif
static const char * _getopt_initialize(int argc, char * const *argv, const char *optstring)
{
  /* Start processing options with ARGV-element 1 (since ARGV-element 0
     is the program name); the sequence of previously skipped
     non-option ARGV-elements is empty.  */

  first_nonopt = last_nonopt = optind;

  nextchar = NULL;

  posixly_correct = getenv ("POSIXLY_CORRECT");

  /* Determine how to handle the ordering of options and nonoptions.  */

  if (optstring[0] == '-')
    {
      ordering = RETURN_IN_ORDER;
      ++optstring;
    }
  else if (optstring[0] == '+')
    {
      ordering = REQUIRE_ORDER;
      ++optstring;
    }
  else if (posixly_correct != NULL)
    ordering = REQUIRE_ORDER;
  else
    ordering = PERMUTE;

#if defined _LIBC && defined USE_NONOPTION_FLAGS
  if (posixly_correct == NULL
      && argc == original_argc && argv == original_argv)
    {
      if (nonoption_flags_max_len == 0)
	{
	  if (__getopt_nonoption_flags == NULL
	      || __getopt_nonoption_flags[0] == '\0')
	    nonoption_flags_max_len = -1;
	  else
	    {
	      const char *orig_str = __getopt_nonoption_flags;
	      int len = nonoption_flags_max_len = strlen (orig_str);
	      if (nonoption_flags_max_len < argc)
		nonoption_flags_max_len = argc;
	      __getopt_nonoption_flags =
		(char *) malloc (nonoption_flags_max_len);
	      if (__getopt_nonoption_flags == NULL)
		nonoption_flags_max_len = -1;
	      else
		memset (__mempcpy (__getopt_nonoption_flags, orig_str, len),
			'\0', nonoption_flags_max_len - len);
	    }
	}
      nonoption_flags_len = nonoption_flags_max_len;
    }
  else
    nonoption_flags_len = 0;
#endif

  return optstring;
}

int
_getopt_internal(int argc, char * const *argv, const char *optstring,
    const struct option *longopts, int *longind, int long_only)
{
  int print_errors = opterr;
  if (optstring[0] == ':')
    print_errors = 0;

  if (argc < 1)
    return -1;

  optarg = NULL;

  if (optind == 0 || !__getopt_initialized)
    {
      if (optind == 0)
    optind = 1;
      optstring = _getopt_initialize (argc, argv, optstring);
      __getopt_initialized = 1;
    }


#if defined _LIBC && defined USE_NONOPTION_FLAGS
# define NONOPTION_P (argv[optind][0] != '-' || argv[optind][1] == '\0'	      \
		      || (optind < nonoption_flags_len			      \
			  && __getopt_nonoption_flags[optind] == '1'))
#else
# define NONOPTION_P (argv[optind][0] != '-' || argv[optind][1] == '\0')
#endif

  if (nextchar == NULL || *nextchar == '\0')
    {
      if (last_nonopt > optind)
	last_nonopt = optind;
      if (first_nonopt > optind)
	first_nonopt = optind;

      if (ordering == PERMUTE)
	{

	  if (first_nonopt != last_nonopt && last_nonopt != optind)
	    exchange ((char **) argv);
	  else if (last_nonopt != optind)
	    first_nonopt = optind;

	 

	  while (optind < argc && NONOPTION_P)
	    optind++;
	  last_nonopt = optind;
	}

      

      if (optind != argc && !strcmp (argv[optind], "--"))
	{
	  optind++;

	  if (first_nonopt != last_nonopt && last_nonopt != optind)
	    exchange ((char **) argv);
	  else if (first_nonopt == last_nonopt)
	    first_nonopt = optind;
	  last_nonopt = argc;

	  optind = argc;
	}


      if (optind == argc)
	{
	  if (first_nonopt != last_nonopt)
	    optind = first_nonopt;
	  return -1;
	}


      if (NONOPTION_P)
	{
	  if (ordering == REQUIRE_ORDER)
	    return -1;
	  optarg = argv[optind++];
	  return 1;
	}


      nextchar = (argv[optind] + 1
		  + (longopts != NULL && argv[optind][1] == '-'));
    }



  if (longopts != NULL
      && (argv[optind][1] == '-'
	  || (long_only && (argv[optind][2] || !my_index (optstring, argv[optind][1])))))
    {
      char *nameend;
      const struct option *p;
      const struct option *pfound = NULL;
      int exact = 0;
      int ambig = 0;
      int indfound = -1;
      int option_index;

      for (nameend = nextchar; *nameend && *nameend != '='; nameend++)
      for (p = longopts, option_index = 0; p->name; p++, option_index++)
	if (!strncmp (p->name, nextchar, nameend - nextchar))
	  {
	    if ((unsigned int) (nameend - nextchar)
		== (unsigned int) strlen (p->name))
	      {
		pfound = p;
		indfound = option_index;
		exact = 1;
		break;
	      }
	    else if (pfound == NULL)
	      {
		pfound = p;
		indfound = option_index;
	      }
	    else if (long_only
		     || pfound->has_arg != p->has_arg
		     || pfound->flag != p->flag
		     || pfound->val != p->val)
	      ambig = 1;
	  }

      if (ambig && !exact)
	{
	  if (print_errors)
	    fprintf (stderr, _("%s: option `%s' is ambiguous\n"),
		     argv[0], argv[optind]);
	  nextchar += strlen (nextchar);
	  optind++;
	  optopt = 0;
	  return '?';
	}

      if (pfound != NULL)
	{
	  option_index = indfound;
	  optind++;
	  if (*nameend)
	    {
	      if (pfound->has_arg)
		optarg = nameend + 1;
	      else
		{
		  if (print_errors)
		    {
		      if (argv[optind - 1][1] == '-')
			fprintf (stderr,
				 _("%s: option `--%s' doesn't allow an argument\n"),
				 argv[0], pfound->name);
		      else
			fprintf (stderr,
				 _("%s: option `%c%s' doesn't allow an argument\n"),
				 argv[0], argv[optind - 1][0], pfound->name);
		    }

		  nextchar += strlen (nextchar);

		  optopt = pfound->val;
		  return '?';
		}
	    }
	  else if (pfound->has_arg == 1)
	    {
	      if (optind < argc)
		optarg = argv[optind++];
	      else
		{
		  if (print_errors)
		    fprintf (stderr,
			   _("%s: option `%s' requires an argument\n"),
			   argv[0], argv[optind - 1]);
		  nextchar += strlen (nextchar);
		  optopt = pfound->val;
		  return optstring[0] == ':' ? ':' : '?';
		}
	    }
	  nextchar += strlen (nextchar);
	  if (longind != NULL)
	    *longind = option_index;
	  if (pfound->flag)
	    {
	      *(pfound->flag) = pfound->val;
	      return 0;
	    }
	  return pfound->val;
	}

      
      if (!long_only || argv[optind][1] == '-'
	  || my_index (optstring, *nextchar) == NULL)
	{
	  if (print_errors)
	    {
	      if (argv[optind][1] == '-')
		/* --option */
		fprintf (stderr, _("%s: unrecognized option `--%s'\n"),
			 argv[0], nextchar);
	      else
		/* +option or -option */
		fprintf (stderr, _("%s: unrecognized option `%c%s'\n"),
			 argv[0], argv[optind][0], nextchar);
	    }
	  nextchar = (char *) "";
	  optind++;
	  optopt = 0;
	  return '?';
	}
    }

  {
    char c = *nextchar++;
    char *temp = my_index (optstring, c);

    if (*nextchar == '\0')
      ++optind;

    if (temp == NULL || c == ':')
      {
	if (print_errors)
	  {
	    if (posixly_correct)
	      /* 1003.2 specifies the format of this message.  */
	      fprintf (stderr, _("%s: illegal option -- %c\n"),
		       argv[0], c);
	    else
	      fprintf (stderr, _("%s: invalid option -- %c\n"),
		       argv[0], c);
	  }
	optopt = c;
	return '?';
      }
    /* Convenience. Treat POSIX -W foo same as long option --foo */
    if (temp[0] == 'W' && temp[1] == ';')
      {
	char *nameend;
	const struct option *p;
	const struct option *pfound = NULL;
	int exact = 0;
	int ambig = 0;
	int indfound = 0;
	int option_index;

	/* This is an option that requires an argument.  */
	if (*nextchar != '\0')
	  {
	    optarg = nextchar;
	    /* If we end this ARGV-element by taking the rest as an arg,
	       we must advance to the next element now.  */
	    optind++;
	  }
	else if (optind == argc)
	  {
	    if (print_errors)
	      {
		/* 1003.2 specifies the format of this message.  */
		fprintf (stderr, _("%s: option requires an argument -- %c\n"),
			 argv[0], c);
	      }
	    optopt = c;
	    if (optstring[0] == ':')
	      c = ':';
	    else
	      c = '?';
	    return c;
	  }
	else
	  /* We already incremented `optind' once;
	     increment it again when taking next ARGV-elt as argument.  */
	  optarg = argv[optind++];

	/* optarg is now the argument, see if it's in the
	   table of longopts.  */

	for (nextchar = nameend = optarg; *nameend && *nameend != '='; nameend++)
	  /* Do nothing.  */ ;

	/* Test all long options for either exact match
	   or abbreviated matches.  */
	for (p = longopts, option_index = 0; p->name; p++, option_index++)
	  if (!strncmp (p->name, nextchar, nameend - nextchar))
	    {
	      if ((unsigned int) (nameend - nextchar) == strlen (p->name))
		{
		  /* Exact match found.  */
		  pfound = p;
		  indfound = option_index;
		  exact = 1;
		  break;
		}
	      else if (pfound == NULL)
		{
		  /* First nonexact match found.  */
		  pfound = p;
		  indfound = option_index;
		}
	      else
		/* Second or later nonexact match found.  */
		ambig = 1;
	    }
	if (ambig && !exact)
	  {
	    if (print_errors)
	      fprintf (stderr, _("%s: option `-W %s' is ambiguous\n"),
		       argv[0], argv[optind]);
	    nextchar += strlen (nextchar);
	    optind++;
	    return '?';
	  }
	if (pfound != NULL)
	  {
	    option_index = indfound;
	    if (*nameend)
	      {
		/* Don't test has_arg with >, because some C compilers don't
		   allow it to be used on enums.  */
		if (pfound->has_arg)
		  optarg = nameend + 1;
		else
		  {
		    if (print_errors)
		      fprintf (stderr, _("\
%s: option `-W %s' doesn't allow an argument\n"),
			       argv[0], pfound->name);

		    nextchar += strlen (nextchar);
		    return '?';
		  }
	      }
	    else if (pfound->has_arg == 1)
	      {
		if (optind < argc)
		  optarg = argv[optind++];
		else
		  {
		    if (print_errors)
		      fprintf (stderr,
			       _("%s: option `%s' requires an argument\n"),
			       argv[0], argv[optind - 1]);
		    nextchar += strlen (nextchar);
		    return optstring[0] == ':' ? ':' : '?';
		  }
	      }
	    nextchar += strlen (nextchar);
	    if (longind != NULL)
	      *longind = option_index;
	    if (pfound->flag)
	      {
		*(pfound->flag) = pfound->val;
		return 0;
	      }
	    return pfound->val;
	  }
	  nextchar = NULL;
	  return 'W';	/* Let the application handle it.   */
      }
    if (temp[1] == ':')
      {
	if (temp[2] == ':')
	  {
	    /* This is an option that accepts an argument optionally.  */
	    if (*nextchar != '\0')
	      {
		optarg = nextchar;
		optind++;
	      }
	    else
	      optarg = NULL;
	    nextchar = NULL;
	  }
	else
	  {
	    /* This is an option that requires an argument.  */
	    if (*nextchar != '\0')
	      {
		optarg = nextchar;
		/* If we end this ARGV-element by taking the rest as an arg,
		   we must advance to the next element now.  */
		optind++;
	      }
	    else if (optind == argc)
	      {
		if (print_errors)
		  {
		    /* 1003.2 specifies the format of this message.  */
		    fprintf (stderr,
			     _("%s: option requires an argument -- %c\n"),
			     argv[0], c);
		  }
		optopt = c;
		if (optstring[0] == ':')
		  c = ':';
		else
		  c = '?';
	      }
	    else
	      /* We already incremented `optind' once;
		 increment it again when taking next ARGV-elt as argument.  */
	      optarg = argv[optind++];
	    nextchar = NULL;
	  }
      }
    return c;
  }
}

int
getopt (int argc, char * const *argv, const char *optstring)
{
  return _getopt_internal (argc, argv, optstring,
			   (const struct option *) 0,
			   (int *) 0,
			   0);
}
#if !defined __STDC__ || !__STDC__
#ifndef const
#define const
#endif
#endif

static char  *license_msg[] = {
(char *)"Copyright 2002 Free Software Foundation",
(char *)"Copyright 1992-1993 Jean-loup Gailly",
(char *)"This program comes with ABSOLUTELY NO WARRANTY.",
(char *)"You may redistribute copies of this program",
(char *)"under the terms of the GNU General Public License.",
(char *)"For more information about these matters, see the file named COPYING.",
0};


#ifdef RCSID
static char rcsid[] = "$Id: gzip.c,v 0.24 1993/06/24 10:52:07 jloup Exp $";
#endif


#define PATCHLEVEL 0
#define REVDATE "2002-09-30"
#ifdef LZW
#  undef LZW
#endif


#  define NAMLEN(direct) strlen((direct)->d_name)
#  define DIR_OPT "DIRENT"
#ifndef NO_DIR
# define NO_DIR 0
#endif

#ifdef CLOSEDIR_VOID
# define CLOSEDIR(d) (closedir(d), 0)
#else
# define CLOSEDIR(d) closedir(d)
#endif

#if !defined(HAVE_LSTAT) && !defined(lstat)
# define lstat(name, buf) stat(name, buf)
#endif

#    define TIME_OPT "UTIME"

#if !defined(S_ISDIR) && defined(S_IFDIR)
#  define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
#if !defined(S_ISREG) && defined(S_IFREG)
#  define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

typedef RETSIGTYPE (*sig_type) OF((int));

#ifndef	O_BINARY
#  define  O_BINARY  0
#endif


#ifndef S_IRUSR
#  define S_IRUSR 0400
#endif
#ifndef S_IWUSR
#  define S_IWUSR 0200
#endif
#define RW_USER (S_IRUSR | S_IWUSR) 

#ifndef MAX_PATH_LEN
#  define MAX_PATH_LEN   1024 
#endif

#ifndef SEEK_END
#  define SEEK_END 2
#endif

#ifndef CHAR_BIT
#  define CHAR_BIT 8
#endif

#ifdef off_t
  off_t lseek OF((int fd, off_t offset, int whence));
#endif

#ifndef OFF_T_MIN
#define OFF_T_MIN (~ (off_t) 0 << (sizeof (off_t) * CHAR_BIT - 1))
#endif

#ifndef OFF_T_MAX
#define OFF_T_MAX (~ (off_t) 0 - OFF_T_MIN)
#endif
#ifdef NO_MULTIPLE_DOTS
#  define PART_SEP "-"
#else
#  define PART_SEP "."
#endif

DECLARE(uch, inbuf,  INBUFSIZ +INBUF_EXTRA);
DECLARE(uch, outbuf, OUTBUFSIZ+OUTBUF_EXTRA);
DECLARE(ush, d_buf,  DIST_BUFSIZE);
DECLARE(uch, window, 2L*WSIZE);
#ifndef MAXSEG_64K
    DECLARE(ush, tab_prefix, 1L<<BITS);
#else
    DECLARE(ush, tab_prefix0, 1L<<(BITS-1));
    DECLARE(ush, tab_prefix1, 1L<<(BITS-1));
#endif

int ascii = 0;        
int to_stdout = 0;    
int decompress = 0;   
int force = 0;        
int no_name = -1;     
int no_time = -1;     
int recursive = 0;    
int list = 0;         
int verbose = 0;      
int quiet = 0;        
int do_lzw = 0;       
int test = 0;         
int foreground;       
char *progname;       
int maxbits = BITS;   
int method = DEFLATED;
int level = 6;        
int exit_code = OK;   
int save_orig_name;   
int last_member;      
int part_nb;          
time_t time_stamp;   
off_t ifile_size;    
char *env;           
char **args = NULL;  
char *z_suffix;      
size_t z_len;        

off_t bytes_in;    
off_t bytes_out;   
off_t total_in;		
off_t total_out;	
char ifname[MAX_PATH_LEN];
char ofname[MAX_PATH_LEN];
int  remove_ofname = 0;	  
struct stat istat;        
int  ifd;                 
int  ofd;                 
unsigned insize;          
unsigned inptr;           
unsigned outcnt;          
int rsync = 0;            

struct option longopts[] =
{
    {"ascii",      0, 0, 'a'}, 
    {"to-stdout",  0, 0, 'c'}, 
    {"stdout",     0, 0, 'c'}, 
    {"decompress", 0, 0, 'd'}, 
    {"uncompress", 0, 0, 'd'}, 
    {"force",      0, 0, 'f'}, 
    {"help",       0, 0, 'h'}, 
    {"list",       0, 0, 'l'}, 
    {"license",    0, 0, 'L'}, 
    {"no-name",    0, 0, 'n'}, 
    {"name",       0, 0, 'N'}, 
    {"quiet",      0, 0, 'q'}, 
    {"silent",     0, 0, 'q'}, 
    {"recursive",  0, 0, 'r'}, 
    {"suffix",     1, 0, 'S'}, 
    {"test",       0, 0, 't'}, 
    {"no-time",    0, 0, 'T'}, 
    {"verbose",    0, 0, 'v'}, 
    {"version",    0, 0, 'V'}, 
    {"fast",       0, 0, '1'}, 
    {"best",       0, 0, '9'}, 
    {"lzw",        0, 0, 'Z'}, 
    {"bits",       1, 0, 'b'}, 
    {"rsyncable",  0, 0, 'R'}, 
    { 0, 0, 0, 0 }
};

local void usage        OF((void));
local void help         OF((void));
local void license      OF((void));
local void version      OF((void));
local int input_eof	OF((void));
local void treat_stdin  OF((void));
local void treat_file   OF((char *iname));
local int create_outfile OF((void));
local int  do_stat      OF((char *name, struct stat *sbuf));
local char *get_suffix  OF((char *name));
local int  get_istat    OF((char *iname, struct stat *sbuf));
local int  make_ofname  OF((void));
local int  same_file    OF((struct stat *stat1, struct stat *stat2));
local int name_too_long OF((char *name, struct stat *statb));
local void shorten_name  OF((char *name));
local int  get_method   OF((int in));
local void do_list      OF((int ifd, int method));
local int  check_ofname OF((void));
local void copy_stat    OF((struct stat *ifstat));
local void do_exit      OF((int exitcode));
      int main          OF((int argc, char **argv));
int (*work) OF((int infile, int outfile)) = zip;

#if ! NO_DIR
local void treat_dir    OF((char *dir));
#endif
#ifdef HAVE_UTIME
local void reset_times  OF((char *name, struct stat *statb));
#endif

#define strequ(s1, s2) (strcmp((s1),(s2)) == 0)

local void usage()
{
    printf ("usage: %s [-%scdfhlLnN%stvV19] [-S suffix] [file ...]\n",
	    progname,
	    O_BINARY ? "a" : "", NO_DIR ? "" : "r");
}

local void help()
{
    static char  *help_msg[] = {
#if O_BINARY
 (char *)" -a --ascii       ascii text; convert end-of-lines using local conventions",
#endif
 (char *)" -c --stdout      write on standard output, keep original files unchanged",
 (char *)" -d --decompress  decompress",
 (char *)" -f --force       force overwrite of output file and compress links",
 (char *)" -h --help        give this help",
 (char *)" -l --list        list compressed file contents",
 (char *)" -L --license     display software license",
#ifdef UNDOCUMENTED
 (char *)" -m --no-time     do not save or restore the original modification time",
 (char *)" -M --time        save or restore the original modification time",
#endif
 (char *)" -n --no-name     do not save or restore the original name and time stamp",
 (char *)" -N --name        save or restore the original name and time stamp",
 (char *)" -q --quiet       suppress all warnings",
#if ! NO_DIR
 (char *)" -r --recursive   operate recursively on directories",
#endif
 (char *)" -S .suf  --suffix .suf     use suffix .suf on compressed files",
 (char *)" -t --test        test compressed file integrity",
 (char *)" -v --verbose     verbose mode",
 (char *)" -V --version     display version number",
 (char *)" -1 --fast        compress faster",
 (char *)" -9 --best        compress better",
#ifdef LZW
 (char *)" -Z --lzw         produce output compatible with old compress",
 (char *)" -b --bits maxbits   max number of bits per code (implies -Z)",
#endif
 (char *)"    --rsyncable   Make rsync-friendly archive",
 (char *)" file...          files to (de)compress. If none given, use standard input.",
 (char *)"Report bugs to <bug-gzip@gnu.org>.",
  0};
    char **p = help_msg;

    printf ("%s %s\n(%s)\n", progname, VERSION, REVDATE);
    usage();
    while (*p) printf ("%s\n", *p++);
}

local void license()
{
    char **p = license_msg;

    printf ("%s %s\n(%s)\n", progname, VERSION, REVDATE);
    while (*p) printf ("%s\n", *p++);
}

local void version()
{
    license ();
    printf ("Compilation options:\n%s %s ", DIR_OPT, TIME_OPT);
#ifdef STDC_HEADERS
    printf ("STDC_HEADERS ");
#endif
#ifdef HAVE_UNISTD_H
    printf ("HAVE_UNISTD_H ");
#endif
#ifdef HAVE_MEMORY_H
    printf ("HAVE_MEMORY_H ");
#endif
#ifdef HAVE_STRING_H
    printf ("HAVE_STRING_H ");
#endif
#ifdef HAVE_LSTAT
    printf ("HAVE_LSTAT ");
#endif
#ifdef NO_MULTIPLE_DOTS
    printf ("NO_MULTIPLE_DOTS ");
#endif
#ifdef HAVE_CHOWN
    printf ("HAVE_CHOWN ");
#endif
#ifdef PROTO
    printf ("PROTO ");
#endif
#ifdef ASMV
    printf ("ASMV ");
#endif
#ifdef DEBUG
    printf ("DEBUG ");
#endif
#ifdef DYN_ALLOC
    printf ("DYN_ALLOC ");
#endif
#ifdef MAXSEG_64K
    printf ("MAXSEG_64K");
#endif
    printf ("\n");
    printf ("Written by Jean-loup Gailly.\n");
}

local void progerror (char *string)
{
    int e = errno;
    fprintf(stderr, "%s: ", progname);
    errno = e;
    perror(string);
    exit_code = ERROR;
}

int main(int argc, char **argv)
{
    int file_count;
    int proglen;
    int optc;

    EXPAND(argc, argv);

    progname = base_name (argv[0]);
    proglen = strlen(progname);

    if (proglen > 4 && strequ(progname+proglen-4, ".exe")) {
        progname[proglen-4] = '\0';
    }

    /* Add options in GZIP environment variable if there is one */
    env = add_envopt(&argc, &argv, OPTIONS_VAR);
    if (env != NULL) args = argv;

    foreground = signal(SIGINT, SIG_IGN) != SIG_IGN;
    if (foreground) {
	(void) signal (SIGINT, (sig_type)abort_gzip_signal);
    }
#ifdef SIGTERM
    if (signal(SIGTERM, SIG_IGN) != SIG_IGN) {
	(void) signal(SIGTERM, (sig_type)abort_gzip_signal);
    }
#endif
#ifdef SIGHUP
    if (signal(SIGHUP, SIG_IGN) != SIG_IGN) {
	(void) signal(SIGHUP,  (sig_type)abort_gzip_signal);
    }
#endif

#ifndef GNU_STANDARD
    /* For compatibility with old compress, use program name as an option.
     * If you compile with -DGNU_STANDARD, this program will behave as
     * gzip even if it is invoked under the name gunzip or zcat.
     *
     * Systems which do not support links can still use -d or -dc.
     * Ignore an .exe extension for MSDOS, OS/2 and VMS.
     */
    if (  strncmp(progname, "un",  2) == 0     /* ungzip, uncompress */
       || strncmp(progname, "gun", 3) == 0) {  /* gunzip */
	decompress = 1;
    } else if (strequ(progname+1, "cat")       /* zcat, pcat, gcat */
	    || strequ(progname, "gzcat")) {    /* gzcat */
	decompress = to_stdout = 1;
    }
#endif

    z_suffix = Z_SUFFIX;
    z_len = strlen(z_suffix);

    while ((optc = getopt_long (argc, argv, "ab:cdfhH?lLmMnNqrS:tvVZ123456789",
				longopts, (int *)0)) != -1) {
	switch (optc) {
        case 'a':
            ascii = 1; break;
	case 'b':
	    maxbits = atoi(optarg);
	    for (; *optarg; optarg++)
	      if (! ('0' <= *optarg && *optarg <= '9'))
		{
		  fprintf (stderr, "%s: -b operand is not an integer\n",
			   progname);
		  usage ();
		  do_exit (ERROR);
		}
	    break;
	case 'c':
	    to_stdout = 1; break;
	case 'd':
	    decompress = 1; break;
	case 'f':
	    force++; break;
	case 'h': case 'H': case '?':
	    help(); do_exit(OK); break;
	case 'l':
	    list = decompress = to_stdout = 1; break;
	case 'L':
	    license(); do_exit(OK); break;
	case 'm': /* undocumented, may change later */
	    no_time = 1; break;
	case 'M': /* undocumented, may change later */
	    no_time = 0; break;
	case 'n':
	    no_name = no_time = 1; break;
	case 'N':
	    no_name = no_time = 0; break;
	case 'q':
	    quiet = 1; verbose = 0; break;
	case 'r':
#if NO_DIR
	    fprintf(stderr, "%s: -r not supported on this system\n", progname);
	    usage();
	    do_exit(ERROR); break;
#else
	    recursive = 1; break;
#endif
	case 'R':
	    rsync = 1; break;

	case 'S':
#ifdef NO_MULTIPLE_DOTS
            if (*optarg == '.') optarg++;
#endif
            z_len = strlen(optarg);
	    z_suffix = optarg;
            break;
	case 't':
	    test = decompress = to_stdout = 1;
	    break;
	case 'v':
	    verbose++; quiet = 0; break;
	case 'V':
	    version(); do_exit(OK); break;
	case 'Z':
#ifdef LZW
	    do_lzw = 1; break;
#else
	    fprintf(stderr, "%s: -Z not supported in this version\n",
		    progname);
	    usage();
	    do_exit(ERROR); break;
#endif
	case '1':  case '2':  case '3':  case '4':
	case '5':  case '6':  case '7':  case '8':  case '9':
	    level = optc - '0';
	    break;
	default:
	    /* Error message already emitted by getopt_long. */
	    usage();
	    do_exit(ERROR);
	}
    } /* loop on all arguments */

#ifdef SIGPIPE
    /* Ignore "Broken Pipe" message with --quiet */
    if (quiet && signal (SIGPIPE, SIG_IGN) != SIG_IGN)
      signal (SIGPIPE, (sig_type) abort_gzip_signal);
#endif

    /* By default, save name and timestamp on compression but do not
     * restore them on decompression.
     */
    if (no_time < 0) no_time = decompress;
    if (no_name < 0) no_name = decompress;

    file_count = argc - optind;

#if O_BINARY
#else
    if (ascii && !quiet) {
	fprintf(stderr, "%s: option --ascii ignored on this system\n",
		progname);
    }
#endif
    if ((z_len == 0 && !decompress) || z_len > MAX_SUFFIX) {
        fprintf(stderr, "%s: incorrect suffix '%s'\n",
                progname, z_suffix);
        do_exit(ERROR);
    }
    if (do_lzw && !decompress) work = lzw;

    /* Allocate all global buffers (for DYN_ALLOC option) */
    ALLOC(uch, inbuf,  INBUFSIZ +INBUF_EXTRA);
    ALLOC(uch, outbuf, OUTBUFSIZ+OUTBUF_EXTRA);
    ALLOC(ush, d_buf,  DIST_BUFSIZE);
    ALLOC(uch, window, 2L*WSIZE);
#ifndef MAXSEG_64K
    ALLOC(ush, tab_prefix, 1L<<BITS);
#else
    ALLOC(ush, tab_prefix0, 1L<<(BITS-1));
    ALLOC(ush, tab_prefix1, 1L<<(BITS-1));
#endif

    /* And get to work */
    if (file_count != 0) {
	if (to_stdout && !test && !list && (!decompress || !ascii)) {
	    SET_BINARY_MODE(fileno(stdout));
	}
        while (optind < argc) {
	    treat_file(argv[optind++]);
	}
    } else {  /* Standard input */
	treat_stdin();
    }
    if (list && !quiet && file_count > 1) {
	do_list(-1, -1); /* print totals */
    }
    do_exit(exit_code);
    return exit_code; /* just to avoid lint warning */
}

/* Return nonzero when at end of file on input.  */
local int
input_eof ()
{
  if (!decompress || last_member)
    return 1;

  if (inptr == insize)
    {
      if (insize != INBUFSIZ || fill_inbuf (1) == EOF)
	return 1;

      /* Unget the char that fill_inbuf got.  */
      inptr = 0;
    }

  return 0;
}

/* ========================================================================
 * Compress or decompress stdin
 */
local void treat_stdin()
{
    if (!force && !list &&
	isatty(fileno((FILE *)(decompress ? stdin : stdout)))) {
	/* Do not send compressed data to the terminal or read it from
	 * the terminal. We get here when user invoked the program
	 * without parameters, so be helpful. According to the GNU standards:
	 *
	 *   If there is one behavior you think is most useful when the output
	 *   is to a terminal, and another that you think is most useful when
	 *   the output is a file or a pipe, then it is usually best to make
	 *   the default behavior the one that is useful with output to a
	 *   terminal, and have an option for the other behavior.
	 *
	 * Here we use the --force option to get the other behavior.
	 */
	fprintf(stderr,
    "%s: compressed data not %s a terminal. Use -f to force %scompression.\n",
		progname, decompress ? "read from" : "written to",
		decompress ? "de" : "");
	fprintf(stderr,"For help, type: %s -h\n", progname);
	do_exit(ERROR);
    }

    if (decompress || !ascii) {
	SET_BINARY_MODE(fileno(stdin));
    }
    if (!test && !list && (!decompress || !ascii)) {
	SET_BINARY_MODE(fileno(stdout));
    }
    strcpy(ifname, "stdin");
    strcpy(ofname, "stdout");

    time_stamp = 0;

#ifndef NO_STDIN_FSTAT
    if (list || !no_time) {
	if (fstat(fileno(stdin), &istat) != 0) {
	    progerror((char *)"standard input");
	    do_exit(ERROR);
	}
# ifdef NO_PIPE_TIMESTAMP
	if (S_ISREG(istat.st_mode))
# endif
	    time_stamp = istat.st_mtime;
#endif
    }
    ifile_size = -1L;
    clear_bufs();
    to_stdout = 1;
    part_nb = 0;

    if (decompress) {
	method = get_method(ifd);
	if (method < 0) {
	    do_exit(exit_code);
	}
    }
    if (list) {
        do_list(ifd, method);
        return;
    }

    for (;;) {
	if ((*work)(fileno(stdin), fileno(stdout)) != OK) return;

	if (input_eof ())
	  break;

	method = get_method(ifd);
	if (method < 0) return;
	bytes_out = 0;
    }

    if (verbose) {
	if (test) {
	    fprintf(stderr, " OK\n");

	} else if (!decompress) {
	    display_ratio(bytes_in-(bytes_out-header_bytes), bytes_in, stderr);
	    fprintf(stderr, "\n");
#ifdef DISPLAY_STDIN_RATIO
	} else {
	    display_ratio(bytes_out-(bytes_in-header_bytes), bytes_out,stderr);
	    fprintf(stderr, "\n");
#endif
	}
    }
}

local void treat_file(char *iname)
{
    if (strequ(iname, "-")) {
	int cflag = to_stdout;
	treat_stdin();
	to_stdout = cflag;
	return;
    }

    if (get_istat(iname, &istat) != OK) return;

    if (S_ISDIR(istat.st_mode)) {
#if ! NO_DIR
	if (recursive) {
	    struct stat st;
	    st = istat;
	    treat_dir(iname);
#  ifndef NO_UTIME
	    reset_times (iname, &st);
#  endif
	} else
#endif
	WARN((stderr,"%s: %s is a directory -- ignored\n", progname, ifname));
	return;
    }
    if (!S_ISREG(istat.st_mode)) {
	WARN((stderr,
	      "%s: %s is not a directory or a regular file - ignored\n",
	      progname, ifname));
	return;
    }
    if (istat.st_nlink > 1 && !to_stdout && !force) {
	WARN((stderr, "%s: %s has %lu other link%c -- unchanged\n",
	      progname, ifname, (unsigned long) istat.st_nlink - 1,
	      istat.st_nlink > 2 ? 's' : ' '));
	return;
    }

    ifile_size = istat.st_size;
    time_stamp = no_time && !list ? 0 : istat.st_mtime;

    /* Generate output file name. For -r and (-t or -l), skip files
     * without a valid gzip suffix (check done in make_ofname).
     */
    if (to_stdout && !list && !test) {
	strcpy(ofname, "stdout");

    } else if (make_ofname() != OK) {
	return;
    }

    /* Open the input file and determine compression method. The mode
     * parameter is ignored but required by some systems (VMS) and forbidden
     * on other systems (MacOS).
     */
    ifd = OPEN(ifname, ascii && !decompress ? O_RDONLY : O_RDONLY | O_BINARY,
	       RW_USER);
    if (ifd == -1) {
	progerror(ifname);
	return;
    }
    clear_bufs(); /* clear input and output buffers */
    part_nb = 0;

    if (decompress) {
	method = get_method(ifd); /* updates ofname if original given */
	if (method < 0) {
	    close(ifd);
	    return;               /* error message already emitted */
	}
    }
    if (list) {
        do_list(ifd, method);
        close(ifd);
        return;
    }

    /* If compressing to a file, check if ofname is not ambiguous
     * because the operating system truncates names. Otherwise, generate
     * a new ofname and save the original name in the compressed file.
     */
    if (to_stdout) {
	ofd = fileno(stdout);
	/* keep remove_ofname as zero */
    } else {
	if (create_outfile() != OK) return;

	if (!decompress && save_orig_name && !verbose && !quiet) {
	    fprintf(stderr, "%s: %s compressed to %s\n",
		    progname, ifname, ofname);
	}
    }
    /* Keep the name even if not truncated except with --no-name: */
    if (!save_orig_name) save_orig_name = !no_name;

    if (verbose) {
	fprintf(stderr, "%s:\t", ifname);
    }

    /* Actually do the compression/decompression. Loop over zipped members.
     */
    for (;;) {
	if ((*work)(ifd, ofd) != OK) {
	    method = -1; /* force cleanup */
	    break;
	}

	if (input_eof ())
	  break;

	method = get_method(ifd);
	if (method < 0) break;    /* error message already emitted */
	bytes_out = 0;            /* required for length check */
    }

    close(ifd);
    if (!to_stdout) {
         /* Copy modes, times, ownership, and remove the input file */
         copy_stat(&istat);
         if (close(ofd))
            write_error();
    }
    if (method == -1) {
	if (!to_stdout) xunlink (ofname);
	return;
    }
    /* Display statistics */
    if(verbose) {
	if (test) {
	    fprintf(stderr, " OK");
	} else if (decompress) {
	    display_ratio(bytes_out-(bytes_in-header_bytes), bytes_out,stderr);
	} else {
	    display_ratio(bytes_in-(bytes_out-header_bytes), bytes_in, stderr);
	}
	if (!test && !to_stdout) {
	    fprintf(stderr, " -- replaced with %s", ofname);
	}
	fprintf(stderr, "\n");
    }
}

/* ========================================================================
 * Create the output file. Return OK or ERROR.
 * Try several times if necessary to avoid truncating the z_suffix. For
 * example, do not create a compressed file of name "1234567890123."
 * Sets save_orig_name to true if the file name has been truncated.
 * IN assertions: the input file has already been open (ifd is set) and
 *   ofname has already been updated if there was an original name.
 * OUT assertions: ifd and ofd are closed in case of error.
 */
local int create_outfile()
{
    struct stat	ostat; /* stat for ofname */
    int flags = O_WRONLY | O_CREAT | O_EXCL | O_BINARY;

    if (ascii && decompress) {
	flags &= ~O_BINARY; /* force ascii text mode */
    }
    for (;;) {
	/* Make sure that ofname is not an existing file */
	if (check_ofname() != OK) {
	    close(ifd);
	    return ERROR;
	}
	/* Create the output file */
	remove_ofname = 1;
	ofd = OPEN(ofname, flags, RW_USER);
	if (ofd == -1) {
	    progerror(ofname);
	    close(ifd);
	    return ERROR;
	}

	/* Check for name truncation on new file (1234567890123.gz) */
#ifdef NO_FSTAT
	if (stat(ofname, &ostat) != 0) {
#else
	if (fstat(ofd, &ostat) != 0) {
#endif
	    progerror(ofname);
	    close(ifd); close(ofd);
	    xunlink (ofname);
	    return ERROR;
	}
	if (!name_too_long(ofname, &ostat)) return OK;

	if (decompress) {
	    /* name might be too long if an original name was saved */
	    WARN((stderr, "%s: %s: warning, name truncated\n",
		  progname, ofname));
	    return OK;
	}
	close(ofd);
	xunlink (ofname);
#ifdef NO_MULTIPLE_DOTS
	/* Should never happen, see check_ofname() */
	fprintf(stderr, "%s: %s: name too long\n", progname, ofname);
	do_exit(ERROR);
#endif
	shorten_name(ofname);
    }
}

/* ========================================================================
 * Use lstat if available, except for -c or -f. Use stat otherwise.
 * This allows links when not removing the original file.
 */
local int do_stat(char *name, struct stat *sbuf)
{
    errno = 0;
    if (!to_stdout && !force) {
	return lstat(name, sbuf);
    }
    return stat(name, sbuf);
}

local char *get_suffix(char *name)
{
    int nlen, slen;
    char suffix[MAX_SUFFIX+3];
    static char *known_suffixes[] =
       {NULL, (char *)".gz", (char *)".z", (char *)".taz", (char *)".tgz",
        (char *)"-gz", "-z", "_z",
#ifdef MAX_EXT_CHARS
          (char *)"z",
#endif
          NULL};
    char **suf = known_suffixes;

    *suf = z_suffix;
    if (strequ(z_suffix, "z")) suf++;

#ifdef SUFFIX_SEP
    {
	char *v = strrchr(name, SUFFIX_SEP);
 	if (v != NULL) *v = '\0';
    }
#endif
    nlen = strlen(name);
    if (nlen <= MAX_SUFFIX+2) {
        strcpy(suffix, name);
    } else {
        strcpy(suffix, name+nlen-MAX_SUFFIX-2);
    }
    strlwr(suffix);
    slen = strlen(suffix);
    do {
       int s = strlen(*suf);
       if (slen > s && suffix[slen-s-1] != PATH_SEP
           && strequ(suffix + slen - s, *suf)) {
           return name+nlen-s;
       }
    } while (*++suf != NULL);

    return NULL;
}

local int get_istat(char *iname, struct stat *sbuf)
{
    int ilen;
    int z_suffix_errno = 0;
    static char *suffixes[] = {NULL, (char *)".gz", (char *)".z",
            (char *)"-z", (char *)".Z", NULL};
    char **suf = suffixes;
    char *s;
#ifdef NO_MULTIPLE_DOTS
    char *dot;
#endif

    *suf = z_suffix;

    if (sizeof ifname - 1 <= strlen (iname))
	goto name_too_long;

    strcpy(ifname, iname);
    if (do_stat(ifname, sbuf) == 0) return OK;

    if (!decompress || errno != ENOENT) {
	progerror(ifname);
	return ERROR;
    }
    s = get_suffix(ifname);
    if (s != NULL) {
	progerror(ifname);
	return ERROR;
    }
#ifdef NO_MULTIPLE_DOTS
    dot = strrchr(ifname, '.');
    if (dot == NULL) {
        strcat(ifname, ".");
        dot = strrchr(ifname, '.');
    }
#endif
    ilen = strlen(ifname);
    if (strequ(z_suffix, ".gz")) suf++;

    do {
        char *s0 = s = *suf;
        strcpy (ifname, iname);
#ifdef NO_MULTIPLE_DOTS
        if (*s == '.') s++;
        if (*dot == '\0') strcpy (dot, ".");
#endif
#ifdef MAX_EXT_CHARS
	if (MAX_EXT_CHARS < strlen (s) + strlen (dot + 1))
	  dot[MAX_EXT_CHARS + 1 - strlen (s)] = '\0';
#endif
	if (sizeof ifname <= ilen + strlen (s))
	  goto name_too_long;
        strcat(ifname, s);
        if (do_stat(ifname, sbuf) == 0) return OK;
	if (strequ (s0, z_suffix))
	  z_suffix_errno = errno;
    } while (*++suf != NULL);

    strcpy(ifname, iname);
#ifdef NO_MULTIPLE_DOTS
    if (*dot == '\0') strcpy(dot, ".");
#endif
#ifdef MAX_EXT_CHARS
    if (MAX_EXT_CHARS < z_len + strlen (dot + 1))
      dot[MAX_EXT_CHARS + 1 - z_len] = '\0';
#endif
    strcat(ifname, z_suffix);
    errno = z_suffix_errno;
    progerror(ifname);
    return ERROR;
      
 name_too_long:
    fprintf (stderr, "%s: %s: file name too long\n", progname, iname);
    exit_code = ERROR;
    return ERROR;
}

/* ========================================================================
 * Generate ofname given ifname. Return OK, or WARNING if file must be skipped.
 * Sets save_orig_name to true if the file name has been truncated.
 */
local int make_ofname()
{
    char *suff;            /* ofname z suffix */

    strcpy(ofname, ifname);
    /* strip a version number if any and get the gzip suffix if present: */
    suff = get_suffix(ofname);

    if (decompress) {
	if (suff == NULL) {
	    /* With -t or -l, try all files (even without .gz suffix)
	     * except with -r (behave as with just -dr).
             */
            if (!recursive && (list || test)) return OK;

	    /* Avoid annoying messages with -r */
	    if (verbose || (!recursive && !quiet)) {
		WARN((stderr,"%s: %s: unknown suffix -- ignored\n",
		      progname, ifname));
	    }
	    return WARNING;
	}
	/* Make a special case for .tgz and .taz: */
	strlwr(suff);
	if (strequ(suff, ".tgz") || strequ(suff, ".taz")) {
	    strcpy(suff, ".tar");
	} else {
	    *suff = '\0'; /* strip the z suffix */
	}
        /* ofname might be changed later if infile contains an original name */

    } else if (suff != NULL) {
	/* Avoid annoying messages with -r (see treat_dir()) */
	if (verbose || (!recursive && !quiet)) {
	    /* don't use WARN -- it will cause an exit_code of 2 */
	    fprintf(stderr, "%s: %s already has %s suffix -- unchanged\n",
		  progname, ifname, suff);
	}
	return WARNING;
    } else {
        save_orig_name = 0;

#ifdef NO_MULTIPLE_DOTS
	suff = strrchr(ofname, '.');
	if (suff == NULL) {
	    if (sizeof ofname <= strlen (ofname) + 1)
		goto name_too_long;
            strcat(ofname, ".");
#  ifdef MAX_EXT_CHARS
	    if (strequ(z_suffix, "z")) {
		if (sizeof ofname <= strlen (ofname) + 2)
		    goto name_too_long;
		strcat(ofname, "gz"); /* enough room */
		return OK;
	    }
        /* On the Atari and some versions of MSDOS, name_too_long()
         * does not work correctly because of a bug in stat(). So we
         * must truncate here.
         */
        } else if (strlen(suff)-1 + z_len > MAX_SUFFIX) {
            suff[MAX_SUFFIX+1-z_len] = '\0';
            save_orig_name = 1;
#  endif
        }
#endif /* NO_MULTIPLE_DOTS */
	if (sizeof ofname <= strlen (ofname) + z_len)
	    goto name_too_long;
	strcat(ofname, z_suffix);

    } /* decompress ? */
    return OK;

 name_too_long:
    WARN ((stderr, "%s: %s: file name too long\n", progname, ifname));
    return WARNING;
}

local int get_method(int in)
{
    uch flags;     /* compression flags */
    char magic[2]; /* magic header */
    int imagic1;   /* like magic[1], but can represent EOF */
    ulg stamp;     /* time stamp */

    /* If --force and --stdout, zcat == cat, so do not complain about
     * premature end of file: use try_byte instead of get_byte.
     */
    if (force && to_stdout) {
	magic[0] = (char)try_byte();
	imagic1 = try_byte ();
	magic[1] = (char) imagic1;
	/* If try_byte returned EOF, magic[1] == (char) EOF.  */
    } else {
	magic[0] = (char)get_byte();
	magic[1] = (char)get_byte();
	imagic1 = 0; /* avoid lint warning */
    }
    method = -1;                 /* unknown yet */
    part_nb++;                   /* number of parts in gzip file */
    header_bytes = 0;
    last_member = RECORD_IO;
    /* assume multiple members in gzip file except for record oriented I/O */

    if (memcmp(magic, GZIP_MAGIC, 2) == 0
        || memcmp(magic, OLD_GZIP_MAGIC, 2) == 0) {

	method = (int)get_byte();
	if (method != DEFLATED) {
	    fprintf(stderr,
		    "%s: %s: unknown method %d -- not supported\n",
		    progname, ifname, method);
	    exit_code = ERROR;
	    return -1;
	}
	work = unzip;
	flags  = (uch)get_byte();

	if ((flags & ENCRYPTED) != 0) {
	    fprintf(stderr,
		    "%s: %s is encrypted -- not supported\n",
		    progname, ifname);
	    exit_code = ERROR;
	    return -1;
	}
	if ((flags & CONTINUATION) != 0) {
	    fprintf(stderr,
		    "%s: %s is a a multi-part gzip file -- not supported\n",
		    progname, ifname);
	    exit_code = ERROR;
	    if (force <= 1) return -1;
	}
	if ((flags & RESERVED) != 0) {
	    fprintf(stderr,
		    "%s: %s has flags 0x%x -- not supported\n",
		    progname, ifname, flags);
	    exit_code = ERROR;
	    if (force <= 1) return -1;
	}
	stamp  = (ulg)get_byte();
	stamp |= ((ulg)get_byte()) << 8;
	stamp |= ((ulg)get_byte()) << 16;
	stamp |= ((ulg)get_byte()) << 24;
	if (stamp != 0 && !no_time) time_stamp = stamp;

	(void)get_byte();  /* Ignore extra flags for the moment */
	(void)get_byte();  /* Ignore OS type for the moment */

	if ((flags & CONTINUATION) != 0) {
	    unsigned part = (unsigned)get_byte();
	    part |= ((unsigned)get_byte())<<8;
	    if (verbose) {
		fprintf(stderr,"%s: %s: part number %u\n",
			progname, ifname, part);
	    }
	}
	if ((flags & EXTRA_FIELD) != 0) {
	    unsigned len = (unsigned)get_byte();
	    len |= ((unsigned)get_byte())<<8;
	    if (verbose) {
		fprintf(stderr,"%s: %s: extra field of %u bytes ignored\n",
			progname, ifname, len);
	    }
	    while (len--) (void)get_byte();
	}

	/* Get original file name if it was truncated */
	if ((flags & ORIG_NAME) != 0) {
	    if (no_name || (to_stdout && !list) || part_nb > 1) {
		/* Discard the old name */
		char c; /* dummy used for NeXTstep 3.0 cc optimizer bug */
		do {c=get_byte();} while (c != 0);
	    } else {
		/* Copy the base name. Keep a directory prefix intact. */
                char *p = base_name (ofname);
                char *base = p;
		char *base2;
		for (;;) {
		    *p = (char)get_char();
		    if (*p++ == '\0') break;
		    if (p >= ofname+sizeof(ofname)) {
			error((char *)"corrupted input -- file name too large");
		    }
		}
		base2 = base_name (base);
		strcpy(base, base2);
                /* If necessary, adapt the name to local OS conventions: */
                if (!list) {
                   MAKE_LEGAL_NAME(base);
		   if (base) list=0; /* avoid warning about unused variable */
                }
	    } /* no_name || to_stdout */
	} /* ORIG_NAME */

	/* Discard file comment if any */
	if ((flags & COMMENT) != 0) {
	    while (get_char() != 0) /* null */ ;
	}
	if (part_nb == 1) {
	    header_bytes = inptr + 2*sizeof(long); /* include crc and size */
	}

    } else if (memcmp(magic, PKZIP_MAGIC, 2) == 0 && inptr == 2
	    && memcmp((char*)inbuf, PKZIP_MAGIC, 4) == 0) {
	/* To simplify the code, we support a zip file when alone only.
         * We are thus guaranteed that the entire local header fits in inbuf.
         */
        inptr = 0;
	work = unzip;
	if (check_zipfile(in) != OK) return -1;
	/* check_zipfile may get ofname from the local header */
	last_member = 1;

    } else if (memcmp(magic, PACK_MAGIC, 2) == 0) {
	work = unpack;
	method = PACKED;

    } else if (memcmp(magic, LZW_MAGIC, 2) == 0) {
	work = unlzw;
	method = COMPRESSED;
	last_member = 1;

    } else if (memcmp(magic, LZH_MAGIC, 2) == 0) {
	work = unlzh;
	method = LZHED;
	last_member = 1;

    } else if (force && to_stdout && !list) { /* pass input unchanged */
	method = STORED;
	work = copy;
        inptr = 0;
	last_member = 1;
    }
    if (method >= 0) return method;

    if (part_nb == 1) {
	fprintf(stderr, "\n%s: %s: not in gzip format\n", progname, ifname);
	exit_code = ERROR;
	return -1;
    } else {
	if (magic[0] == 0)
	  {
	    int inbyte;
	    for (inbyte = imagic1;  inbyte == 0;  inbyte = try_byte ())
	      continue;
	    if (inbyte == EOF)
	      {
		if (verbose)
		  WARN ((stderr, "\n%s: %s: decompression OK, trailing zero bytes ignored\n",
			 progname, ifname));
		return -3;
	      }
	  }

	WARN((stderr, "\n%s: %s: decompression OK, trailing garbage ignored\n",
	      progname, ifname));
	return -2;
    }
}

local void do_list(int ifd, int method)
{
    ulg crc;
    static int first_time = 1;
    static char* methods[MAX_METHODS] = {
        (char *)"store",
        (char *)"compr",
        (char *)"pack ",
        (char *)"lzh  ",
        (char *)"", (char *)"", (char *)"", (char *)"",
        (char *)"defla"};
    char *date;
    int positive_off_t_width = 1;
    off_t o;

    for (o = OFF_T_MAX;  9 < o;  o /= 10) {
	positive_off_t_width++;
    }

    if (first_time && method >= 0) {
	first_time = 0;
	if (verbose)  {
	    printf("method  crc     date  time  ");
	}
	if (!quiet) {
	    printf("%*.*s %*.*s  ratio uncompressed_name\n",
		   positive_off_t_width, positive_off_t_width, "compressed",
		   positive_off_t_width, positive_off_t_width, "uncompressed");
	}
    } else if (method < 0) {
	if (total_in <= 0 || total_out <= 0) return;
	if (verbose) {
	    printf("                            ");
	}
	if (verbose || !quiet) {
	    fprint_off(stdout, total_in, positive_off_t_width);
	    printf(" ");
	    fprint_off(stdout, total_out, positive_off_t_width);
	    printf(" ");
	}
	display_ratio(total_out-(total_in-header_bytes), total_out, stdout);
	/* header_bytes is not meaningful but used to ensure the same
	 * ratio if there is a single file.
	 */
	printf(" (totals)\n");
	return;
    }
    crc = (ulg)~0; /* unknown */
    bytes_out = -1L;
    bytes_in = ifile_size;

#if RECORD_IO == 0
    if (method == DEFLATED && !last_member) {
        /* Get the crc and uncompressed size for gzip'ed (not zip'ed) files.
         * If the lseek fails, we could use read() to get to the end, but
         * --list is used to get quick results.
         * Use "gunzip < foo.gz | wc -c" to get the uncompressed size if
         * you are not concerned about speed.
         */
        bytes_in = lseek(ifd, (off_t)(-8), SEEK_END);
        if (bytes_in != -1L) {
            uch buf[8];
            bytes_in += 8L;
            if (read(ifd, (char*)buf, sizeof(buf)) != sizeof(buf)) {
                read_error();
            }
            crc       = LG(buf);
	    bytes_out = LG(buf+4);
	}
    }
#endif /* RECORD_IO */
    date = ctime((time_t*)&time_stamp) + 4; /* skip the day of the week */
    date[12] = '\0';               /* suppress the 1/100sec and the year */
    if (verbose) {
        printf("%5s %08lx %11s ", methods[method], crc, date);
    }
    fprint_off(stdout, bytes_in, positive_off_t_width);
    printf(" ");
    fprint_off(stdout, bytes_out, positive_off_t_width);
    printf(" ");
    if (bytes_in  == -1L) {
	total_in = -1L;
	bytes_in = bytes_out = header_bytes = 0;
    } else if (total_in >= 0) {
	total_in  += bytes_in;
    }
    if (bytes_out == -1L) {
	total_out = -1L;
	bytes_in = bytes_out = header_bytes = 0;
    } else if (total_out >= 0) {
	total_out += bytes_out;
    }
    display_ratio(bytes_out-(bytes_in-header_bytes), bytes_out, stdout);
    printf(" %s\n", ofname);
}

/* ========================================================================
 * Return true if the two stat structures correspond to the same file.
 */
local int same_file(struct stat *stat1, struct stat *stat2)
{
    return stat1->st_ino   == stat2->st_ino
	&& stat1->st_dev   == stat2->st_dev
#ifdef NO_ST_INO
        /* Can't rely on st_ino and st_dev, use other fields: */
	&& stat1->st_mode  == stat2->st_mode
	&& stat1->st_uid   == stat2->st_uid
	&& stat1->st_gid   == stat2->st_gid
	&& stat1->st_size  == stat2->st_size
	&& stat1->st_atime == stat2->st_atime
	&& stat1->st_mtime == stat2->st_mtime
	&& stat1->st_ctime == stat2->st_ctime
#endif
	    ;
}


local int name_too_long(char *name, struct stat *statb)
{
    int s = strlen(name);
    char c = name[s-1];
    struct stat	tstat; /* stat for truncated name */
    int res;

    tstat = *statb;      /* Just in case OS does not fill all fields */
    name[s-1] = '\0';
    res = lstat(name, &tstat) == 0 && same_file(statb, &tstat);
    name[s-1] = c;
    Trace((stderr, " too_long(%s) => %d\n", name, res));
    return res;
}

local void shorten_name(char *name)
{
    int len;                 
    char *trunc = NULL;      
    int plen;                
    int min_part = MIN_PART; 
    char *p;

    len = strlen(name);
    if (decompress) {
	if (len <= 1) error((char *)"name too short");
	name[len-1] = '\0';
	return;
    }
    p = get_suffix(name);
    if (p == NULL) error((char *)"can't recover suffix\n");
    *p = '\0';
    save_orig_name = 1;

    if (len > 4 && strequ(p-4, ".tar")) {
	strcpy(p-4, ".tgz");
	return;
    }
    do {
	p = strrchr(name, PATH_SEP);
	p = p ? p+1 : name;
	while (*p) {
	    plen = strcspn(p, PART_SEP);
	    p += plen;
	    if (plen > min_part) trunc = p-1;
	    if (*p) p++;
	}
    } while (trunc == NULL && --min_part != 0);

    if (trunc != NULL) {
	do {
	    trunc[0] = trunc[1];
	} while (*trunc++);
	trunc--;
    } else {
	trunc = strrchr(name, PART_SEP[0]);
	if (trunc == NULL) error((char *)"internal error in shorten_name");
	if (trunc[1] == '\0') trunc--;
    }
    strcpy(trunc, z_suffix);
}

local int check_ofname()
{
    struct stat	ostat;

#ifdef ENAMETOOLONG

    errno = 0;
    while (lstat(ofname, &ostat) != 0) {
        if (errno != ENAMETOOLONG) return 0;
	shorten_name(ofname);
    }
#else
    if (lstat(ofname, &ostat) != 0) return 0;
#endif
    if (!decompress && name_too_long(ofname, &ostat)) {
	shorten_name(ofname);
	if (lstat(ofname, &ostat) != 0) return 0;
    }

    if (same_file(&istat, &ostat)) {
	if (strequ(ifname, ofname)) {
	    fprintf(stderr, "%s: %s: cannot %scompress onto itself\n",
		    progname, ifname, decompress ? "de" : "");
	} else {
	    fprintf(stderr, "%s: %s and %s are the same file\n",
		    progname, ifname, ofname);
	}
	exit_code = ERROR;
	return ERROR;
    }
    if (!force) {
	int ok = 0;
	fprintf(stderr, "%s: %s already exists;", progname, ofname);
	if (foreground && isatty(fileno(stdin))) {
	    fprintf(stderr, " do you wish to overwrite (y or n)? ");
	    fflush(stderr);
	    ok = yesno();
	}
	if (!ok) {
	    fprintf(stderr, "\tnot overwritten\n");
	    if (exit_code == OK) exit_code = WARNING;
	    return ERROR;
	}
    }
    if (xunlink (ofname)) {
	progerror(ofname);
	return ERROR;
    }
    return OK;
}


#ifndef NO_UTIME
/* ========================================================================
 * Set the access and modification times from the given stat buffer.
 */
local void reset_times (char *name, struct stat *statb)
{
    struct utimbuf	timep;

    /* Copy the time stamp */
    timep.actime  = statb->st_atime;
    timep.modtime = statb->st_mtime;

    /* Some systems (at least OS/2) do not support utime on directories */
    if (utime(name, &timep) && !S_ISDIR(statb->st_mode)) {
	int e = errno;
	WARN((stderr, "%s: ", progname));
	if (!quiet) {
	    errno = e;
	    perror(ofname);
	}
    }
}
#endif


/* ========================================================================
 * Copy modes, times, ownership from input file to output file.
 * IN assertion: to_stdout is false.
 */
local void copy_stat(struct stat *ifstat)
{
#ifndef NO_UTIME
    if (decompress && time_stamp != 0 && ifstat->st_mtime != time_stamp) {
	ifstat->st_mtime = time_stamp;
	if (verbose > 1) {
	    fprintf(stderr, "%s: time stamp restored\n", ofname);
	}
    }
    reset_times(ofname, ifstat);
#endif
    /* Copy the protection modes */
    if (fchmod(ofd, ifstat->st_mode & 07777)) {
	int e = errno;
	WARN((stderr, "%s: ", progname));
	if (!quiet) {
	    errno = e;
	    perror(ofname);
	}
    }
#ifndef NO_CHOWN
    fchown(ofd, ifstat->st_uid, ifstat->st_gid);  /* Copy ownership */
#endif
    remove_ofname = 0;
    /* It's now safe to remove the input file: */
    if (xunlink (ifname)) {
	int e = errno;
	WARN((stderr, "%s: ", progname));
	if (!quiet) {
	    errno = e;
	    perror(ifname);
	}
    }
}

#if ! NO_DIR

/* ========================================================================
 * Recurse through the given directory. This code is taken from ncompress.
 */
local void treat_dir(char *dir)
{
    struct dirent *dp;
    DIR      *dirp;
    char     nbuf[MAX_PATH_LEN];
    int      len;

    dirp = opendir(dir);
    
    if (dirp == NULL) {
	progerror(dir);
	return ;
    }
        
    while ((errno = 0, dp = readdir(dirp)) != NULL) {

	if (strequ(dp->d_name,".") || strequ(dp->d_name,"..")) {
	    continue;
	}
	len = strlen(dir);
	if (len + NAMLEN(dp) + 1 < MAX_PATH_LEN - 1) {
	    strcpy(nbuf,dir);
	    if (len != 0
#ifdef PATH_SEP2
		&& dir[len-1] != PATH_SEP2
#endif
#ifdef PATH_SEP3
		&& dir[len-1] != PATH_SEP3
#endif
	    ) {
		nbuf[len++] = PATH_SEP;
	    }
	    strcpy(nbuf+len, dp->d_name);
	    treat_file(nbuf);
	} else {
	    fprintf(stderr,"%s: %s/%s: pathname too long\n",
		    progname, dir, dp->d_name);
	    exit_code = ERROR;
	}
    }
    if (errno != 0)
	progerror(dir);
    if (CLOSEDIR(dirp) != 0)
	progerror(dir);
}
#endif /* ! NO_DIR */

local void do_exit(int exitcode)
{
    static int in_exit = 0;

    if (in_exit) exit(exitcode);
    in_exit = 1;
    if (env != NULL)  free(env),  env  = NULL;
    if (args != NULL) free((char*)args), args = NULL;
    FREE(inbuf);
    FREE(outbuf);
    FREE(d_buf);
    FREE(window);
#ifndef MAXSEG_64K
    FREE(tab_prefix);
#else
    FREE(tab_prefix0);
    FREE(tab_prefix1);
#endif
    exit(exitcode);
}

/* ========================================================================
 * Close and unlink the output file if appropriate.  This routine must be
 * async-signal-safe.
 */
local void do_remove() {
   if (remove_ofname) {
       close(ofd);
       xunlink (ofname);
   }
}

/* ========================================================================
 * Error handler.
 */
RETSIGTYPE abort_gzip()
{
	do_remove();
	do_exit(ERROR);
}

RETSIGTYPE abort_gzip_signal()
{
	do_remove();
	_exit(ERROR);
}

#ifdef RCSID
static char rcsid[] = "$Id: inflate.c,v 0.14 1993/06/10 13:27:04 jloup Exp $";
#endif

#define slide window

struct huft {
    uch e;
    uch b;
  union {
    ush n;          
    struct huft *t; 
  } v;
};

int huft_build OF((unsigned *, unsigned, unsigned, ush *, ush *,
                   struct huft **, int *));
int huft_free OF((struct huft *));
int inflate_codes OF((struct huft *, struct huft *, int, int));
int inflate_stored OF((void));
int inflate_fixed OF((void));
int inflate_dynamic OF((void));
int inflate_block OF((int *));
int inflate OF((void));

#define wp outcnt
#define flush_output(w) (wp=(w),flush_window())

static unsigned border[] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static ush cplens[] = {         /* Copy lengths for literal codes 257..285 */
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
        /* note: see note #13 above about the 258 in this list. */
static ush cplext[] = {         /* Extra bits for literal codes 257..285 */
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99}; /* 99==invalid */
static ush cpdist[] = {         /* Copy offsets for distance codes 0..29 */
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577};
static ush cpdext[] = {         /* Extra bits for distance codes */
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
        12, 12, 13, 13};



/* Macros for inflate() bit peeking and grabbing.
   The usage is:
   
        NEEDBITS(j)
        x = b & mask_bits[j];
        DUMPBITS(j)

   where NEEDBITS makes sure that b has at least j bits in it, and
   DUMPBITS removes the bits from b.  The macros use the variable k
   for the number of bits in b.  Normally, b and k are register
   variables for speed, and are initialized at the beginning of a
   routine that uses these macros from a global bit buffer and count.
   The macros also use the variable w, which is a cached copy of wp.

   If we assume that EOB will be the longest code, then we will never
   ask for bits with NEEDBITS that are beyond the end of the stream.
   So, NEEDBITS should not read any more bytes than are needed to
   meet the request.  Then no bytes need to be "returned" to the buffer
   at the end of the last block.

   However, this assumption is not true for fixed blocks--the EOB code
   is 7 bits, but the other literal/length codes can be 8 or 9 bits.
   (The EOB code is shorter than other codes because fixed blocks are
   generally short.  So, while a block always has an EOB, many other
   literal/length codes have a significantly lower probability of
   showing up at all.)  However, by making the first table have a
   lookup of seven bits, the EOB code will be found in that first
   lookup, and so will not require that too many bits be pulled from
   the stream.
 */

ulg bb;                         /* bit buffer */
unsigned bk;                    /* bits in bit buffer */

ush mask_bits[] = {
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
    0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};

#define GETBYTE() (inptr < insize ? inbuf[inptr++] : (wp = w, fill_inbuf(0)))

#ifdef CRYPT
  uch cc;
#  define NEXTBYTE() \
     (decrypt ? (cc = GETBYTE(), zdecode(cc), cc) : GETBYTE())
#else
#  define NEXTBYTE()  (uch)GETBYTE()
#endif
#define NEEDBITS(n) {while(k<(n)){b|=((ulg)NEXTBYTE())<<k;k+=8;}}
#define DUMPBITS(n) {b>>=(n);k-=(n);}


/*
   Huffman code decoding is performed using a multi-level table lookup.
   The fastest way to decode is to simply build a lookup table whose
   size is determined by the longest code.  However, the time it takes
   to build this table can also be a factor if the data being decoded
   is not very long.  The most common codes are necessarily the
   shortest codes, so those codes dominate the decoding time, and hence
   the speed.  The idea is you can have a shorter table that decodes the
   shorter, more probable codes, and then point to subsidiary tables for
   the longer codes.  The time it costs to decode the longer codes is
   then traded against the time it takes to make longer tables.

   This results of this trade are in the variables lbits and dbits
   below.  lbits is the number of bits the first level table for literal/
   length codes can decode in one step, and dbits is the same thing for
   the distance codes.  Subsequent tables are also less than or equal to
   those sizes.  These values may be adjusted either when all of the
   codes are shorter than that, in which case the longest code length in
   bits is used, or when the shortest code is *longer* than the requested
   table size, in which case the length of the shortest code in bits is
   used.

   There are two different values for the two tables, since they code a
   different number of possibilities each.  The literal/length table
   codes 286 possible values, or in a flat code, a little over eight
   bits.  The distance table codes 30 possible values, or a little less
   than five bits, flat.  The optimum values for speed end up being
   about one bit more than those, so lbits is 8+1 and dbits is 5+1.
   The optimum values may differ though from machine to machine, and
   possibly even between compilers.  Your mileage may vary.
 */


int lbits = 9;          /* bits in base literal/length lookup table */
int dbits = 6;          /* bits in base distance lookup table */


/* If BMAX needs to be larger than 16, then h and x[] should be ulg. */
#define BMAX 16         /* maximum bit length of any code (16 for explode) */
#define N_MAX 288       /* maximum number of codes in any set */


unsigned hufts;         /* track memory usage */


int huft_build(unsigned *b, unsigned n, unsigned s, ush *d, ush *e, struct huft **t, int *m)
{
  unsigned a;                   /* counter for codes of length k */
  unsigned c[BMAX+1];           /* bit length count table */
  unsigned f;                   /* i repeats in table every f entries */
  int g;                        /* maximum code length */
  int h;                        /* table level */
  register unsigned i;          /* counter, current code */
  register unsigned j;          /* counter */
  register int k;               /* number of bits in current code */
  int l;                        /* bits per table (returned in m) */
  register unsigned *p;         /* pointer into c[], b[], or v[] */
  register struct huft *q;      /* points to current table */
  struct huft r;                /* table entry for structure assignment */
  struct huft *u[BMAX];         /* table stack */
  unsigned v[N_MAX];            /* values in order of bit length */
  register int w;               /* bits before this table == (l * h) */
  unsigned x[BMAX+1];           /* bit offsets, then code stack */
  unsigned *xp;                 /* pointer into x */
  int y;                        /* number of dummy codes added */
  unsigned z;                   /* number of entries in current table */


  /* Generate counts for each bit length */
  memzero(c, sizeof(c));
  p = b;  i = n;
  do {
    Tracecv(*p, (stderr, (n-i >= ' ' && n-i <= '~' ? "%c %d\n" : "0x%x %d\n"), 
	    n-i, *p));
    c[*p]++;                    /* assume all entries <= BMAX */
    p++;                      /* Can't combine with above line (Solaris bug) */
  } while (--i);
  if (c[0] == n)                /* null input--all zero length codes */
  {
    *t = (struct huft *)NULL;
    *m = 0;
    return 0;
  }


  /* Find minimum and maximum length, bound *m by those */
  l = *m;
  for (j = 1; j <= BMAX; j++)
    if (c[j])
      break;
  k = j;                        /* minimum code length */
  if ((unsigned)l < j)
    l = j;
  for (i = BMAX; i; i--)
    if (c[i])
      break;
  g = i;                        /* maximum code length */
  if ((unsigned)l > i)
    l = i;
  *m = l;


  /* Adjust last length count to fill out codes, if needed */
  for (y = 1 << j; j < i; j++, y <<= 1)
    if ((y -= c[j]) < 0)
      return 2;                 /* bad input: more codes than bits */
  if ((y -= c[i]) < 0)
    return 2;
  c[i] += y;


  /* Generate starting offsets into the value table for each length */
  x[1] = j = 0;
  p = c + 1;  xp = x + 2;
  while (--i) {                 /* note that i == g from above */
    *xp++ = (j += *p++);
  }


  /* Make a table of values in order of bit lengths */
  p = b;  i = 0;
  do {
    if ((j = *p++) != 0)
      v[x[j]++] = i;
  } while (++i < n);
  n = x[g];                   /* set n to length of v */


  /* Generate the Huffman codes and for each, make the table entries */
  x[0] = i = 0;                 /* first Huffman code is zero */
  p = v;                        /* grab values in bit order */
  h = -1;                       /* no tables yet--level -1 */
  w = -l;                       /* bits decoded == (l * h) */
  u[0] = (struct huft *)NULL;   /* just to keep compilers happy */
  q = (struct huft *)NULL;      /* ditto */
  z = 0;                        /* ditto */

  /* go through the bit lengths (k already is bits in shortest code) */
  for (; k <= g; k++)
  {
    a = c[k];
    while (a--)
    {
      /* here i is the Huffman code of length k bits for value *p */
      /* make tables up to required level */
      while (k > w + l)
      {
        h++;
        w += l;                 /* previous table always l bits */

        /* compute minimum size table less than or equal to l bits */
        z = (z = g - w) > (unsigned)l ? l : z;  /* upper limit on table size */
        if ((f = 1 << (j = k - w)) > a + 1)     /* try a k-w bit table */
        {                       /* too few codes for k-w bit table */
          f -= a + 1;           /* deduct codes from patterns left */
          xp = c + k;
	  if (j < z)
	    while (++j < z)       /* try smaller tables up to z bits */
	    {
	      if ((f <<= 1) <= *++xp)
		break;            /* enough codes to use up j bits */
	      f -= *xp;           /* else deduct codes from patterns */
	    }
        }
        z = 1 << j;             /* table entries for j-bit table */

        /* allocate and link in new table */
        if ((q = (struct huft *)malloc((z + 1)*sizeof(struct huft))) ==
            (struct huft *)NULL)
        {
          if (h)
            huft_free(u[0]);
          return 3;             /* not enough memory */
        }
        hufts += z + 1;         /* track memory usage */
        *t = q + 1;             /* link to list for huft_free() */
        *(t = &(q->v.t)) = (struct huft *)NULL;
        u[h] = ++q;             /* table starts after link */

        /* connect to last table, if there is one */
        if (h)
        {
          x[h] = i;             /* save pattern for backing up */
          r.b = (uch)l;         /* bits to dump before this table */
          r.e = (uch)(16 + j);  /* bits in this table */
          r.v.t = q;            /* pointer to this table */
          j = i >> (w - l);     /* (get around Turbo C bug) */
          u[h-1][j] = r;        /* connect to last table */
        }
      }

      /* set up table entry in r */
      r.b = (uch)(k - w);
      if (p >= v + n)
        r.e = 99;               /* out of values--invalid code */
      else if (*p < s)
      {
        r.e = (uch)(*p < 256 ? 16 : 15);    /* 256 is end-of-block code */
        r.v.n = (ush)(*p);             /* simple code is just the value */
	p++;                           /* one compiler does not like *p++ */
      }
      else
      {
        r.e = (uch)e[*p - s];   /* non-simple--look up in lists */
        r.v.n = d[*p++ - s];
      }

      /* fill code-like entries with r */
      f = 1 << (k - w);
      for (j = i >> w; j < z; j += f)
        q[j] = r;

      /* backwards increment the k-bit code i */
      for (j = 1 << (k - 1); i & j; j >>= 1)
        i ^= j;
      i ^= j;

      /* backup over finished tables */
      while ((i & ((1 << w) - 1)) != x[h])
      {
        h--;                    /* don't need to update q */
        w -= l;
      }
    }
  }


  /* Return true (1) if we were given an incomplete table */
  return y != 0 && g != 1;
}



int huft_free(struct huft *t)
/* Free the malloc'ed tables built by huft_build(), which makes a linked
   list of the tables it made, with the links in a dummy first entry of
   each table. */
{
  register struct huft *p, *q;


  /* Go through linked list, freeing from the malloced (t[-1]) address. */
  p = t;
  while (p != (struct huft *)NULL)
  {
    q = (--p)->v.t;
    free((char*)p);
    p = q;
  } 
  return 0;
}


int inflate_codes(struct huft *tl, struct huft *td, int bl, int bd)
{
  register unsigned e;  /* table entry flag/number of extra bits */
  unsigned n, d;        /* length and index for copy */
  unsigned w;           /* current window position */
  struct huft *t;       /* pointer to table entry */
  unsigned ml, md;      /* masks for bl and bd bits */
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */


  /* make local copies of globals */
  b = bb;                       /* initialize bit buffer */
  k = bk;
  w = wp;                       /* initialize window position */

  /* inflate the coded data */
  ml = mask_bits[bl];           /* precompute masks for speed */
  md = mask_bits[bd];
  for (;;)                      /* do until end of block */
  {
    NEEDBITS((unsigned)bl)
    if ((e = (t = tl + ((unsigned)b & ml))->e) > 16)
      do {
        if (e == 99)
          return 1;
        DUMPBITS(t->b)
        e -= 16;
        NEEDBITS(e)
      } while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
    DUMPBITS(t->b)
    if (e == 16)                /* then it's a literal */
    {
      slide[w++] = (uch)t->v.n;
      Tracevv((stderr, "%c", slide[w-1]));
      if (w == WSIZE)
      {
        flush_output(w);
        w = 0;
      }
    }
    else                        /* it's an EOB or a length */
    {
      /* exit if end of block */
      if (e == 15)
        break;

      /* get length of block to copy */
      NEEDBITS(e)
      n = t->v.n + ((unsigned)b & mask_bits[e]);
      DUMPBITS(e);

      /* decode distance of block to copy */
      NEEDBITS((unsigned)bd)
      if ((e = (t = td + ((unsigned)b & md))->e) > 16)
        do {
          if (e == 99)
            return 1;
          DUMPBITS(t->b)
          e -= 16;
          NEEDBITS(e)
        } while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
      DUMPBITS(t->b)
      NEEDBITS(e)
      d = w - t->v.n - ((unsigned)b & mask_bits[e]);
      DUMPBITS(e)
      Tracevv((stderr,"\\[%d,%d]", w-d, n));

      /* do the copy */
      do {
        n -= (e = (e = WSIZE - ((d &= WSIZE-1) > w ? d : w)) > n ? n : e);
#if !defined(NOMEMCPY) && !defined(DEBUG)
        if (w - d >= e)         /* (this test assumes unsigned comparison) */
        {
          memcpy(slide + w, slide + d, e);
          w += e;
          d += e;
        }
        else                      /* do it slow to avoid memcpy() overlap */
#endif /* !NOMEMCPY */
          do {
            slide[w++] = slide[d++];
	    Tracevv((stderr, "%c", slide[w-1]));
          } while (--e);
        if (w == WSIZE)
        {
          flush_output(w);
          w = 0;
        }
      } while (n);
    }
  }


  /* restore the globals from the locals */
  wp = w;                       /* restore global window pointer */
  bb = b;                       /* restore global bit buffer */
  bk = k;

  /* done */
  return 0;
}



int inflate_stored()
/* "decompress" an inflated type 0 (stored) block. */
{
  unsigned n;           /* number of bytes in block */
  unsigned w;           /* current window position */
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */


  /* make local copies of globals */
  b = bb;                       /* initialize bit buffer */
  k = bk;
  w = wp;                       /* initialize window position */


  /* go to byte boundary */
  n = k & 7;
  DUMPBITS(n);


  /* get the length and its complement */
  NEEDBITS(16)
  n = ((unsigned)b & 0xffff);
  DUMPBITS(16)
  NEEDBITS(16)
  if (n != (unsigned)((~b) & 0xffff))
    return 1;                   /* error in compressed data */
  DUMPBITS(16)


  /* read and output the compressed data */
  while (n--)
  {
    NEEDBITS(8)
    slide[w++] = (uch)b;
    if (w == WSIZE)
    {
      flush_output(w);
      w = 0;
    }
    DUMPBITS(8)
  }


  /* restore the globals from the locals */
  wp = w;                       /* restore global window pointer */
  bb = b;                       /* restore global bit buffer */
  bk = k;
  return 0;
}



int inflate_fixed()
/* decompress an inflated type 1 (fixed Huffman codes) block.  We should
   either replace this with a custom decoder, or at least precompute the
   Huffman tables. */
{
  int i;                /* temporary variable */
  struct huft *tl;      /* literal/length code table */
  struct huft *td;      /* distance code table */
  int bl;               /* lookup bits for tl */
  int bd;               /* lookup bits for td */
  unsigned l[288];      /* length list for huft_build */


  /* set up literal table */
  for (i = 0; i < 144; i++)
    l[i] = 8;
  for (; i < 256; i++)
    l[i] = 9;
  for (; i < 280; i++)
    l[i] = 7;
  for (; i < 288; i++)          /* make a complete, but wrong code set */
    l[i] = 8;
  bl = 7;
  if ((i = huft_build(l, 288, 257, cplens, cplext, &tl, &bl)) != 0)
    return i;


  /* set up distance table */
  for (i = 0; i < 30; i++)      /* make an incomplete code set */
    l[i] = 5;
  bd = 5;
  if ((i = huft_build(l, 30, 0, cpdist, cpdext, &td, &bd)) > 1)
  {
    huft_free(tl);
    return i;
  }


  /* decompress until an end-of-block code */
  if (inflate_codes(tl, td, bl, bd))
    return 1;


  /* free the decoding tables, return */
  huft_free(tl);
  huft_free(td);
  return 0;
}



int inflate_dynamic()
/* decompress an inflated type 2 (dynamic Huffman codes) block. */
{
  int i;                /* temporary variables */
  unsigned j;
  unsigned l;           /* last length */
  unsigned m;           /* mask for bit lengths table */
  unsigned n;           /* number of lengths to get */
  unsigned w;           /* current window position */
  struct huft *tl;      /* literal/length code table */
  struct huft *td;      /* distance code table */
  int bl;               /* lookup bits for tl */
  int bd;               /* lookup bits for td */
  unsigned nb;          /* number of bit length codes */
  unsigned nl;          /* number of literal/length codes */
  unsigned nd;          /* number of distance codes */
#ifdef PKZIP_BUG_WORKAROUND
  unsigned ll[288+32];  /* literal/length and distance code lengths */
#else
  unsigned ll[286+30];  /* literal/length and distance code lengths */
#endif
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */


  /* make local bit buffer */
  b = bb;
  k = bk;
  w = wp;


  /* read in table lengths */
  NEEDBITS(5)
  nl = 257 + ((unsigned)b & 0x1f);      /* number of literal/length codes */
  DUMPBITS(5)
  NEEDBITS(5)
  nd = 1 + ((unsigned)b & 0x1f);        /* number of distance codes */
  DUMPBITS(5)
  NEEDBITS(4)
  nb = 4 + ((unsigned)b & 0xf);         /* number of bit length codes */
  DUMPBITS(4)
#ifdef PKZIP_BUG_WORKAROUND
  if (nl > 288 || nd > 32)
#else
  if (nl > 286 || nd > 30)
#endif
    return 1;                   /* bad lengths */


  /* read in bit-length-code lengths */
  for (j = 0; j < nb; j++)
  {
    NEEDBITS(3)
    ll[border[j]] = (unsigned)b & 7;
    DUMPBITS(3)
  }
  for (; j < 19; j++)
    ll[border[j]] = 0;


  /* build decoding table for trees--single level, 7 bit lookup */
  bl = 7;
  if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0)
  {
    if (i == 1)
      huft_free(tl);
    return i;                   /* incomplete code set */
  }

  if (tl == NULL)		/* Grrrhhh */
	return 2;

  /* read in literal and distance code lengths */
  n = nl + nd;
  m = mask_bits[bl];
  i = l = 0;
  while ((unsigned)i < n)
  {
    NEEDBITS((unsigned)bl)
    j = (td = tl + ((unsigned)b & m))->b;
    DUMPBITS(j)
    j = td->v.n;
    if (j < 16)                 /* length of code in bits (0..15) */
      ll[i++] = l = j;          /* save last length in l */
    else if (j == 16)           /* repeat last length 3 to 6 times */
    {
      NEEDBITS(2)
      j = 3 + ((unsigned)b & 3);
      DUMPBITS(2)
      if ((unsigned)i + j > n)
        return 1;
      while (j--)
        ll[i++] = l;
    }
    else if (j == 17) 
    {
      NEEDBITS(3)
      j = 3 + ((unsigned)b & 7);
      DUMPBITS(3)
      if ((unsigned)i + j > n)
        return 1;
      while (j--)
        ll[i++] = 0;
      l = 0;
    }
    else   
    {
      NEEDBITS(7)
      j = 11 + ((unsigned)b & 0x7f);
      DUMPBITS(7)
      if ((unsigned)i + j > n)
        return 1;
      while (j--)
        ll[i++] = 0;
      l = 0;
    }
  }

  huft_free(tl);

  bb = b;
  bk = k;

  bl = lbits;
  if ((i = huft_build(ll, nl, 257, cplens, cplext, &tl, &bl)) != 0)
  {
    if (i == 1) {
      fprintf(stderr, " incomplete literal tree\n");
      huft_free(tl);
    }
    return i;                   /* incomplete code set */
  }
  bd = dbits;
  if ((i = huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0)
  {
    if (i == 1) {
      fprintf(stderr, " incomplete distance tree\n");
#ifdef PKZIP_BUG_WORKAROUND
      i = 0;
    }
#else
      huft_free(td);
    }
    huft_free(tl);
    return i;                   /* incomplete code set */
#endif
  }


  /* decompress until an end-of-block code */
  if (inflate_codes(tl, td, bl, bd))
    return 1;


  /* free the decoding tables, return */
  huft_free(tl);
  huft_free(td);
  return 0;
}



int inflate_block(int *e)
{
  unsigned t;           /* block type */
  unsigned w;           /* current window position */
  register ulg b;       /* bit buffer */
  register unsigned k;  /* number of bits in bit buffer */


  /* make local bit buffer */
  b = bb;
  k = bk;
  w = wp;


  /* read in last block bit */
  NEEDBITS(1)
  *e = (int)b & 1;
  DUMPBITS(1)


  /* read in block type */
  NEEDBITS(2)
  t = (unsigned)b & 3;
  DUMPBITS(2)


  /* restore the global bit buffer */
  bb = b;
  bk = k;


  /* inflate that block type */
  if (t == 2)
    return inflate_dynamic();
  if (t == 0)
    return inflate_stored();
  if (t == 1)
    return inflate_fixed();


  /* bad block type */
  return 2;
}



int inflate()
/* decompress an inflated entry */
{
  int e;                /* last block flag */
  int r;                /* result code */
  unsigned h;           /* maximum struct huft's malloc'ed */


  /* initialize window, bit buffer */
  wp = 0;
  bk = 0;
  bb = 0;


  /* decompress until the last block */
  h = 0;
  do {
    hufts = 0;
    if ((r = inflate_block(&e)) != 0)
      return r;
    if (hufts > h)
      h = hufts;
  } while (!e);

  /* Undo too much lookahead. The next read will be byte aligned so we
   * can discard unused bits in the last meaningful byte.
   */
  while (bk >= 8) {
    bk -= 8;
    inptr--;
  }

  /* flush out slide */
  flush_output(wp);


  /* return success */
#ifdef DEBUG
  fprintf(stderr, "<%u> ", h);
#endif /* DEBUG */
  return 0;
}
/* lzw.c -- compress files in LZW format.
 * This is a dummy version avoiding patent problems.
 */

#ifdef RCSID
static char rcsid[] = "$Id: lzw.c,v 0.9 1993/06/10 13:27:31 jloup Exp $";
#endif

static int msg_done = 0;

/* Compress in to out with lzw method. */
int lzw(int in, int out)
{
    if (msg_done) return ERROR;
    msg_done = 1;
    fprintf(stderr,"output in compress .Z format not supported\n");
    if (in != out) { /* avoid warnings on unused variables */
        exit_code = ERROR;
    }
    return ERROR;
}
/* trees.c -- output deflated data using Huffman coding
 * Copyright (C) 1992-1993 Jean-loup Gailly
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License, see the file COPYING.
 */

/*
 *  PURPOSE
 *
 *      Encode various sets of source values using variable-length
 *      binary code trees.
 *
 *  DISCUSSION
 *
 *      The PKZIP "deflation" process uses several Huffman trees. The more
 *      common source values are represented by shorter bit sequences.
 *
 *      Each code tree is stored in the ZIP file in a compressed form
 *      which is itself a Huffman encoding of the lengths of
 *      all the code strings (in ascending order by source values).
 *      The actual code strings are reconstructed from the lengths in
 *      the UNZIP process, as described in the "application note"
 *      (APPNOTE.TXT) distributed as part of PKWARE's PKZIP program.
 *
 *  REFERENCES
 *
 *      Lynch, Thomas J.
 *          Data Compression:  Techniques and Applications, pp. 53-55.
 *          Lifetime Learning Publications, 1985.  ISBN 0-534-03418-7.
 *
 *      Storer, James A.
 *          Data Compression:  Methods and Theory, pp. 49-50.
 *          Computer Science Press, 1988.  ISBN 0-7167-8156-5.
 *
 *      Sedgewick, R.
 *          Algorithms, p290.
 *          Addison-Wesley, 1983. ISBN 0-201-06672-6.
 *
 *  INTERFACE
 *
 *      void ct_init (ush *attr, int *methodp)
 *          Allocate the match buffer, initialize the various tables and save
 *          the location of the internal file attribute (ascii/binary) and
 *          method (DEFLATE/STORE)
 *
 *      void ct_tally (int dist, int lc);
 *          Save the match info and tally the frequency counts.
 *
 *      off_t flush_block (char *buf, ulg stored_len, int pad, int eof)
 *          Determine the best encoding for the current block: dynamic trees,
 *          static trees or store, and output the encoded block to the zip
 *          file. If pad is set, pads the block to the next
 *          byte. Returns the total compressed length for the file so
 *          far.
 * */

#ifdef RCSID
static char rcsid[] = "$Id: trees.c,v 0.12 1993/06/10 13:27:54 jloup Exp $";
#endif

/* ===========================================================================
 * Constants
 */

#define MAX_BITS 15
/* All codes must not exceed MAX_BITS bits */

#define MAX_BL_BITS 7
/* Bit length codes must not exceed MAX_BL_BITS bits */

#define LENGTH_CODES 29
/* number of length codes, not counting the special END_BLOCK code */

#define LITERALS  256
/* number of literal bytes 0..255 */

#define END_BLOCK 256
/* end of block literal code */

#define L_CODES (LITERALS+1+LENGTH_CODES)
/* number of Literal or Length codes, including the END_BLOCK code */

#define D_CODES   30
/* number of distance codes */

#define BL_CODES  19
/* number of codes used to transfer the bit lengths */


local int near extra_lbits[LENGTH_CODES] /* extra bits for each length code */
   = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

local int near extra_dbits[D_CODES] /* extra bits for each distance code */
   = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

local int near extra_blbits[BL_CODES]/* extra bits for each bit length code */
   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};

#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2
/* The three kinds of block type */

#ifndef LIT_BUFSIZE
#  ifdef SMALL_MEM
#    define LIT_BUFSIZE  0x2000
#  else
#  ifdef MEDIUM_MEM
#    define LIT_BUFSIZE  0x4000
#  else
#    define LIT_BUFSIZE  0x8000
#  endif
#  endif
#endif
#ifndef DIST_BUFSIZE
#  define DIST_BUFSIZE  LIT_BUFSIZE
#endif

#if LIT_BUFSIZE > INBUFSIZ
    error cannot overlay l_buf and inbuf
#endif

#define REP_3_6      16
#define REPZ_3_10    17
#define REPZ_11_138  18

typedef struct ct_data {
    union {
        ush  freq;       /* frequency count */
        ush  code;       /* bit string */
    } fc;
    union {
        ush  dad;        /* father node in Huffman tree */
        ush  len;        /* length of bit string */
    } dl;
} ct_data;

#define Freq fc.freq
#define Code fc.code
#define Dad  dl.dad
#define Len  dl.len

#define HEAP_SIZE (2*L_CODES+1)
local ct_data near dyn_ltree[HEAP_SIZE];   /* literal and length tree */
local ct_data near dyn_dtree[2*D_CODES+1]; /* distance tree */

local ct_data near static_ltree[L_CODES+2];

local ct_data near static_dtree[D_CODES];

local ct_data near bl_tree[2*BL_CODES+1];
typedef struct tree_desc {
    ct_data near *dyn_tree;      /* the dynamic tree */
    ct_data near *static_tree;   /* corresponding static tree or NULL */
    int     near *extra_bits;    /* extra bits for each code or NULL */
    int     extra_base;          /* base index for extra_bits */
    int     elems;               /* max number of elements in the tree */
    int     max_length;          /* max bit length for the codes */
    int     max_code;            /* largest code with non zero frequency */
} tree_desc;

local tree_desc near l_desc =
{dyn_ltree, static_ltree, extra_lbits, LITERALS+1, L_CODES, MAX_BITS, 0};

local tree_desc near d_desc =
{dyn_dtree, static_dtree, extra_dbits, 0,          D_CODES, MAX_BITS, 0};

local tree_desc near bl_desc =
{bl_tree, (ct_data near *)0, extra_blbits, 0,      BL_CODES, MAX_BL_BITS, 0};


local ush near bl_count[MAX_BITS+1];
local uch near bl_order[BL_CODES]
   = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};

local int near heap[2*L_CODES+1]; /* heap used to build the Huffman trees */
local int heap_len;               /* number of elements in the heap */
local int heap_max;               /* element of largest frequency */

local uch near depth[2*L_CODES+1];

local uch length_code[MAX_MATCH-MIN_MATCH+1];
local uch dist_code[512];

local int near base_length[LENGTH_CODES];
local int near base_dist[D_CODES];
#define l_buf inbuf
local uch near flag_buf[(LIT_BUFSIZE/8)];
local unsigned last_lit;    /* running index in l_buf */
local unsigned last_dist;   /* running index in d_buf */
local unsigned last_flags;  /* running index in flag_buf */
local uch flags;            /* current flags not yet saved in flag_buf */
local uch flag_bit;         /* current bit used in flags */

local ulg opt_len;        /* bit length of current block with optimal trees */
local ulg static_len;     /* bit length of current block with static trees */

local off_t compressed_len; /* total bit length of compressed file */

local off_t input_len;      /* total byte length of input file */
ush *file_type;        /* pointer to UNKNOWN, BINARY or ASCII */
int *file_method;      /* pointer to DEFLATE or STORE */

#ifdef DEBUG
extern off_t bits_sent;  /* bit length of the compressed data */
#endif

extern long block_start;       /* window offset of current block */
extern unsigned near strstart; /* window offset of current string */


local void init_block     OF((void));
local void pqdownheap     OF((ct_data near *tree, int k));
local void gen_bitlen     OF((tree_desc near *desc));
local void gen_codes      OF((ct_data near *tree, int max_code));
local void build_tree_1     OF((tree_desc near *desc));
local void scan_tree      OF((ct_data near *tree, int max_code));
local void send_tree      OF((ct_data near *tree, int max_code));
local int  build_bl_tree  OF((void));
local void send_all_trees OF((int lcodes, int dcodes, int blcodes));
local void compress_block OF((ct_data near *ltree, ct_data near *dtree));
local void set_file_type  OF((void));


#ifndef DEBUG
#  define send_code(c, tree) send_bits(tree[c].Code, tree[c].Len)
#else
#  define send_code(c, tree) \
     { if (verbose>1) fprintf(stderr,"\ncd %3d ",(c)); \
       send_bits(tree[c].Code, tree[c].Len); }
#endif

#define d_code(dist) \
   ((dist) < 256 ? dist_code[dist] : dist_code[256+((dist)>>7)])


#define MAX(a,b) (a >= b ? a : b)

void ct_init(ush *attr, int *methodp)
{
    int n;        /* iterates over tree elements */
    int bits;     /* bit counter */
    int length;   /* length value */
    int code;     /* code value */
    int dist;     /* distance index */

    file_type = attr;
    file_method = methodp;
    compressed_len = input_len = 0L;
        
    if (static_dtree[0].Len != 0) return; /* ct_init already called */

    /* Initialize the mapping length (0..255) -> length code (0..28) */
    length = 0;
    for (code = 0; code < LENGTH_CODES-1; code++) {
        base_length[code] = length;
        for (n = 0; n < (1<<extra_lbits[code]); n++) {
            length_code[length++] = (uch)code;
        }
    }
    Assert (length == 256, "ct_init: length != 256");
    /* Note that the length 255 (match length 258) can be represented
     * in two different ways: code 284 + 5 bits or code 285, so we
     * overwrite length_code[255] to use the best encoding:
     */
    length_code[length-1] = (uch)code;

    /* Initialize the mapping dist (0..32K) -> dist code (0..29) */
    dist = 0;
    for (code = 0 ; code < 16; code++) {
        base_dist[code] = dist;
        for (n = 0; n < (1<<extra_dbits[code]); n++) {
            dist_code[dist++] = (uch)code;
        }
    }
    Assert (dist == 256, "ct_init: dist != 256");
    dist >>= 7; /* from now on, all distances are divided by 128 */
    for ( ; code < D_CODES; code++) {
        base_dist[code] = dist << 7;
        for (n = 0; n < (1<<(extra_dbits[code]-7)); n++) {
            dist_code[256 + dist++] = (uch)code;
        }
    }
    Assert (dist == 256, "ct_init: 256+dist != 512");

    /* Construct the codes of the static literal tree */
    for (bits = 0; bits <= MAX_BITS; bits++) bl_count[bits] = 0;
    n = 0;
    while (n <= 143) static_ltree[n++].Len = 8, bl_count[8]++;
    while (n <= 255) static_ltree[n++].Len = 9, bl_count[9]++;
    while (n <= 279) static_ltree[n++].Len = 7, bl_count[7]++;
    while (n <= 287) static_ltree[n++].Len = 8, bl_count[8]++;
    /* Codes 286 and 287 do not exist, but we must include them in the
     * tree construction to get a canonical Huffman tree (longest code
     * all ones)
     */
    gen_codes((ct_data near *)static_ltree, L_CODES+1);

    /* The static distance tree is trivial: */
    for (n = 0; n < D_CODES; n++) {
        static_dtree[n].Len = 5;
        static_dtree[n].Code = bi_reverse(n, 5);
    }

    /* Initialize the first block of the first file: */
    init_block();
}

/* ===========================================================================
 * Initialize a new block.
 */
local void init_block()
{
    int n; /* iterates over tree elements */

    /* Initialize the trees. */
    for (n = 0; n < L_CODES;  n++) dyn_ltree[n].Freq = 0;
    for (n = 0; n < D_CODES;  n++) dyn_dtree[n].Freq = 0;
    for (n = 0; n < BL_CODES; n++) bl_tree[n].Freq = 0;

    dyn_ltree[END_BLOCK].Freq = 1;
    opt_len = static_len = 0L;
    last_lit = last_dist = last_flags = 0;
    flags = 0; flag_bit = 1;
}

#define SMALLEST 1
/* Index within the heap array of least frequent node in the Huffman tree */


/* ===========================================================================
 * Remove the smallest element from the heap and recreate the heap with
 * one less element. Updates heap and heap_len.
 */
#define pqremove(tree, top) \
{\
    top = heap[SMALLEST]; \
    heap[SMALLEST] = heap[heap_len--]; \
    pqdownheap(tree, SMALLEST); \
}

/* ===========================================================================
 * Compares to subtrees, using the tree depth as tie breaker when
 * the subtrees have equal frequency. This minimizes the worst case length.
 */
#define smaller(tree, n, m) \
   (tree[n].Freq < tree[m].Freq || \
   (tree[n].Freq == tree[m].Freq && depth[n] <= depth[m]))


local void pqdownheap(ct_data near *tree, int k)
{
    int v = heap[k];
    int j = k << 1;  /* left son of k */
    while (j <= heap_len) {
        /* Set j to the smallest of the two sons: */
        if (j < heap_len && smaller(tree, heap[j+1], heap[j])) j++;

        /* Exit if v is smaller than both sons */
        if (smaller(tree, v, heap[j])) break;

        /* Exchange v with the smallest son */
        heap[k] = heap[j];  k = j;

        /* And continue down the tree, setting j to the left son of k */
        j <<= 1;
    }
    heap[k] = v;
}

local void gen_bitlen(tree_desc near *desc)
{
    ct_data near *tree  = desc->dyn_tree;
    int near *extra     = desc->extra_bits;
    int base            = desc->extra_base;
    int max_code        = desc->max_code;
    int max_length      = desc->max_length;
    ct_data near *stree = desc->static_tree;
    int h;              /* heap index */
    int n, m;           /* iterate over the tree elements */
    int bits;           /* bit length */
    int xbits;          /* extra bits */
    ush f;              /* frequency */
    int overflow = 0;   /* number of elements with bit length too large */

    for (bits = 0; bits <= MAX_BITS; bits++) bl_count[bits] = 0;

    /* In a first pass, compute the optimal bit lengths (which may
     * overflow in the case of the bit length tree).
     */
    tree[heap[heap_max]].Len = 0; /* root of the heap */

    for (h = heap_max+1; h < HEAP_SIZE; h++) {
        n = heap[h];
        bits = tree[tree[n].Dad].Len + 1;
        if (bits > max_length) bits = max_length, overflow++;
        tree[n].Len = (ush)bits;
        /* We overwrite tree[n].Dad which is no longer needed */

        if (n > max_code) continue; /* not a leaf node */

        bl_count[bits]++;
        xbits = 0;
        if (n >= base) xbits = extra[n-base];
        f = tree[n].Freq;
        opt_len += (ulg)f * (bits + xbits);
        if (stree) static_len += (ulg)f * (stree[n].Len + xbits);
    }
    if (overflow == 0) return;

    Trace((stderr,"\nbit length overflow\n"));
    /* This happens for example on obj2 and pic of the Calgary corpus */

    /* Find the first bit length which could increase: */
    do {
        bits = max_length-1;
        while (bl_count[bits] == 0) bits--;
        bl_count[bits]--;      /* move one leaf down the tree */
        bl_count[bits+1] += 2; /* move one overflow item as its brother */
        bl_count[max_length]--;
        /* The brother of the overflow item also moves one step up,
         * but this does not affect bl_count[max_length]
         */
        overflow -= 2;
    } while (overflow > 0);

    /* Now recompute all bit lengths, scanning in increasing frequency.
     * h is still equal to HEAP_SIZE. (It is simpler to reconstruct all
     * lengths instead of fixing only the wrong ones. This idea is taken
     * from 'ar' written by Haruhiko Okumura.)
     */
    for (bits = max_length; bits != 0; bits--) {
        n = bl_count[bits];
        while (n != 0) {
            m = heap[--h];
            if (m > max_code) continue;
            if (tree[m].Len != (unsigned) bits) {
                Trace((stderr,"code %d bits %d->%d\n", m, tree[m].Len, bits));
                opt_len += ((long)bits-(long)tree[m].Len)*(long)tree[m].Freq;
                tree[m].Len = (ush)bits;
            }
            n--;
        }
    }
}

local void gen_codes (ct_data near *tree, int max_code)
{
    ush next_code[MAX_BITS+1];
    ush code = 0;             
    int bits;                 
    int n;                    

    for (bits = 1; bits <= MAX_BITS; bits++) {
        next_code[bits] = code = (code + bl_count[bits-1]) << 1;
    }
    Assert (code + bl_count[MAX_BITS]-1 == (1<<MAX_BITS)-1,
            "inconsistent bit counts");
    Tracev((stderr,"\ngen_codes: max_code %d ", max_code));

    for (n = 0;  n <= max_code; n++) {
        int len = tree[n].Len;
        if (len == 0) continue;
        tree[n].Code = bi_reverse(next_code[len]++, len);

        Tracec(tree != static_ltree, (stderr,"\nn %3d %c l %2d c %4x (%x) ",
             n, (isgraph(n) ? n : ' '), len, tree[n].Code, next_code[len]-1));
    }
}


local void build_tree_1(tree_desc near *desc)
{
    ct_data near *tree   = desc->dyn_tree;
    ct_data near *stree  = desc->static_tree;
    int elems            = desc->elems;
    int n, m;          /* iterate over heap elements */
    int max_code = -1; /* largest code with non zero frequency */
    int node = elems;  /* next internal node of the tree */


    heap_len = 0, heap_max = HEAP_SIZE;

    for (n = 0; n < elems; n++) {
        if (tree[n].Freq != 0) {
            heap[++heap_len] = max_code = n;
            depth[n] = 0;
        } else {
            tree[n].Len = 0;
        }
    }


    while (heap_len < 2) {
        int new2 = heap[++heap_len] = (max_code < 2 ? ++max_code : 0);
        tree[new2].Freq = 1;
        depth[new2] = 0;
        opt_len--; if (stree) static_len -= stree[new2].Len;
    }
    desc->max_code = max_code;

    for (n = heap_len/2; n >= 1; n--) pqdownheap(tree, n);

    do {
        pqremove(tree, n);   
        m = heap[SMALLEST];  

        heap[--heap_max] = n;
        heap[--heap_max] = m;

        tree[node].Freq = tree[n].Freq + tree[m].Freq;
        depth[node] = (uch) (MAX(depth[n], depth[m]) + 1);
        tree[n].Dad = tree[m].Dad = (ush)node;
#ifdef DUMP_BL_TREE
        if (tree == bl_tree) {
            fprintf(stderr,"\nnode %d(%d), sons %d(%d) %d(%d)",
                    node, tree[node].Freq, n, tree[n].Freq, m, tree[m].Freq);
        }
#endif
        /* and insert the new node in the heap */
        heap[SMALLEST] = node++;
        pqdownheap(tree, SMALLEST);

    } while (heap_len >= 2);

    heap[--heap_max] = heap[SMALLEST];

    gen_bitlen((tree_desc near *)desc);
    gen_codes ((ct_data near *)tree, max_code);
}


local void scan_tree (ct_data near *tree, int max_code)
{
    int n;                     
    int prevlen = -1;          
    int curlen;                
    int nextlen = tree[0].Len; 
    int count = 0;             
    int max_count = 7;         
    int min_count = 4;         

    if (nextlen == 0) max_count = 138, min_count = 3;
    tree[max_code+1].Len = (ush)0xffff; /* guard */

    for (n = 0; n <= max_code; n++) {
        curlen = nextlen; nextlen = tree[n+1].Len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            bl_tree[curlen].Freq += count;
        } else if (curlen != 0) {
            if (curlen != prevlen) bl_tree[curlen].Freq++;
            bl_tree[REP_3_6].Freq++;
        } else if (count <= 10) {
            bl_tree[REPZ_3_10].Freq++;
        } else {
            bl_tree[REPZ_11_138].Freq++;
        }
        count = 0; prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138, min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6, min_count = 3;
        } else {
            max_count = 7, min_count = 4;
        }
    }
}

local void send_tree (ct_data near *tree, int max_code)
{
    int n;                     
    int prevlen = -1;          
    int curlen;                
    int nextlen = tree[0].Len; 
    int count = 0;             
    int max_count = 7;         
    int min_count = 4;         
    if (nextlen == 0) max_count = 138, min_count = 3;

    for (n = 0; n <= max_code; n++) {
        curlen = nextlen; nextlen = tree[n+1].Len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            do { send_code(curlen, bl_tree); } while (--count != 0);

        } else if (curlen != 0) {
            if (curlen != prevlen) {
                send_code(curlen, bl_tree); count--;
            }
            Assert(count >= 3 && count <= 6, " 3_6?");
            send_code(REP_3_6, bl_tree); send_bits(count-3, 2);

        } else if (count <= 10) {
            send_code(REPZ_3_10, bl_tree); send_bits(count-3, 3);

        } else {
            send_code(REPZ_11_138, bl_tree); send_bits(count-11, 7);
        }
        count = 0; prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138, min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6, min_count = 3;
        } else {
            max_count = 7, min_count = 4;
        }
    }
}

local int build_bl_tree()
{
    int max_blindex;
    scan_tree((ct_data near *)dyn_ltree, l_desc.max_code);
    scan_tree((ct_data near *)dyn_dtree, d_desc.max_code);
    build_tree_1((tree_desc near *)(&bl_desc));
    
    for (max_blindex = BL_CODES-1; max_blindex >= 3; max_blindex--) {
        if (bl_tree[bl_order[max_blindex]].Len != 0) break;
    }
    opt_len += 3*(max_blindex+1) + 5+5+4;
    Tracev((stderr, "\ndyn trees: dyn %lu, stat %lu", opt_len, static_len));

    return max_blindex;
}


local void send_all_trees(int lcodes, int dcodes, int blcodes)
{
    int rank;

    Assert (lcodes >= 257 && dcodes >= 1 && blcodes >= 4, "not enough codes");
    Assert (lcodes <= L_CODES && dcodes <= D_CODES && blcodes <= BL_CODES,
            "too many codes");
    Tracev((stderr, "\nbl counts: "));
    send_bits(lcodes-257, 5);
    send_bits(dcodes-1,   5);
    send_bits(blcodes-4,  4);
    for (rank = 0; rank < blcodes; rank++) {
        Tracev((stderr, "\nbl code %2d ", bl_order[rank]));
        send_bits(bl_tree[bl_order[rank]].Len, 3);
    }

    send_tree((ct_data near *)dyn_ltree, lcodes-1);
    send_tree((ct_data near *)dyn_dtree, dcodes-1);
}


off_t flush_block(char *buf, ulg stored_len, int pad, int eof)
{
    ulg opt_lenb, static_lenb;
    int max_blindex;

    flag_buf[last_flags] = flags;
    if (*file_type == (ush)UNKNOWN) set_file_type();
    build_tree_1((tree_desc near *)(&l_desc));
    Tracev((stderr, "\nlit data: dyn %lu, stat %lu", opt_len, static_len));
    build_tree_1((tree_desc near *)(&d_desc));
    Tracev((stderr, "\ndist data: dyn %lu, stat %lu", opt_len, static_len));
    max_blindex = build_bl_tree();

    opt_lenb = (opt_len+3+7)>>3;
    static_lenb = (static_len+3+7)>>3;
    input_len += stored_len;

    Trace((stderr, "\nopt %lu(%lu) stat %lu(%lu) stored %lu lit %u dist %u ",
            opt_lenb, opt_len, static_lenb, static_len, stored_len,
            last_lit, last_dist));

    if (static_lenb <= opt_lenb) opt_lenb = static_lenb;


#ifdef FORCE_METHOD
    if (level == 1 && eof && compressed_len == 0L) {
#else
    if (stored_len <= opt_lenb && eof && compressed_len == 0L && seekable()) {
#endif
        if (buf == (char*)0) error ((char *)"block vanished");

        copy_block(buf, (unsigned)stored_len, 0);
        compressed_len = stored_len << 3;
        *file_method = STORED;

#ifdef FORCE_METHOD
    } else if (level == 2 && buf != (char*)0) {
#else
    } else if (stored_len+4 <= opt_lenb && buf != (char*)0) {
#endif
        send_bits((STORED_BLOCK<<1)+eof, 3);
        compressed_len = (compressed_len + 3 + 7) & ~7L;
        compressed_len += (stored_len + 4) << 3;

        copy_block(buf, (unsigned)stored_len, 1);

#ifdef FORCE_METHOD
    } else if (level == 3) {
#else
    } else if (static_lenb == opt_lenb) {
#endif
        send_bits((STATIC_TREES<<1)+eof, 3);
        compress_block((ct_data near *)static_ltree, (ct_data near *)static_dtree);
        compressed_len += 3 + static_len;
    } else {
        send_bits((DYN_TREES<<1)+eof, 3);
        send_all_trees(l_desc.max_code+1, d_desc.max_code+1, max_blindex+1);
        compress_block((ct_data near *)dyn_ltree, (ct_data near *)dyn_dtree);
        compressed_len += 3 + opt_len;
    }
    Assert (compressed_len == bits_sent, "bad compressed size");
    init_block();

    if (eof) {
        Assert (input_len == bytes_in, "bad input size");
        bi_windup();
        compressed_len += 7;  /* align on byte boundary */
    } else if (pad && (compressed_len % 8) != 0) {
        send_bits((STORED_BLOCK<<1)+eof, 3);  /* send block type */
        compressed_len = (compressed_len + 3 + 7) & ~7L;
        copy_block(buf, 0, 1); /* with header */
    }

    return compressed_len >> 3;
}

/* ===========================================================================
 * Save the match info and tally the frequency counts. Return true if
 * the current block must be flushed.
 */
int ct_tally (int dist, int lc)
{
    l_buf[last_lit++] = (uch)lc;
    if (dist == 0) {
        /* lc is the unmatched char */
        dyn_ltree[lc].Freq++;
    } else {
        /* Here, lc is the match length - MIN_MATCH */
        dist--;             /* dist = match distance - 1 */
        Assert((ush)dist < (ush)MAX_DIST &&
               (ush)lc <= (ush)(MAX_MATCH-MIN_MATCH) &&
               (ush)d_code(dist) < (ush)D_CODES,  "ct_tally: bad match");

        dyn_ltree[length_code[lc]+LITERALS+1].Freq++;
        dyn_dtree[d_code(dist)].Freq++;

        d_buf[last_dist++] = (ush)dist;
        flags |= flag_bit;
    }
    flag_bit <<= 1;

    /* Output the flags if they fill a byte: */
    if ((last_lit & 7) == 0) {
        flag_buf[last_flags++] = flags;
        flags = 0, flag_bit = 1;
    }
    /* Try to guess if it is profitable to stop the current block here */
    if (level > 2 && (last_lit & 0xfff) == 0) {
        /* Compute an upper bound for the compressed length */
        ulg out_length = (ulg)last_lit*8L;
        ulg in_length = (ulg)strstart-block_start;
        int dcode;
        for (dcode = 0; dcode < D_CODES; dcode++) {
            out_length += (ulg)dyn_dtree[dcode].Freq*(5L+extra_dbits[dcode]);
        }
        out_length >>= 3;
        Trace((stderr,"\nlast_lit %u, last_dist %u, in %ld, out ~%ld(%ld%%) ",
               last_lit, last_dist, in_length, out_length,
               100L - out_length*100L/in_length));
        if (last_dist < last_lit/2 && out_length < in_length/2) return 1;
    }
    return (last_lit == LIT_BUFSIZE-1 || last_dist == DIST_BUFSIZE);
    /* We avoid equality with LIT_BUFSIZE because of wraparound at 64K
     * on 16 bit machines and because stored blocks are restricted to
     * 64K-1 bytes.
     */
}


local void compress_block(ct_data near *ltree, ct_data near *dtree)
{
    unsigned dist;    
    int lc;           
    unsigned lx = 0;  
    unsigned dx = 0;  
    unsigned fx = 0;  
    uch flag = 0;     
    unsigned code;    
    int extra;        

    if (last_lit != 0) do {
        if ((lx & 7) == 0) flag = flag_buf[fx++];
        lc = l_buf[lx++];
        if ((flag & 1) == 0) {
            send_code(lc, ltree); /* send a literal byte */
            Tracecv(isgraph(lc), (stderr," '%c' ", lc));
        } else {
            /* Here, lc is the match length - MIN_MATCH */
            code = length_code[lc];
            send_code(code+LITERALS+1, ltree); /* send the length code */
            extra = extra_lbits[code];
            if (extra != 0) {
                lc -= base_length[code];
                send_bits(lc, extra);        /* send the extra length bits */
            }
            dist = d_buf[dx++];
            /* Here, dist is the match distance - 1 */
            code = d_code(dist);
            Assert (code < D_CODES, "bad d_code");

            send_code(code, dtree);       /* send the distance code */
            extra = extra_dbits[code];
            if (extra != 0) {
                dist -= base_dist[code];
                send_bits(dist, extra);   /* send the extra distance bits */
            }
        }
        flag >>= 1;
    } while (lx < last_lit);

    send_code(END_BLOCK, ltree);
}

local void set_file_type()
{
    int n = 0;
    unsigned ascii_freq = 0;
    unsigned bin_freq = 0;
    while (n < 7)        bin_freq += dyn_ltree[n++].Freq;
    while (n < 128)    ascii_freq += dyn_ltree[n++].Freq;
    while (n < LITERALS) bin_freq += dyn_ltree[n++].Freq;
    *file_type = bin_freq > (ascii_freq >> 2) ? BINARY : ASCII;
    if (*file_type == BINARY && translate_eol) {
        warning ((char *)"-l used on binary file");
    }
}


#ifdef RCSID
static char rcsid[] = "$Id: unlzh.c,v 1.2 1993/06/24 10:59:01 jloup Exp $";
#endif

local unsigned  decode  OF((unsigned count, uch buffer[]));
local void decode_start OF((void));
local void huf_decode_start OF((void));
local unsigned decode_c     OF((void));
local unsigned decode_p     OF((void));
local void read_pt_len      OF((int nn, int nbit, int i_special));
local void read_c_len       OF((void));
local void fillbuf      OF((int n));
local unsigned getbits  OF((int n));
local void init_getbits OF((void));

local void make_table OF((int nchar, uch bitlen[],
			  int tablebits, ush table[]));


#define DICBIT    13 
#define DICSIZ ((unsigned) 1 << DICBIT)

#ifndef CHAR_BIT
#  define CHAR_BIT 8
#endif

#ifndef UCHAR_MAX
#  define UCHAR_MAX 255
#endif

#define BITBUFSIZ (CHAR_BIT * 2 * sizeof(char))

#define MAXMATCH 256 
#define THRESHOLD  3 

#define NC (UCHAR_MAX + MAXMATCH + 2 - THRESHOLD)
#define CBIT 9 
#define CODE_BIT  16 

#define NP (DICBIT + 1)
#define NT (CODE_BIT + 3)
#define PBIT 4  
#define TBIT 5  
#if NT > NP
# define NPT NT
#else
# define NPT NP
#endif

#define left  prev
#define right head
#if NC > (1<<(BITS-2))
    error cannot overlay left+right and prev
#endif
#define c_len outbuf
#if NC > OUTBUFSIZ
    error cannot overlay c_len and outbuf
#endif

local uch pt_len[NPT];
local unsigned blocksize;
local ush pt_table[256];
#define c_table d_buf
#if (DIST_BUFSIZE-1) < 4095
    error cannot overlay c_table and d_buf
#endif

local ush       io_bitbuf;
local unsigned  subbitbuf;
local int       bitcount;

local void fillbuf(int n)
{
    io_bitbuf <<= n;
    while (n > bitcount) {
	io_bitbuf |= subbitbuf << (n -= bitcount);
	subbitbuf = (unsigned)try_byte();
	if ((int)subbitbuf == EOF) subbitbuf = 0;
	bitcount = CHAR_BIT;
    }
    io_bitbuf |= subbitbuf >> (bitcount -= n);
}

local unsigned getbits(int n)
{
    unsigned x;

    x = io_bitbuf >> (BITBUFSIZ - n);  fillbuf(n);
    return x;
}

local void init_getbits()
{
    io_bitbuf = 0;  subbitbuf = 0;  bitcount = 0;
    fillbuf(BITBUFSIZ);
}

local void make_table(int nchar, uch bitlen[], int tablebits, ush table[])
{
    ush count[17], weight[17], start[18], *p;
    unsigned i, k, len, ch, jutbits, avail, nextcode, mask;

    for (i = 1; i <= 16; i++) count[i] = 0;
    for (i = 0; i < (unsigned)nchar; i++) count[bitlen[i]]++;

    start[1] = 0;
    for (i = 1; i <= 16; i++)
	start[i + 1] = start[i] + (count[i] << (16 - i));
    if ((start[17] & 0xffff) != 0)
	error((char *)"Bad table\n");

    jutbits = 16 - tablebits;
    for (i = 1; i <= (unsigned)tablebits; i++) {
	start[i] >>= jutbits;
	weight[i] = (unsigned) 1 << (tablebits - i);
    }
    while (i <= 16) {
	weight[i] = (unsigned) 1 << (16 - i);
	i++;
    }

    i = start[tablebits + 1] >> jutbits;
    if (i != 0) {
	k = 1 << tablebits;
	while (i != k) table[i++] = 0;
    }

    avail = nchar;
    mask = (unsigned) 1 << (15 - tablebits);
    for (ch = 0; ch < (unsigned)nchar; ch++) {
	if ((len = bitlen[ch]) == 0) continue;
	nextcode = start[len] + weight[len];
	if (len <= (unsigned)tablebits) {
	    for (i = start[len]; i < nextcode; i++) table[i] = ch;
	} else {
	    k = start[len];
	    p = &table[k >> jutbits];
	    i = len - tablebits;
	    while (i != 0) {
		if (*p == 0) {
		    right[avail] = left[avail] = 0;
		    *p = avail++;
		}
		if (k & mask) p = &right[*p];
		else          p = &left[*p];
		k <<= 1;  i--;
	    }
	    *p = ch;
	}
	start[len] = nextcode;
    }
}

local void read_pt_len(int nn, int nbit, int i_special)
{
    int i, c, n;
    unsigned mask;

    n = getbits(nbit);
    if (n == 0) {
	c = getbits(nbit);
	for (i = 0; i < nn; i++) pt_len[i] = 0;
	for (i = 0; i < 256; i++) pt_table[i] = c;
    } else {
	i = 0;
	while (i < n) {
	    c = io_bitbuf >> (BITBUFSIZ - 3);
	    if (c == 7) {
		mask = (unsigned) 1 << (BITBUFSIZ - 1 - 3);
		while (mask & io_bitbuf) {  mask >>= 1;  c++;  }
	    }
	    fillbuf((c < 7) ? 3 : c - 3);
	    pt_len[i++] = c;
	    if (i == i_special) {
		c = getbits(2);
		while (--c >= 0) pt_len[i++] = 0;
	    }
	}
	while (i < nn) pt_len[i++] = 0;
	make_table(nn, pt_len, 8, pt_table);
    }
}

local void read_c_len()
{
    int i, c, n;
    unsigned mask;

    n = getbits(CBIT);
    if (n == 0) {
	c = getbits(CBIT);
	for (i = 0; i < NC; i++) c_len[i] = 0;
	for (i = 0; i < 4096; i++) c_table[i] = c;
    } else {
	i = 0;
	while (i < n) {
	    c = pt_table[io_bitbuf >> (BITBUFSIZ - 8)];
	    if (c >= NT) {
		mask = (unsigned) 1 << (BITBUFSIZ - 1 - 8);
		do {
		    if (io_bitbuf & mask) c = right[c];
		    else               c = left [c];
		    mask >>= 1;
		} while (c >= NT);
	    }
	    fillbuf((int) pt_len[c]);
	    if (c <= 2) {
		if      (c == 0) c = 1;
		else if (c == 1) c = getbits(4) + 3;
		else             c = getbits(CBIT) + 20;
		while (--c >= 0) c_len[i++] = 0;
	    } else c_len[i++] = c - 2;
	}
	while (i < NC) c_len[i++] = 0;
	make_table(NC, c_len, 12, c_table);
    }
}

local unsigned decode_c()
{
    unsigned j, mask;

    if (blocksize == 0) {
	blocksize = getbits(16);
	if (blocksize == 0) {
	    return NC;
	}
	read_pt_len(NT, TBIT, 3);
	read_c_len();
	read_pt_len(NP, PBIT, -1);
    }
    blocksize--;
    j = c_table[io_bitbuf >> (BITBUFSIZ - 12)];
    if (j >= NC) {
	mask = (unsigned) 1 << (BITBUFSIZ - 1 - 12);
	do {
	    if (io_bitbuf & mask) j = right[j];
	    else               j = left [j];
	    mask >>= 1;
	} while (j >= NC);
    }
    fillbuf((int) c_len[j]);
    return j;
}

local unsigned decode_p()
{
    unsigned j, mask;

    j = pt_table[io_bitbuf >> (BITBUFSIZ - 8)];
    if (j >= NP) {
	mask = (unsigned) 1 << (BITBUFSIZ - 1 - 8);
	do {
	    if (io_bitbuf & mask) j = right[j];
	    else               j = left [j];
	    mask >>= 1;
	} while (j >= NP);
    }
    fillbuf((int) pt_len[j]);
    if (j != 0) j = ((unsigned) 1 << (j - 1)) + getbits((int) (j - 1));
    return j;
}

local void huf_decode_start()
{
    init_getbits();  blocksize = 0;
}

local int j;    
local int done; 

local void decode_start()
{
    huf_decode_start();
    j = 0;
    done = 0;
}

local unsigned decode(unsigned count, uch buffer[])
{
    local unsigned i;
    unsigned r, c;

    r = 0;
    while (--j >= 0) {
	buffer[r] = buffer[i];
	i = (i + 1) & (DICSIZ - 1);
	if (++r == count) return r;
    }
    for ( ; ; ) {
	c = decode_c();
	if (c == NC) {
	    done = 1;
	    return r;
	}
	if (c <= UCHAR_MAX) {
	    buffer[r] = c;
	    if (++r == count) return r;
	} else {
	    j = c - (UCHAR_MAX + 1 - THRESHOLD);
	    i = (r - decode_p() - 1) & (DICSIZ - 1);
	    while (--j >= 0) {
		buffer[r] = buffer[i];
		i = (i + 1) & (DICSIZ - 1);
		if (++r == count) return r;
	    }
	}
    }
}

int unlzh(int in, int out)
{
    unsigned n;
    ifd = in;
    ofd = out;

    decode_start();
    while (!done) {
	n = decode((unsigned) DICSIZ, window);
	if (!test && n > 0) {
	    write_buf(out, (char*)window, n);
	}
    }
    return OK;
}

#ifdef RCSID
static char rcsid[] = "$Id: unlzw.c,v 0.15 1993/06/10 13:28:35 jloup Exp $";
#endif

typedef	unsigned char	char_type;
typedef          long   code_int;
typedef unsigned long 	count_int;
typedef unsigned short	count_short;
typedef unsigned long 	cmp_code_int;

#define MAXCODE(n)	(1L << (n))
    
#ifndef	REGISTERS
#	define	REGISTERS	2
#endif
#define	REG1	
#define	REG2	
#define	REG3	
#define	REG4	
#define	REG5	
#define	REG6	
#define	REG7	
#define	REG8	
#define	REG9	
#define	REG10
#define	REG11	
#define	REG12	
#define	REG13
#define	REG14
#define	REG15
#define	REG16
#if REGISTERS >= 1
#	undef	REG1
#	define	REG1	register
#endif
#if REGISTERS >= 2
#	undef	REG2
#	define	REG2	register
#endif
#if REGISTERS >= 3
#	undef	REG3
#	define	REG3	register
#endif
#if REGISTERS >= 4
#	undef	REG4
#	define	REG4	register
#endif
#if REGISTERS >= 5
#	undef	REG5
#	define	REG5	register
#endif
#if REGISTERS >= 6
#	undef	REG6
#	define	REG6	register
#endif
#if REGISTERS >= 7
#	undef	REG7
#	define	REG7	register
#endif
#if REGISTERS >= 8
#	undef	REG8
#	define	REG8	register
#endif
#if REGISTERS >= 9
#	undef	REG9
#	define	REG9	register
#endif
#if REGISTERS >= 10
#	undef	REG10
#	define	REG10	register
#endif
#if REGISTERS >= 11
#	undef	REG11
#	define	REG11	register
#endif
#if REGISTERS >= 12
#	undef	REG12
#	define	REG12	register
#endif
#if REGISTERS >= 13
#	undef	REG13
#	define	REG13	register
#endif
#if REGISTERS >= 14
#	undef	REG14
#	define	REG14	register
#endif
#if REGISTERS >= 15
#	undef	REG15
#	define	REG15	register
#endif
#if REGISTERS >= 16
#	undef	REG16
#	define	REG16	register
#endif
    
#ifndef	BYTEORDER
#	define	BYTEORDER	0000
#endif
	
#ifndef	NOALLIGN
#	define	NOALLIGN	0
#endif


union	bytes {
    long  word;
    struct {
#if BYTEORDER == 4321
	char_type	b1;
	char_type	b2;
	char_type	b3;
	char_type	b4;
#else
#if BYTEORDER == 1234
	char_type	b4;
	char_type	b3;
	char_type	b2;
	char_type	b1;
#else
#	undef	BYTEORDER
	int  dummy;
#endif
#endif
    } bytes;
};

#if BYTEORDER == 4321 && NOALLIGN == 1
#  define input(b,o,c,n,m){ \
     (c) = (*(long *)(&(b)[(o)>>3])>>((o)&0x7))&(m); \
     (o) += (n); \
   }
#else
#  define input(b,o,c,n,m){ \
     REG1 char_type *p = &(b)[(o)>>3]; \
     (c) = ((((long)(p[0]))|((long)(p[1])<<8)| \
     ((long)(p[2])<<16))>>((o)&0x7))&(m); \
     (o) += (n); \
   }
#endif

#ifndef MAXSEG_64K
   /* DECLARE(ush, tab_prefix, (1<<BITS)); -- prefix code */
#  define tab_prefixof(i) tab_prefix[i]
#  define clear_tab_prefixof()	memzero(tab_prefix, 256);
#else
   ush *tab_prefix[2];
#  define tab_prefixof(i) tab_prefix[(i)&1][(i)>>1]
#  define clear_tab_prefixof()	\
      memzero(tab_prefix0, 128), \
      memzero(tab_prefix1, 128);
#endif
#define de_stack        ((char_type *)(&d_buf[DIST_BUFSIZE-1]))
#define tab_suffixof(i) tab_suffix[i]

int block_mode = BLOCK_MODE;


int unlzw(int in, int out) 
{
    REG2   char_type  *stackp;
    REG3   code_int   code;
    REG4   int        finchar;
    REG5   code_int   oldcode;
    REG6   code_int   incode;
    REG7   long       inbits;
    REG8   long       posbits;
    REG9   int        outpos;
    REG11  unsigned   bitmask;
    REG12  code_int   free_ent;
    REG13  code_int   maxcode;
    REG14  code_int   maxmaxcode;
    REG15  int        n_bits;
    REG16  int        rsize;
    
#ifdef MAXSEG_64K
    tab_prefix[0] = tab_prefix0;
    tab_prefix[1] = tab_prefix1;
#endif
    maxbits = get_byte();
    block_mode = maxbits & BLOCK_MODE;
    if ((maxbits & LZW_RESERVED) != 0) {
	WARN((stderr, "\n%s: %s: warning, unknown flags 0x%x\n",
	      progname, ifname, maxbits & LZW_RESERVED));
    }
    maxbits &= BIT_MASK;
    maxmaxcode = MAXCODE(maxbits);
    
    if (maxbits > BITS) {
	fprintf(stderr,
		"\n%s: %s: compressed with %d bits, can only handle %d bits\n",
		progname, ifname, maxbits, BITS);
	exit_code = ERROR;
	return ERROR;
    }
    rsize = insize;
    maxcode = MAXCODE(n_bits = INIT_BITS)-1;
    bitmask = (1<<n_bits)-1;
    oldcode = -1;
    finchar = 0;
    outpos = 0;
    posbits = inptr<<3;

    free_ent = ((block_mode) ? FIRST : 256);
    
    clear_tab_prefixof();
    
    for (code = 255 ; code >= 0 ; --code) {
	tab_suffixof(code) = (char_type)code;
    }
    do {
	REG1 int i;
	int  e;
	int  o;
	
    resetbuf:
	e = insize-(o = (posbits>>3));
	
	for (i = 0 ; i < e ; ++i) {
	    inbuf[i] = inbuf[i+o];
	}
	insize = e;
	posbits = 0;
	
	if (insize < INBUF_EXTRA) {
	    if ((rsize = read(in, (char*)inbuf+insize, INBUFSIZ)) == -1) {
		read_error();
	    }
	    insize += rsize;
	    bytes_in += (off_t)rsize;
	}
	inbits = ((rsize != 0) ? ((long)insize - insize%n_bits)<<3 : 
		  ((long)insize<<3)-(n_bits-1));
	
	while (inbits > posbits) {
	    if (free_ent > maxcode) {
		posbits = ((posbits-1) +
			   ((n_bits<<3)-(posbits-1+(n_bits<<3))%(n_bits<<3)));
		++n_bits;
		if (n_bits == maxbits) {
		    maxcode = maxmaxcode;
		} else {
		    maxcode = MAXCODE(n_bits)-1;
		}
		bitmask = (1<<n_bits)-1;
		goto resetbuf;
	    }
	    input(inbuf,posbits,code,n_bits,bitmask);
	    Tracev((stderr, "%d ", code));

	    if (oldcode == -1) {
		if (code >= 256) error((char *)"corrupt input.");
		outbuf[outpos++] = (char_type)(finchar = (int)(oldcode=code));
		continue;
	    }
	    if (code == CLEAR && block_mode) {
		clear_tab_prefixof();
		free_ent = FIRST - 1;
		posbits = ((posbits-1) +
			   ((n_bits<<3)-(posbits-1+(n_bits<<3))%(n_bits<<3)));
		maxcode = MAXCODE(n_bits = INIT_BITS)-1;
		bitmask = (1<<n_bits)-1;
		goto resetbuf;
	    }
	    incode = code;
	    stackp = de_stack;
	    
	    if (code >= free_ent) {
		if (code > free_ent) {
#ifdef DEBUG		    
		    char_type *p;

		    posbits -= n_bits;
		    p = &inbuf[posbits>>3];
		    fprintf(stderr,
			    "code:%ld free_ent:%ld n_bits:%d insize:%u\n",
			    code, free_ent, n_bits, insize);
		    fprintf(stderr,
			    "posbits:%ld inbuf:%02X %02X %02X %02X %02X\n",
			    posbits, p[-1],p[0],p[1],p[2],p[3]);
#endif
		    if (!test && outpos > 0) {
			write_buf(out, (char*)outbuf, outpos);
			bytes_out += (off_t)outpos;
		    }
		    error(to_stdout ? (char *)"corrupt input." :
			  (char *)"corrupt input. Use zcat to recover some data.");
		}
		*--stackp = (char_type)finchar;
		code = oldcode;
	    }

	    while ((cmp_code_int)code >= (cmp_code_int)256) {
		/* Generate output characters in reverse order */
		*--stackp = tab_suffixof(code);
		code = tab_prefixof(code);
	    }
	    *--stackp =	(char_type)(finchar = tab_suffixof(code));
	    
	    /* And put them out in forward order */
	    {
		REG1 int	i;
	    
		if (outpos+(i = (de_stack-stackp)) >= OUTBUFSIZ) {
		    do {
			if (i > OUTBUFSIZ-outpos) i = OUTBUFSIZ-outpos;

			if (i > 0) {
			    memcpy(outbuf+outpos, stackp, i);
			    outpos += i;
			}
			if (outpos >= OUTBUFSIZ) {
			    if (!test) {
				write_buf(out, (char*)outbuf, outpos);
				bytes_out += (off_t)outpos;
			    }
			    outpos = 0;
			}
			stackp+= i;
		    } while ((i = (de_stack-stackp)) > 0);
		} else {
		    memcpy(outbuf+outpos, stackp, i);
		    outpos += i;
		}
	    }

	    if ((code = free_ent) < maxmaxcode) { /* Generate the new entry. */

		tab_prefixof(code) = (unsigned short)oldcode;
		tab_suffixof(code) = (char_type)finchar;
		free_ent = code+1;
	    } 
	    oldcode = incode;	/* Remember previous code.	*/
	}
    } while (rsize != 0);
    
    if (!test && outpos > 0) {
	write_buf(out, (char*)outbuf, outpos);
	bytes_out += (off_t)outpos;
    }
    return OK;
}
/* unpack.c -- decompress files in pack format.
 * Copyright (C) 1992-1993 Jean-loup Gailly
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License, see the file COPYING.
 */

#ifdef RCSID
static char rcsid[] = "$Id: unpack.c,v 1.4 1993/06/11 19:25:36 jloup Exp $";
#endif

#define MIN(a,b) ((a) <= (b) ? (a) : (b))
/* The arguments must not have side effects. */

#define MAX_BITLEN 25
/* Maximum length of Huffman codes. (Minor modifications to the code
 * would be needed to support 32 bits codes, but pack never generates
 * more than 24 bits anyway.)
 */

#define LITERALS 256
/* Number of literals, excluding the End of Block (EOB) code */

#define MAX_PEEK 12
/* Maximum number of 'peek' bits used to optimize traversal of the
 * Huffman tree.
 */

local ulg orig_len;       /* original uncompressed length */
local int max_len;        /* maximum bit length of Huffman codes */

local uch literal[LITERALS];


local int lit_base[MAX_BITLEN+1];


local int leaves [MAX_BITLEN+1]; /* Number of leaves for each bit length */
local int parents[MAX_BITLEN+1]; /* Number of parents for each bit length */

local int peek_bits; /* Number of peek bits currently used */

#define prefix_len outbuf

#if (1<<MAX_PEEK) > OUTBUFSIZ
    error cannot overlay prefix_len and outbuf
#endif

local ulg bitbuf;
local int valid;                  /* number of valid bits in bitbuf */

#define look_bits(code,bits,mask) \
{ \
  while (valid < (bits)) bitbuf = (bitbuf<<8) | (ulg)get_byte(), valid += 8; \
  code = (bitbuf >> (valid-(bits))) & (mask); \
}

#define skip_bits(bits)  (valid -= (bits))

#define clear_bitbuf() (valid = 0, bitbuf = 0)

local void read_tree  OF((void));
local void build_tree OF((void));

local void read_tree()
{
    int len;  /* bit length */
    int base; /* base offset for a sequence of leaves */
    int n;

    orig_len = 0;
    for (n = 1; n <= 4; n++) orig_len = (orig_len << 8) | (ulg)get_byte();

    max_len = (int)get_byte(); /* maximum bit length of Huffman codes */
    if (max_len > MAX_BITLEN) {
	error((char *)"invalid compressed data -- Huffman code > 32 bits");
    }

    n = 0;
    for (len = 1; len <= max_len; len++) {
	leaves[len] = (int)get_byte();
	n += leaves[len];
    }
    if (n > LITERALS) {
	error((char *)"too many leaves in Huffman tree");
    }
    Trace((stderr, "orig_len %lu, max_len %d, leaves %d\n",
	   orig_len, max_len, n));
    
    leaves[max_len]++;
    base = 0;
    for (len = 1; len <= max_len; len++) {
	lit_base[len] = base;
	for (n = leaves[len]; n > 0; n--) {
	    literal[base++] = (uch)get_byte();
	}
    }
    leaves[max_len]++;
}


local void build_tree()
{
    int nodes = 0; 
    int len;       
    uch *prefixp;  

    for (len = max_len; len >= 1; len--) {

	nodes >>= 1;
	parents[len] = nodes;
	
	lit_base[len] -= nodes;
	nodes += leaves[len];
    }

    peek_bits = MIN(max_len, MAX_PEEK);
    prefixp = &prefix_len[1<<peek_bits];
    for (len = 1; len <= peek_bits; len++) {
	int prefixes = leaves[len] << (peek_bits-len);
	while (prefixes--) *--prefixp = (uch)len;
    }
    while (prefixp > prefix_len) *--prefixp = 0;
}


int unpack(int in, int out)
{
    int len;
    unsigned eob;
    register unsigned peek;
    unsigned peek_mask;
    ifd = in;
    ofd = out;
    read_tree();     
    build_tree();    
    clear_bitbuf();  
    peek_mask = (1<<peek_bits)-1;

    eob = leaves[max_len]-1;
    Trace((stderr, "eob %d %x\n", max_len, eob));

    for (;;) {
	look_bits(peek, peek_bits, peek_mask);
	len = prefix_len[peek];
	if (len > 0) {
	    peek >>= peek_bits - len;
	} else {
	    ulg mask = peek_mask;
	    len = peek_bits;
	    do {
                len++, mask = (mask<<1)+1;
		look_bits(peek, len, mask);
	    } while (peek < (unsigned)parents[len]);
	}
	if (peek == eob && len == max_len) break;
	put_ubyte(literal[peek+lit_base[len]]);
	Tracev((stderr,"%02d %04x %c\n", len, peek,
		literal[peek+lit_base[len]]));
	skip_bits(len);
    }

    flush_window();
    if (orig_len != (ulg)(bytes_out & 0xffffffff)) {
	error((char *)"invalid compressed data--length error");
    }
    return OK;
}


#ifdef RCSID
static char rcsid[] = "$Id: unzip.c,v 0.13 1993/06/10 13:29:00 jloup Exp $";
#endif

#define LOCSIG 0x04034b50L
#define LOCFLG 6          
#define  CRPFLG 1         
#define  EXTFLG 8         
#define LOCHOW 8          
#define LOCTIM 10         
#define LOCCRC 14         
#define LOCSIZ 18         
#define LOCLEN 22         
#define LOCFIL 26         
#define LOCEXT 28         
#define LOCHDR 30         
#define EXTHDR 16         
#define RAND_HEAD_LEN  12 


int decrypt;
char *key;          
int pkzip = 0;      
int ext_header = 0; 

int check_zipfile(int in)
{
    uch *h = inbuf + inptr;

    ifd = in;
    inptr += LOCHDR + SH(h + LOCFIL) + SH(h + LOCEXT);

    if (inptr > insize || LG(h) != LOCSIG) {
	fprintf(stderr, "\n%s: %s: not a valid zip file\n",
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }
    method = h[LOCHOW];
    if (method != STORED && method != DEFLATED) {
	fprintf(stderr,
		"\n%s: %s: first entry not deflated or stored -- use unzip\n",
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }

    if ((decrypt = h[LOCFLG] & CRPFLG) != 0) {
	fprintf(stderr, "\n%s: %s: encrypted file -- use unzip\n",
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }

    ext_header = (h[LOCFLG] & EXTFLG) != 0;
    pkzip = 1;

    return OK;
}

int unzip(int in, int out)
{
    ulg orig_crc = 0;  
    ulg orig_len = 0;  
    int n;
    uch buf[EXTHDR];   
    int err = OK;

    ifd = in;
    ofd = out;

    updcrc(NULL, 0);  

    if (pkzip && !ext_header) { 
	orig_crc = LG(inbuf + LOCCRC);
	orig_len = LG(inbuf + LOCLEN);
    }

    /* Decompress */
    if (method == DEFLATED)  {

	int res = inflate();

	if (res == 3) {
	    error((char *)"out of memory");
	} else if (res != 0) {
	    error((char *)"invalid compressed data--format violated");
	}

    } else if (pkzip && method == STORED) {

	register ulg n = LG(inbuf + LOCLEN);

	if (n != LG(inbuf + LOCSIZ) - (decrypt ? RAND_HEAD_LEN : 0)) {

	    fprintf(stderr, "len %ld, siz %ld\n", n, LG(inbuf + LOCSIZ));
	    error((char *)"invalid compressed data--length mismatch");
	}
	while (n--) {
	    uch c = (uch)get_byte();
	    put_ubyte(c);
	}
	flush_window();
    } else {
	error((char *)"internal error, invalid method");
    }

    if (!pkzip)
    {
        for (n = 0; n < 8; n++)
        {
	    buf[n] = (uch)get_byte();
	}
	orig_crc = LG(buf);
	orig_len = LG(buf+4);

    } else if (ext_header) {  /* If extended header, check it */
	/* signature - 4bytes: 0x50 0x4b 0x07 0x08
	 * CRC-32 value
         * compressed size 4-bytes
         * uncompressed size 4-bytes
	 */
	for (n = 0; n < EXTHDR; n++) {
	    buf[n] = (uch)get_byte(); /* may cause an error if EOF */
	}
	orig_crc = LG(buf+4);
	orig_len = LG(buf+12);
    }

    /* Validate decompression */
    if (orig_crc != updcrc(outbuf, 0)) {
	fprintf(stderr, "\n%s: %s: invalid compressed data--crc error\n",
		progname, ifname);
	err = ERROR;
    }
    if (orig_len != (ulg)(bytes_out & 0xffffffff)) {
	fprintf(stderr, "\n%s: %s: invalid compressed data--length error\n",
		progname, ifname);
	err = ERROR;
    }

    /* Check if there are more entries in a pkzip file */
    if (pkzip && inptr + 4 < insize && LG(inbuf+inptr) == LOCSIG) {
	if (to_stdout) {
	    WARN((stderr,
		  "%s: %s has more than one entry--rest ignored\n",
		  progname, ifname));
	} else {
	    /* Don't destroy the input zip file */
	    fprintf(stderr,
		    "%s: %s has more than one entry -- unchanged\n",
		    progname, ifname);
	    err = ERROR;
	}
    }
    ext_header = pkzip = 0; /* for next file */
    if (err == OK) return OK;
    exit_code = ERROR;
    if (!test) abort_gzip();
    return err;
}


#ifdef RCSID
static char rcsid[] = "$Id: util.c,v 0.15 1993/06/15 09:04:13 jloup Exp $";
#endif

#ifndef CHAR_BIT
#  define CHAR_BIT 8
#endif

extern ulg crc_32_tab[];

int copy(int in, int out)
{
    errno = 0;
    while (insize != 0 && (int)insize != -1) {
	write_buf(out, (char*)inbuf, insize);
	bytes_out += insize;
	insize = read(in, (char*)inbuf, INBUFSIZ);
    }
    if ((int)insize == -1) {
	read_error();
    }
    bytes_in = bytes_out;
    return OK;
}


ulg updcrc(uch *s, unsigned n)
{
    register ulg c;
    static ulg crc = (ulg)0xffffffffL;

    if (s == NULL) {
	c = 0xffffffffL;
    } else {
	c = crc;
        if (n) do {
            c = crc_32_tab[((int)c ^ (*s++)) & 0xff] ^ (c >> 8);
        } while (--n);
    }
    crc = c;
    return c ^ 0xffffffffL;       /* (instead of ~c for 64-bit machines) */
}

void clear_bufs()
{
    outcnt = 0;
    insize = inptr = 0;
    bytes_in = bytes_out = 0L;
}

int fill_inbuf(int eof_ok)
{
    int len;
    insize = 0;
    do {
	len = read(ifd, (char*)inbuf+insize, INBUFSIZ-insize);
	if (len == 0) break;
	if (len == -1) {
	  read_error();
	  break;
	}
	insize += len;
    } while (insize < INBUFSIZ);

    if (insize == 0) {
	if (eof_ok) return EOF;
	flush_window();
	errno = 0;
	read_error();
    }
    bytes_in += (off_t)insize;
    inptr = 1;
    return inbuf[0];
}

void flush_outbuf()
{
    if (outcnt == 0) return;

    write_buf(ofd, (char *)outbuf, outcnt);
    bytes_out += (off_t)outcnt;
    outcnt = 0;
}

void flush_window()
{
    if (outcnt == 0) return;
    updcrc(window, outcnt);

    if (!test) {
	write_buf(ofd, (char *)window, outcnt);
    }
    bytes_out += (off_t)outcnt;
    outcnt = 0;
}

void write_buf(int fd, voidp buf, unsigned cnt)
{
    unsigned  n;

    while ((n = write(fd, buf, cnt)) != cnt) {
	if (n == (unsigned)(-1)) {
	    write_error();
	}
	cnt -= n;
	buf = (voidp)((char*)buf+n);
    }
}

char *strlwr(char *s)
{
    char *t;
    for (t = s; *t; t++)
      *t = tolow ((unsigned char) *t);
    return s;
}

char *base_name(char *fname)
{
    char *p;

    if ((p = strrchr(fname, PATH_SEP))  != NULL) fname = p+1;
#ifdef PATH_SEP2
    if ((p = strrchr(fname, PATH_SEP2)) != NULL) fname = p+1;
#endif
#ifdef PATH_SEP3
    if ((p = strrchr(fname, PATH_SEP3)) != NULL) fname = p+1;
#endif
#ifdef SUFFIX_SEP
    if ((p = strrchr(fname, SUFFIX_SEP)) != NULL) *p = '\0';
#endif
    if (casemap('A') == 'a') strlwr(fname);
    return fname;
}

int xunlink (char *filename)
{
  int r = unlink (filename);

#ifdef UNLINK_READONLY_BUG
  if (r != 0)
    {
      int e = errno;
      if (chmod (filename, S_IWUSR) != 0)
	{
	  errno = e;
	  return -1;
	}

      r = unlink (filename);
    }
#endif

  return r;
}

void make_simple_name(char *name)
{
    char *p = strrchr(name, '.');
    if (p == NULL) return;
    if (p == name) p++;
    do {
        if (*--p == '.') *p = '_';
    } while (p != name);
}


#if !defined HAVE_STRING_H && !defined STDC_HEADERS

#  ifndef __STDC__
#    define const
#  endif

int strspn  OF((const char *s, const char *accept));
int strcspn OF((const char *s, const char *reject));


int strspn(const char *s, const char *accept)
{
    register const char *p;
    register const char *a;
    register int count = 0;

    for (p = s; *p != '\0'; ++p) {
	for (a = accept; *a != '\0'; ++a) {
	    if (*p == *a) break;
	}
	if (*a == '\0') return count;
	++count;
    }
    return count;
}

int strcspn(const char *s, const char *reject)
{
    register int count = 0;

    while (*s != '\0') {
	if (strchr(reject, *s++) != NULL) return count;
	++count;
    }
    return count;
}

#endif

#define SEPARATOR	" \t"	/* separators in env variable */

char *add_envopt(int *argcp, char ***argvp, char *env)
{
    char *p;             /* running pointer through env variable */
    char **oargv;        /* runs through old argv array */
    char **nargv;        /* runs through new argv array */
    int	 oargc = *argcp; /* old argc */
    int  nargc = 0;      /* number of arguments in env variable */

    env = (char*)getenv(env);
    if (env == NULL) return NULL;

    p = (char*)xmalloc(strlen(env)+1);
    env = strcpy(p, env);                    /* keep env variable intact */

    for (p = env; *p; nargc++ ) {            /* move through env */
	p += strspn(p, SEPARATOR);	     /* skip leading separators */
	if (*p == '\0') break;

	p += strcspn(p, SEPARATOR);	     /* find end of word */
	if (*p) *p++ = '\0';		     /* mark it */
    }
    if (nargc == 0) {
	free(env);
	return NULL;
    }
    *argcp += nargc;
    nargv = (char**)calloc(*argcp+1, sizeof(char *));
    if (nargv == NULL) error((char *)"out of memory");
    oargv  = *argvp;
    *argvp = nargv;

    if (oargc-- < 0) error((char *)"argc<=0");
    *(nargv++) = *(oargv++);

    for (p = env; nargc > 0; nargc--) {
	p += strspn(p, SEPARATOR);
	*(nargv++) = p;
	while (*p++) ;
    }

    while (oargc--) *(nargv++) = *(oargv++);
    *nargv = NULL;
    return env;
}

void error(char *m)
{
    fprintf(stderr, "\n%s: %s: %s\n", progname, ifname, m);
    abort_gzip();
}

void warning (char *m)
{
    WARN ((stderr, "%s: %s: warning: %s\n", progname, ifname, m));
}

void read_error()
{
    int e = errno;
    fprintf(stderr, "\n%s: ", progname);
    if (e != 0) {
	errno = e;
	perror(ifname);
    } else {
	fprintf(stderr, "%s: unexpected end of file\n", ifname);
    }
    abort_gzip();
}

void write_error()
{
    int e = errno;
    fprintf(stderr, "\n%s: ", progname);
    errno = e;
    perror(ofname);
    abort_gzip();
}

void display_ratio(off_t num, off_t den, FILE *file)
{
    fprintf(file, "%5.1f%%", den == 0 ? 0 : 100.0 * num / den);
}

void fprint_off(FILE *file, off_t offset, int width)
{
    char buf[CHAR_BIT * sizeof (off_t)];
    char *p = buf + sizeof buf;

    if (offset < 0) {
	do
	  *--p = '0' - offset % 10;
	while ((offset /= 10) != 0);

	*--p = '-';
    } else {
	do
	  *--p = '0' + offset % 10;
	while ((offset /= 10) != 0);
    }

    width -= buf + sizeof buf - p;
    while (0 < width--) {
	putc (' ', file);
    }
    for (;  p < buf + sizeof buf;  p++)
	putc (*p, file);
}

voidp xmalloc (unsigned size)
{
    voidp cp = (voidp)malloc (size);

    if (cp == NULL) error((char *)"out of memory");
    return cp;
}

ulg crc_32_tab[] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

int rpmatch ();

int
yesno ()
{
  char buf[128];
  int len = 0;
  int c;

  while ((c = getchar ()) != EOF && c != '\n')
    if ((len > 0 && len < 127) || (len == 0 && !isspace (c)))
      buf[len++] = c;
  buf[len] = '\0';

  return rpmatch (buf) == 1;
}

#ifdef RCSID
static char rcsid[] = "$Id: zip.c,v 0.17 1993/06/10 13:29:25 jloup Exp $";
#endif

local ulg crc;       /* crc on uncompressed file data */
off_t header_bytes;   /* number of bytes in gzip header */

int zip(int in, int out)
{
    uch  flags = 0;         /* general purpose bit flags */
    ush  attr = 0;          /* ascii/binary flag */
    ush  deflate_flags = 0; /* pkzip -es, -en or -ex equivalent */

    ifd = in;
    ofd = out;
    outcnt = 0;
    method = DEFLATED;
    put_byte(GZIP_MAGIC[0]); /* magic header */
    put_byte(GZIP_MAGIC[1]);
    put_byte(DEFLATED);      /* compression method */

    if (save_orig_name) {
	flags |= ORIG_NAME;
    }
    put_byte(flags);         /* general flags */
    put_long(time_stamp == (time_stamp & 0xffffffff)
	     ? (ulg)time_stamp : (ulg)0);

    /* Write deflated file to zip file */
    crc = updcrc(0, 0);

    bi_init(out);
    ct_init(&attr, &method);
    lm_init(level, &deflate_flags);

    put_byte((uch)deflate_flags); /* extra flags */
    put_byte(OS_CODE);            /* OS identifier */

    if (save_orig_name) {
	char *p = base_name(ifname); /* Don't save the directory part. */
	do {
	    put_char(*p);
	} while (*p++);
    }
    header_bytes = (off_t)outcnt;

    (void)deflate();

#if !defined(NO_SIZE_CHECK) && !defined(RECORD_IO)
    if (ifile_size != -1L && bytes_in != ifile_size) {
	fprintf(stderr, "%s: %s: file size changed while zipping\n",
		progname, ifname);
    }
#endif

    put_long(crc);
    put_long((ulg)bytes_in);
    header_bytes += 2*sizeof(long);
    flush_outbuf();
    return OK;
}

int file_read(char *buf, unsigned size)
{
    unsigned len;

    Assert(insize == 0, "inbuf not empty");

    len = read(ifd, buf, size);
    if (len == 0) return (int)len;
    if (len == (unsigned)-1) {
	read_error();
	return EOF;
    }

    crc = updcrc((uch*)buf, len);
    bytes_in += (off_t)len;
    return (int)len;
}

int rpmatch (const char *response)
{
    return (*response == 'y' || *response == 'Y' ? 1
        : *response == 'n' || *response == 'N' ? 0 : -1);
}


int getopt_long (int argc, char * const *argv, const char *options,
        const struct option *long_options, int *opt_index)
{
    return _getopt_internal (argc, argv, options, long_options, opt_index, 0);
}

int getopt_long_only (int argc, char * const *argv, const char *options,
            const struct option *long_options, int *opt_index)
{
    return _getopt_internal (argc, argv, options, long_options, opt_index, 1);
}


