#if 1
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <stdint.h>
#else
#include "common.h"
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
static int first_nonopt;
static int last_nonopt;
static const uint8_t CHAR_BIT = 8;
static const uint8_t UCHAR_MAX = 255;
static const uint8_t MAX_SUFFIX = 30;
typedef void *voidp;
typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;
static const uint32_t LOCSIG = 0x04034b50;
static const uint8_t LOCFLG = 6;
static const uint8_t LOCSIZ = 18;
static const uint8_t LOCLEN = 22;
static const uint8_t EXTHDR = 16;
static const uint8_t OK = 0;
static const uint8_t ERROR = 1;
static const uint8_t STORED = 0;
static const uint8_t DEFLATED = 8;
static const uint32_t INBUFSIZ = 0x8000;
static const uint8_t INBUF_EXTRA = 64;
static const int32_t OUTBUFSIZ = 16384;
static const uint16_t OUTBUF_EXTRA = 2048;
static const uint32_t DIST_BUFSIZE = 0x8000;
unsigned outcnt;
int rsync = 0;
off_t header_bytes;
typedef int file_t;
const char PACK_MAGIC[] = "\037\036";
const char GZIP_MAGIC[] = "\037\213";
const char OLD_GZIP_MAGIC[] = "\037\236";
const char LZH_MAGIC[] = "\037\240";
const char PKZIP_MAGIC[] = "\120\113\003\004";
static const uint8_t CONTINUATION = 2;
static const uint8_t ORIG_NAME = 8;
static const uint8_t RESERVED = 0xc0;
static const uint8_t BINARY = 0;
static const uint8_t ASCII = 1;
static const uint32_t WSIZE = 0x8000;
static const uint16_t MIN_MATCH = 3;
static const uint16_t MAX_MATCH = 258;
static const uint16_t MIN_LOOKAHEAD = 262;
static const uint32_t MAX_DIST = WSIZE - 262;
static const uint8_t BITS = 16;
static const uint8_t INIT_BITS = 9;
const char LZW_MAGIC[] = "\037\235";
static const uint8_t BLOCK_MODE = 0x80;
static const uint8_t LZW_RESERVED = 0x60;
static const uint16_t FIRST = 257;
int lzw(int in, int out);
int unlzw(int in, int out);
static const uint32_t HASH_SIZE = (uint32_t)(1<<15);
static const uint32_t HASH_MASK = HASH_SIZE - 1;
static const uint32_t WMASK = WSIZE - 1;
static const uint8_t NIL = 0;
char *optarg;
int optind = 1;
int __getopt_initialized;
static char *nextchar;
int opterr = 1;
int optopt = '?';
typedef ush Pos;
typedef unsigned IPos;
ulg window_size = (ulg)2*WSIZE;
long block_start;
static unsigned ins_h;
static const uint8_t H_SHIFT = 17/3;
unsigned int prev_length;
unsigned strstart;
unsigned match_start;
static int eofile;
static unsigned lookahead;
unsigned max_chain_length;
static unsigned int max_lazy_match;
static int compr_level;
unsigned good_match;
static ulg rsync_sum;
static ulg rsync_chunk_end;
static void fill_window(void);
static off_t deflate_fast(void);
typedef void (*sig_type) (int);
static const uint8_t O_BINARY = 0;
static const uint32_t RW_USER = S_IRUSR | S_IWUSR;
static const uint16_t MAX_PATH_LEN = 1024;
uch inbuf[INBUFSIZ + INBUF_EXTRA];
uch outbuf[OUTBUFSIZ + OUTBUF_EXTRA];
ush d_buf[DIST_BUFSIZE];
uch window[2L*WSIZE];
ush prev[1L<<BITS];
static char *posixly_correct;
char *getenv();
int longest_match(IPos cur_match);
int zip(int in, int out);
int file_read(char *buf,  unsigned size);
int unzip(int in, int out);
int check_zipfile(int in);
int unpack(int in, int out);
int unlzh(int in, int out);
void abort_gzip_signal(void);
int ct_tally(int dist, int lc);
off_t flush_block(char *buf, ulg stored_len, int pad, int eof);
int copy(int in, int out);
ulg  updcrc(uch *s, unsigned n);
void clear_bufs(void);
int  fill_inbuf(int eof_ok);
void flush_outbuf(void);
void flush_window(void);
void write_buf(int fd, voidp buf, unsigned cnt);
char *strlwr(char *s);
char *base_name(char *fname);
int xunlink(char *fname);
void error(char *m);
void warning(char *m);
void read_error(void);
void write_error(void);
void display_ratio(off_t num, off_t den, FILE *file);
void fprint_off(FILE *, off_t, int);
voidp xmalloc(unsigned int size);
int yesno (void);
static file_t zfile; 
static unsigned short bi_buf;
static const uint32_t Buf_size = 8 * 2 * sizeof(char);
static int bi_valid;
int (*read_buf) (char *buf, unsigned size);
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
int ifd;
int ofd;
unsigned insize;
unsigned inptr;
static void treat_stdin  (void);
static void treat_file   (char *iname);
static int create_outfile (void);
static int  get_istat    (char *iname, struct stat *sbuf);
static int  make_ofname  (void);
static int name_too_long (char *name, struct stat *statb);
static void shorten_name  (char *name);
static int  get_method   (int in);
static void do_list      (int ifd, int method);
static int  check_ofname (void);
static void copy_stat    (struct stat *ifstat);
static void do_exit      (int exitcode);
int (*work) (int infile, int outfile) = zip;
static void treat_dir    (char *dir);
static void reset_times  (char *name, struct stat *statb);
ulg bb;
unsigned bk;
static ulg crc;
int lbits = 9;
int dbits = 6;
static const uint8_t BMAX = 16;
unsigned hufts;
int nice_match;
int huft_build (unsigned *, unsigned, unsigned, ush *, ush *, struct huft **, int *);
int huft_free (struct huft *);
int decrypt;
char *key;
int pkzip = 0;
int ext_header = 0;
static const uint8_t MAX_BITS = 15;
static const uint8_t LENGTH_CODES = 29;
static const uint16_t LITERALS = 256;
static const uint16_t END_BLOCK = 256;
static const uint16_t L_CODES = 286;
static const uint8_t D_CODES = 30;
static const uint8_t BL_CODES = 19;
static const uint8_t STORED_BLOCK = 0;
static const uint32_t LIT_BUFSIZE = 0x8000;

class App
{
    void usage();
    void help();
    char *add_envopt(int *argcp, char ***argvp, char *env);
public:
    int run(int argc, char **argv);
};

void bi_init (file_t zipfile)
{
    zfile  = zipfile;
    bi_buf = 0;
    bi_valid = 0;

    if (zfile != -1)
	    read_buf = file_read;
}

void put_byte(int c)
{
    outbuf[outcnt++] = (uch)(c);

    if (outcnt == OUTBUFSIZ)
        flush_outbuf();
}

void put_short(int w)
{
    if (outcnt < OUTBUFSIZ - 2)
    {
        outbuf[outcnt++] = (uch)((w) & 0xff);
        outbuf[outcnt++] = (uch)((ush)(w) >> 8);
    }
    else
    {
        put_byte((uch)((w) & 0xff));
        put_byte((uch)((ush)(w) >> 8));
    }
}


void send_bits(int value, int length)
{
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

unsigned bi_reverse(unsigned code, int len)
{
    register unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

void bi_windup()
{
    if (bi_valid > 8) {
        put_short(bi_buf);
    } else if (bi_valid > 0) {
        put_byte(bi_buf);
    }
    bi_buf = 0;
    bi_valid = 0;

}

void copy_block(char *buf, unsigned len, int header)
{
    bi_windup(); 

    if (header) {
        put_short((ush)len);   
        put_short((ush)~len);

    }
    while (len--) {
	put_byte(*buf++);
    }
}

typedef struct config {
   ush good_length; 
   ush max_lazy;    
   ush nice_length; 
   ush max_chain;
} config;



static config configuration_table[10] = {
 {0,    0,  0,    0},  
 {4,    4,  8,    4},  
 {4,    5, 16,    8},
 {4,    6, 32,   32},

 {4,    4, 16,   16},  
 {8,   16, 32,   32},
 {8,   16, 128, 128},
 {8,   32, 128, 256},
 {32, 128, 258, 1024},
 {32, 258, 258, 4096}};

void lm_init(int pack_level, ush *flags)
{
    register unsigned j;
    if (pack_level < 1 || pack_level > 9) error((char *)"bad pack level");
    compr_level = pack_level;
    memset((char*)prev + WSIZE, 0, HASH_SIZE*sizeof(*(prev+WSIZE)));
    rsync_chunk_end = 0xFFFFFFFFUL;
    rsync_sum = 0;
    max_lazy_match   = configuration_table[pack_level].max_lazy;
    good_match       = configuration_table[pack_level].good_length;
    nice_match       = configuration_table[pack_level].nice_length;
    max_chain_length = configuration_table[pack_level].max_chain;
    if (pack_level == 1) {
       *flags |= 4;
    } else if (pack_level == 9) {
       *flags |= 2;
    }
    strstart = 0;
    block_start = 0L;
    lookahead = read_buf((char*)window,
			 sizeof(int) <= 2 ? (unsigned)WSIZE : 2*WSIZE);

    if (lookahead == 0 || lookahead == (unsigned)EOF) {
       eofile = 1, lookahead = 0;
       return;
    }
    eofile = 0;
    while (lookahead < MIN_LOOKAHEAD && !eofile) fill_window();

    ins_h = 0;

    for (j=0; j<MIN_MATCH-1; j++)
        ins_h = (((ins_h)<<H_SHIFT) ^ (window[j])) & HASH_MASK;

}

int longest_match(IPos cur_match)
{
    unsigned chain_length = max_chain_length;  
    register uch *scan = window + strstart;    
    register uch *match;                       
    register int len;                          
    int best_len = prev_length;                
    IPos limit = strstart > (IPos)MAX_DIST ? strstart - (IPos)MAX_DIST : NIL;

    register uch *strend = window + strstart + MAX_MATCH;
    register uch scan_end1  = scan[best_len-1];
    register uch scan_end   = scan[best_len];

    if (prev_length >= good_match) {
        chain_length >>= 2;
    }

    do {
        match = window + cur_match;

        if (match[best_len]   != scan_end  ||
            match[best_len-1] != scan_end1 ||
            *match            != *scan     ||
            *++match          != scan[1])      continue;

       
        scan += 2, match++;

      
        do {
        } while (*++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 scan < strend);

        len = MAX_MATCH - (int)(strend - scan);
        scan = strend - MAX_MATCH;

        if (len > best_len) {
            match_start = cur_match;
            best_len = len;
            if (len >= nice_match) break;

            scan_end1  = scan[best_len-1];
            scan_end   = scan[best_len];
        }
    } while ((cur_match = prev[cur_match & WMASK]) > limit
	     && --chain_length != 0);

    return best_len;
}



static void fill_window()
{
    register unsigned n, m;
    unsigned more = (unsigned)(window_size - (ulg)lookahead - (ulg)strstart);
    
    if (more == (unsigned)EOF) {
        more--;
    } else if (strstart >= WSIZE+MAX_DIST) {

        memcpy((char*)window, (char*)window+WSIZE, (unsigned)WSIZE);
        match_start -= WSIZE;
        strstart    -= WSIZE;
	if (rsync_chunk_end != 0xFFFFFFFFUL)
	    rsync_chunk_end -= WSIZE;

        block_start -= (long) WSIZE;

        for (n = 0; n < HASH_SIZE; n++) {
            m = (prev+WSIZE)[n];
            (prev+WSIZE)[n] = (Pos)(m >= WSIZE ? m-WSIZE : NIL);
        }
        for (n = 0; n < WSIZE; n++) {
            m = prev[n];
            prev[n] = (Pos)(m >= WSIZE ? m-WSIZE : NIL);
        }
        more += WSIZE;
    }
    if (!eofile) {
        n = read_buf((char*)window+strstart+lookahead, more);
        if (n == 0 || n == (unsigned)EOF) {
            eofile = 1;
        } else {
            lookahead += n;
        }
    }
}

static void rsync_roll(unsigned start, unsigned num)
{
    unsigned i;

    if (start < 4096)
    {
        for (i = start; i < 4096; i++)
        {
            if (i == start + num) return;
            rsync_sum += (ulg)window[i];
        }
    	num -= (4096 - start);
        start = 4096;
    }

    for (i = start; i < start+num; i++)
    {
        rsync_sum += (ulg)window[i];
    	rsync_sum -= (ulg)window[i - 4096];

        if (rsync_chunk_end == 0xFFFFFFFFUL && rsync_sum % 4096 == 0)
            rsync_chunk_end = i;
    }
}

void INSERT_STRING2(unsigned &s, IPos &match_head)
{
    ((ins_h = (((ins_h)<<H_SHIFT) ^ (window[(s) + MIN_MATCH - 1])) & HASH_MASK),
    prev[(s) & WMASK] = match_head = (prev+WSIZE)[ins_h], (prev+WSIZE)[ins_h] = (s));
}

static off_t deflate_fast()
{
    IPos hash_head;
    int flush;     
    unsigned match_length = 0; 
    prev_length = MIN_MATCH-1;

    while (lookahead != 0)
    {
        INSERT_STRING2(strstart, hash_head);

        if (hash_head != NIL && strstart - hash_head <= MAX_DIST &&
	    strstart <= window_size - MIN_LOOKAHEAD) {
            match_length = longest_match (hash_head);
            if (match_length > lookahead) match_length = lookahead;
        }
        if (match_length >= MIN_MATCH) {

            flush = ct_tally(strstart-match_start, match_length - MIN_MATCH);

            lookahead -= match_length;

            do { if (rsync) rsync_roll((strstart), (match_length)); } while (0);

            if (match_length <= max_lazy_match) {
                match_length--;
                do {
                    strstart++;
                    INSERT_STRING2(strstart, hash_head);
            
                } while (--match_length != 0);
	        strstart++; 
            } else {
	        strstart += match_length;
	        match_length = 0;
	        ins_h = window[strstart];
            ins_h = (((ins_h) << H_SHIFT) ^ (window[strstart + 1])) & HASH_MASK;
            }
        } else {
            flush = ct_tally (0, window[strstart]);
            do { if (rsync) rsync_roll((strstart), (1)); } while (0);
            lookahead--;
            strstart++; 
        }
	if (rsync && strstart > rsync_chunk_end) {
	    rsync_chunk_end = 0xFFFFFFFFUL;
	    flush = 2;
	} 
        if (flush)
        {
           flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : 
                (char*)NULL, (long)strstart - block_start, flush-1, (0));

            block_start = strstart;
        }

        while (lookahead < MIN_LOOKAHEAD && !eofile) fill_window();

    }

   return flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : 
                (char*)NULL, (long)strstart - block_start, flush-1, (1));

}

off_t deflate()
{
    IPos hash_head;      
    IPos prev_match;     
    int flush;           
    int match_available = 0;
    register unsigned match_length = MIN_MATCH-1;

    if (compr_level <= 3)
        return deflate_fast();

    while (lookahead != 0)
    {
        INSERT_STRING2(strstart, hash_head);
        prev_length = match_length, prev_match = match_start;
        match_length = MIN_MATCH-1;

        if (hash_head != NIL && prev_length < max_lazy_match &&
            strstart - hash_head <= MAX_DIST &&
	        strstart <= window_size - MIN_LOOKAHEAD)
        {
            match_length = longest_match(hash_head);

            if (match_length > lookahead)
                match_length = lookahead;

            if (match_length == MIN_MATCH && strstart-match_start > 4096)
                match_length--;
        }

        if (prev_length >= MIN_MATCH && match_length <= prev_length)
        {
            flush = ct_tally(strstart-1-prev_match, prev_length - MIN_MATCH);
            lookahead -= prev_length-1;
            prev_length -= 2;

            do { if (rsync) rsync_roll((strstart), (prev_length + 1)); } while (0);

            do
            {
                strstart++;
                INSERT_STRING2(strstart, hash_head);
            }
            while (--prev_length != 0);

            match_available = 0;
            match_length = MIN_MATCH-1;
            strstart++;

	        if (rsync && strstart > rsync_chunk_end)
            {
		        rsync_chunk_end = 0xFFFFFFFFUL;
		        flush = 2;
	        }

            if (flush)
            {
                flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : 
                        (char*)NULL, (long)strstart - block_start, flush-1, (0));

                block_start = strstart;
            }
        }
        else if (match_available)
        {
            flush = ct_tally (0, window[strstart-1]);

            if (rsync && strstart > rsync_chunk_end)
            {
                rsync_chunk_end = 0xFFFFFFFFUL;
                flush = 2;
            }

            if (flush)
            {
                flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : 
                        (char*)NULL, (long)strstart - block_start, flush-1, (0));

                block_start = strstart;
            }

            do { if (rsync) rsync_roll((strstart), (1 + 1)); } while (0);

            strstart++;
            lookahead--;
        }
        else
        {
            if (rsync && strstart > rsync_chunk_end)
            {
                rsync_chunk_end = 0xFFFFFFFFUL;
                flush = 2;

                flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : 
                        (char*)NULL, (long)strstart - block_start, flush-1, (0));

                block_start = strstart;
            }

            match_available = 1;
            
            do { if (rsync) rsync_roll((strstart), (1 + 1)); } while (0);
            strstart++;
            lookahead--;
        }

        while (lookahead < MIN_LOOKAHEAD && !eofile) fill_window();
    }

    if (match_available) ct_tally(0, window[strstart-1]);

    return flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : 
                (char*)NULL, (long)strstart - block_start, flush-1, (1));
}

struct option
{
    const char *name;
    int has_arg;
    int *flag;
    int val;
};



static enum
{
  REQUIRE_ORDER, PERMUTE, RETURN_IN_ORDER
} ordering;



static char *my_index(const char *str, int chr)
{
    while (*str)
    {
        if (*str == chr)
	        return (char *) str;
        str++;
    }

    return 0;
}





static void exchange (char **argv)
{
  int bottom = first_nonopt;
  int middle = last_nonopt;
  int top = optind;
  char *tem;
  while (top > middle && middle > bottom)
    {
      if (top - middle > middle - bottom)
	{
	  int len = middle - bottom;
	  register int i;

	  for (i = 0; i < len; i++)
	    {
	      tem = argv[bottom + i];
	      argv[bottom + i] = argv[top - (middle - bottom) + i];
	      argv[top - (middle - bottom) + i] = tem;
	    }
	  top -= len;
	}
      else
	{
	  int len = top - middle;
	  register int i;

	  for (i = 0; i < len; i++)
	    {
	      tem = argv[bottom + i];
	      argv[bottom + i] = argv[middle + i];
	      argv[middle + i] = tem;
	    }
	  bottom += len;
	}
    }

  first_nonopt += (optind - last_nonopt);
  last_nonopt = optind;
}

static const char * _getopt_initialize(int argc, char * const *argv, const char *optstring)
{

  first_nonopt = last_nonopt = optind;

  nextchar = NULL;

  posixly_correct = getenv ("POSIXLY_CORRECT");

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

  return optstring;
}

int _getopt_internal(int argc, char * const *argv, const char *optstring,
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

	 

	  while (optind < argc &&  (argv[optind][0] != '-' || argv[optind][1] == '\0'))
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


      if ((argv[optind][0] != '-' || argv[optind][1] == '\0'))
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
	    fprintf (stderr, "%s: option `%s' is ambiguous\n",
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
				 "%s: option `--%s' doesn't allow an argument\n",
				 argv[0], pfound->name);
		      else
			fprintf (stderr,
				 "%s: option `%c%s' doesn't allow an argument\n",
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
			   "%s: option `%s' requires an argument\n",
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
		fprintf (stderr, "%s: unrecognized option `--%s'\n",
			 argv[0], nextchar);
	      else
		fprintf (stderr, "%s: unrecognized option `%c%s'\n",
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
	      fprintf (stderr, "%s: illegal option -- %c\n",
		       argv[0], c);
	    else
	      fprintf (stderr, "%s: invalid option -- %c\n",
		       argv[0], c);
	  }
	optopt = c;
	return '?';
      }
    if (temp[0] == 'W' && temp[1] == ';')
      {
	char *nameend;
	const struct option *p;
	const struct option *pfound = NULL;
	int exact = 0;
	int ambig = 0;
	int indfound = 0;
	int option_index;

	if (*nextchar != '\0')
	  {
	    optarg = nextchar;
	    optind++;
	  }
	else if (optind == argc)
	  {
	    if (print_errors)
	      {
		fprintf (stderr, "%s: option requires an argument -- %c\n",
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
	  optarg = argv[optind++];


	for (nextchar = nameend = optarg; *nameend && *nameend != '='; nameend++)

	for (p = longopts, option_index = 0; p->name; p++, option_index++)
	  if (!strncmp (p->name, nextchar, nameend - nextchar))
	    {
	      if ((unsigned int) (nameend - nextchar) == strlen (p->name))
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
	      else
		ambig = 1;
	    }
	if (ambig && !exact)
	  {
	    if (print_errors)
	      fprintf (stderr, "%s: option `-W %s' is ambiguous\n",
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
		if (pfound->has_arg)
		  optarg = nameend + 1;
		else
		  {
		    if (print_errors)
		      fprintf (stderr, "\
%s: option `-W %s' doesn't allow an argument\n",
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
			       "%s: option `%s' requires an argument\n",
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
	  return 'W';
      }
    if (temp[1] == ':')
      {
	if (temp[2] == ':')
	  {
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
	    if (*nextchar != '\0')
	      {
		optarg = nextchar;
		optind++;
	      }
	    else if (optind == argc)
	      {
		if (print_errors)
		  {
		    fprintf (stderr,
			     "%s: option requires an argument -- %c\n",
			     argv[0], c);
		  }
		optopt = c;
		if (optstring[0] == ':')
		  c = ':';
		else
		  c = '?';
	      }
	    else
	      optarg = argv[optind++];
	    nextchar = NULL;
	  }
      }
    return c;
  }
}

int getopt(int argc, char * const *argv, const char *optstring)
{
  return _getopt_internal (argc, argv, optstring,
			   (const struct option *) 0,
			   (int *) 0,
			   0);
}

static char  *license_msg[] = {
    (char *)"Copyright 2002 Free Software Foundation",
    (char *)"Copyright 1992-1993 Jean-loup Gailly",
    (char *)"This program comes with ABSOLUTELY NO WARRANTY.",
    (char *)"You may redistribute copies of this program",
    (char *)"under the terms of the GNU General Public License.",
    (char *)"For more information about these matters, see the file named COPYING.", 0};



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



bool strequ(const char *s1, const char *s2)
{
    return strcmp(s1, s2) == 0;
}

static void license()
{
    char **p = license_msg;

    printf ("%s %s\n(%s)\n", progname, "1.3.5", "2002-09-30");
    while (*p) printf ("%s\n", *p++);
}

static void version()
{
    license ();
    printf ("Compilation options:\n%s %s ", "DIRENT", "UTIME");
    printf ("STDC_HEADERS ");
    printf ("HAVE_UNISTD_H ");
    printf ("HAVE_MEMORY_H ");
    printf ("HAVE_STRING_H ");
    printf ("HAVE_LSTAT ");
    printf ("\n");
    printf ("Written by Jean-loup Gailly.\n");
}

static void progerror (char *string)
{
    int e = errno;
    fprintf(stderr, "%s: ", progname);
    errno = e;
    perror(string);
    exit_code = ERROR;
}



void App::usage()
{
    printf ("usage: %s [-%scdfhlLnN%stvV19] [-S suffix] [file ...]\n",
	    progname,
	    O_BINARY ? "a" : "", 0 ? "" : "r");
}

void App::help()
{
    static char  *help_msg[] = {

 (char *)" -c --stdout      write on standard output, keep original files unchanged",
 (char *)" -d --decompress  decompress",
 (char *)" -f --force       force overwrite of output file and compress links",
 (char *)" -h --help        give this help",
 (char *)" -l --list        list compressed file contents",
 (char *)" -L --license     display software license",
 (char *)" -n --no-name     do not save or restore the original name and time stamp",
 (char *)" -N --name        save or restore the original name and time stamp",
 (char *)" -q --quiet       suppress all warnings",
 (char *)" -r --recursive   operate recursively on directories",
 (char *)" -S .suf  --suffix .suf     use suffix .suf on compressed files",
 (char *)" -t --test        test compressed file integrity",
 (char *)" -v --verbose     verbose mode",
 (char *)" -V --version     display version number",
 (char *)" -1 --fast        compress faster",
 (char *)" -9 --best        compress better",
 (char *)"    --rsyncable   Make rsync-friendly archive",
 (char *)" file...          files to (de)compress. If none given, use standard input.",
 (char *)"Report bugs to <bug-gzip@gnu.org>.",
  0};
    char **p = help_msg;

    printf ("%s %s\n(%s)\n", progname, "1.3.5", "2002-09-30");
    usage();
    while (*p) printf ("%s\n", *p++);
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}

char *App::add_envopt(int *argcp, char ***argvp, char *env)
{
    char *p;
    char **oargv;
    char **nargv;
    int	 oargc = *argcp;
    int  nargc = 0;

    env = (char*)getenv(env);
    if (env == NULL) return NULL;

    p = (char*)xmalloc(strlen(env)+1);
    env = strcpy(p, env); 

    for (p = env; *p; nargc++ ) { 
	p += strspn(p, " \t");
	if (*p == '\0') break;

	p += strcspn(p, " \t");
	if (*p) *p++ = '\0';
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
        p += strspn(p, " \t");
        *(nargv++) = p;
        while (*p++) ;
    }

    while (oargc--) *(nargv++) = *(oargv++);
    *nargv = NULL;
    return env;
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

int App::run(int argc, char **argv)
{
    int file_count;
    int proglen;
    int optc;

    progname = base_name (argv[0]);
    proglen = strlen(progname);

    if (proglen > 4 && strequ(progname+proglen-4, ".exe")) {
        progname[proglen-4] = '\0';
    }

    env = add_envopt(&argc, &argv, (char *)"GZIP");
    if (env != NULL) args = argv;

#if 0
    foreground = signal(SIGINT, SIG_IGN) != SIG_IGN;
    if (foreground) {
	(void) signal (SIGINT, (sig_type)abort_gzip_signal);
    }
    if (signal(SIGTERM, SIG_IGN) != SIG_IGN) {
	(void) signal(SIGTERM, (sig_type)abort_gzip_signal);
    }
    if (signal(SIGHUP, SIG_IGN) != SIG_IGN) {
	(void) signal(SIGHUP,  (sig_type)abort_gzip_signal);
    }
#endif

    if (  strncmp(progname, "un",  2) == 0     
       || strncmp(progname, "gun", 3) == 0) {  
	decompress = 1;
    } else if (strequ(progname+1, "cat")       
	    || strequ(progname, "gzcat")) {   
	decompress = to_stdout = 1;
    }

    z_suffix = (char *)".gz";
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
	case 'm':
	    no_time = 1; break;
	case 'M':
	    no_time = 0; break;
	case 'n':
	    no_name = no_time = 1; break;
	case 'N':
	    no_name = no_time = 0; break;
	case 'q':
	    quiet = 1; verbose = 0; break;
	case 'r':
	    recursive = 1; break;
	case 'R':
	    rsync = 1; break;

	case 'S':
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
	    fprintf(stderr, "%s: -Z not supported in this version\n",
		    progname);
	    usage();
	    do_exit(ERROR); break;
	case '1':  case '2':  case '3':  case '4':
	case '5':  case '6':  case '7':  case '8':  case '9':
	    level = optc - '0';
	    break;
	default:
	    usage();
	    do_exit(ERROR);
	}
    }

#if 0
    if (quiet && signal (SIGPIPE, SIG_IGN) != SIG_IGN)
      signal (SIGPIPE, (sig_type) abort_gzip_signal);
#endif

    if (no_time < 0) no_time = decompress;
    if (no_name < 0) no_name = decompress;

    file_count = argc - optind;

    if (ascii && !quiet) {
	fprintf(stderr, "%s: option --ascii ignored on this system\n",
		progname);
    }
    if ((z_len == 0 && !decompress) || z_len > MAX_SUFFIX) {
        fprintf(stderr, "%s: incorrect suffix '%s'\n",
                progname, z_suffix);
        do_exit(ERROR);
    }
    if (do_lzw && !decompress) work = lzw;

    if (file_count != 0) {

        while (optind < argc) {
	    treat_file(argv[optind++]);
	}
    } else { 
	treat_stdin();
    }
    if (list && !quiet && file_count > 1) {
	do_list(-1, -1);
    }
    do_exit(exit_code);
    return exit_code;
}

static int input_eof ()
{
  if (!decompress || last_member)
    return 1;

  if (inptr == insize)
    {
      if (insize != INBUFSIZ || fill_inbuf (1) == EOF)
	return 1;

      inptr = 0;
    }

  return 0;
}

static void treat_stdin()
{
    if (!force && !list &&
	isatty(fileno((FILE *)(decompress ? stdin : stdout)))) {

	fprintf(stderr,
    "%s: compressed data not %s a terminal. Use -f to force %scompression.\n",
		progname, decompress ? "read from" : "written to",
		decompress ? "de" : "");
	fprintf(stderr,"For help, type: %s -h\n", progname);
	do_exit(ERROR);
    }



    strcpy(ifname, "stdin");
    strcpy(ofname, "stdout");

    time_stamp = 0;

    if (list || !no_time) {
	if (fstat(fileno(stdin), &istat) != 0) {
	    progerror((char *)"standard input");
	    do_exit(ERROR);
	}
	    time_stamp = istat.st_mtime;
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

	}
    }
}


static void treat_file(char *iname)
{
    if (strequ(iname, "-")) {
	int cflag = to_stdout;
	treat_stdin();
	to_stdout = cflag;
	return;
    }

    if (get_istat(iname, &istat) != OK) return;

    if (S_ISDIR(istat.st_mode)) {
	if (recursive) {
	    struct stat st;
	    st = istat;
	    treat_dir(iname);
	    reset_times (iname, &st);
	} else
	return;
    }
    if (!S_ISREG(istat.st_mode)) {
	return;
    }
    if (istat.st_nlink > 1 && !to_stdout && !force) {
	return;
    }

    ifile_size = istat.st_size;
    time_stamp = no_time && !list ? 0 : istat.st_mtime;

    if (to_stdout && !list && !test) {
	strcpy(ofname, "stdout");

    } else if (make_ofname() != OK) {
	return;
    }

    ifd = open(ifname, ascii && !decompress ? O_RDONLY : O_RDONLY | O_BINARY,
	       RW_USER);
    if (ifd == -1) {
	progerror(ifname);
	return;
    }
    clear_bufs();
    part_nb = 0;

    if (decompress) {
	method = get_method(ifd);
	if (method < 0) {
	    close(ifd);
	    return;
	}
    }
    if (list) {
        do_list(ifd, method);
        close(ifd);
        return;
    }

    if (to_stdout) {
	ofd = fileno(stdout);
    } else {
	if (create_outfile() != OK) return;

	if (!decompress && save_orig_name && !verbose && !quiet) {
	    fprintf(stderr, "%s: %s compressed to %s\n",
		    progname, ifname, ofname);
	}
    }
    if (!save_orig_name) save_orig_name = !no_name;

    if (verbose) {
	fprintf(stderr, "%s:\t", ifname);
    }

    for (;;) {
	if ((*work)(ifd, ofd) != OK) {
	    method = -1;
	    break;
	}

	if (input_eof ())
	  break;

	method = get_method(ifd);
	if (method < 0) break; 
	bytes_out = 0;         
    }

    close(ifd);
    if (!to_stdout) {
         copy_stat(&istat);
         if (close(ofd))
            write_error();
    }
    if (method == -1) {
	if (!to_stdout) xunlink (ofname);
	return;
    }
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

static int create_outfile()
{
    struct stat	ostat;
    int flags = O_WRONLY | O_CREAT | O_EXCL | O_BINARY;

    if (ascii && decompress) {
	flags &= ~O_BINARY;
    }
    for (;;) {
	if (check_ofname() != OK) {
	    close(ifd);
	    return ERROR;
	}
	remove_ofname = 1;
	ofd = open(ofname, flags, RW_USER);
	if (ofd == -1) {
	    progerror(ofname);
	    close(ifd);
	    return ERROR;
	}

	if (fstat(ofd, &ostat) != 0) {
	    progerror(ofname);
	    close(ifd); close(ofd);
	    xunlink (ofname);
	    return ERROR;
	}
	if (!name_too_long(ofname, &ostat)) return OK;

	if (decompress) {
	    return OK;
	}
	close(ofd);
	xunlink (ofname);
	shorten_name(ofname);
    }
}

static int do_stat(char *name, struct stat *sbuf)
{
    errno = 0;
    if (!to_stdout && !force) {
	return lstat(name, sbuf);
    }
    return stat(name, sbuf);
}

static char *get_suffix(char *name)
{
    int nlen, slen;
    char suffix[MAX_SUFFIX+3];
    static char *known_suffixes[] =
       {NULL, (char *)".gz", (char *)".z", (char *)".taz", (char *)".tgz",
        (char *)"-gz", (char *)"-z", (char *)"_z",
          NULL};
    char **suf = known_suffixes;

    *suf = z_suffix;
    if (strequ(z_suffix, "z")) suf++;
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
       if (slen > s && suffix[slen-s-1] != '/'
           && strequ(suffix + slen - s, *suf)) {
           return name+nlen-s;
       }
    } while (*++suf != NULL);

    return NULL;
}

static int get_istat(char *iname, struct stat *sbuf)
{
    int ilen;
    int z_suffix_errno = 0;
    static char *suffixes[] = {NULL, (char *)".gz", (char *)".z",
            (char *)"-z", (char *)".Z", NULL};
    char **suf = suffixes;
    char *s;
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
    ilen = strlen(ifname);
    if (strequ(z_suffix, ".gz")) suf++;

    do {
        char *s0 = s = *suf;
        strcpy (ifname, iname);
	if (sizeof ifname <= ilen + strlen (s))
	  goto name_too_long;
        strcat(ifname, s);
        if (do_stat(ifname, sbuf) == 0) return OK;
	if (strequ (s0, z_suffix))
	  z_suffix_errno = errno;
    } while (*++suf != NULL);

    strcpy(ifname, iname);
    strcat(ifname, z_suffix);
    errno = z_suffix_errno;
    progerror(ifname);
    return ERROR;
      
 name_too_long:
    fprintf (stderr, "%s: %s: file name too long\n", progname, iname);
    exit_code = ERROR;
    return ERROR;
}


static int make_ofname()
{
    char *suff;

    strcpy(ofname, ifname);
    suff = get_suffix(ofname);

    if (decompress) {
	if (suff == NULL) {
            if (!recursive && (list || test)) return OK;

	    if (verbose || (!recursive && !quiet)) {
	    }
	    return 2;
	}
	strlwr(suff);
	if (strequ(suff, ".tgz") || strequ(suff, ".taz")) {
	    strcpy(suff, ".tar");
	} else {
	    *suff = '\0';
	}

    } else if (suff != NULL) {
	if (verbose || (!recursive && !quiet)) {
	    fprintf(stderr, "%s: %s already has %s suffix -- unchanged\n",
		  progname, ifname, suff);
	}
	return 2;
    } else {
        save_orig_name = 0;
	if (sizeof ofname <= strlen (ofname) + z_len)
	    goto name_too_long;
	strcat(ofname, z_suffix);

    }
    return OK;

 name_too_long:
    return 2;
}

uch get_byte()
{
    return inptr < insize ? inbuf[inptr++] : fill_inbuf(0);
}


static int get_method(int in)
{
    uch flags;     
    char magic[2]; 
    int imagic1;   
    ulg stamp;     

    if (force && to_stdout)
    {
        magic[0] = (char)(inptr < insize ? inbuf[inptr++] : fill_inbuf(1));
        imagic1 = (inptr < insize ? inbuf[inptr++] : fill_inbuf(1));
        magic[1] = (char)imagic1;
    }
    else
    {
        magic[0] = (char)get_byte();
        magic[1] = (char)get_byte();
        imagic1 = 0;
    }

    method = -1;
    part_nb++;  
    header_bytes = 0;
    last_member = 0;

    if (memcmp(magic, GZIP_MAGIC, 2) == 0 || memcmp(magic, OLD_GZIP_MAGIC, 2) == 0)
    {
	    method = (int)get_byte();

        if (method != DEFLATED)
        {
            fprintf(stderr, "%s: %s: unknown method %d -- not supported\n",
                progname, ifname, method);

            exit_code = ERROR;
            return -1;
        }

        work = unzip;
        flags = (uch)get_byte();

        if ((flags & 0x20) != 0)
        {
            fprintf(stderr,
                "%s: %s is encrypted -- not supported\n",
                progname, ifname);

            exit_code = ERROR;
            return -1;
        }

        if ((flags & CONTINUATION) != 0)
        {
            fprintf(stderr,
                "%s: %s is a a multi-part gzip file -- not supported\n",
                progname, ifname);

            exit_code = ERROR;

            if (force <= 1) return -1;
        }

        if ((flags & RESERVED) != 0)
        {
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

        (void)get_byte();
        (void)get_byte();

        if ((flags & CONTINUATION) != 0)
        {
            unsigned part = (unsigned)get_byte();
            part |= ((unsigned)get_byte())<<8;

            if (verbose)
            {
                fprintf(stderr,"%s: %s: part number %u\n", progname, ifname, part);
            }
        }

        if ((flags & 4) != 0)
        {
            unsigned len = (unsigned)get_byte();
            len |= ((unsigned)get_byte())<<8;

            if (verbose)
            {
                fprintf(stderr,"%s: %s: extra field of %u bytes ignored\n",
			        progname, ifname, len);
            }

            while (len--) (void)get_byte();
        }

        if ((flags & ORIG_NAME) != 0)
        {
            if (no_name || (to_stdout && !list) || part_nb > 1)
            {
                char c; 
                do {c=get_byte();} while (c != 0);
            } else {
                char *p = base_name (ofname);
                char *base = p;
                char *base2;

                for (;;) {
                    *p = (char)get_byte();
                    if (*p++ == '\0') break;
                    if (p >= ofname+sizeof(ofname)) {
                        error((char *)"corrupted input -- file name too large");
                    }
                }
                base2 = base_name (base);
                strcpy(base, base2);
                if (!list) {
                    if (base) list=0;
                }
            }
        } 

        if ((flags & 0x10) != 0) {
            while (get_byte() != 0);
        }

        if (part_nb == 1) {
            header_bytes = inptr + 2*sizeof(long);
        }

    }
    else if (memcmp(magic, PKZIP_MAGIC, 2) == 0 && inptr == 2
            && memcmp((char*)inbuf, PKZIP_MAGIC, 4) == 0)
    {
        inptr = 0;
        work = unzip;
        if (check_zipfile(in) != OK) return -1;
        last_member = 1;
    } else if (memcmp(magic, PACK_MAGIC, 2) == 0) {
    work = unpack;
    method = 2;

    } else if (memcmp(magic, LZW_MAGIC, 2) == 0) {
    work = unlzw;
    method = 1;
    last_member = 1;

    } else if (memcmp(magic, LZH_MAGIC, 2) == 0) {
    work = unlzh;
    method = 3;
    last_member = 1;

    } else if (force && to_stdout && !list) {
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
    }
    else
    {
        if (magic[0] == 0)
        {
            int inbyte;

            for (inbyte = imagic1;
                inbyte == 0;
                inbyte = (inptr < insize ? inbuf[inptr++] : fill_inbuf(1)))
            {
                continue;
            }

            if (inbyte == EOF)
            {
    	        if (verbose)
    	        return -3;
            }
        }

        return -2;
    }
}

ush SH(uch *p)
{
    return ((ush)(uch)((p)[0]) | ((ush)(uch)((p)[1]) << 8));
}

static void do_list(int ifd, int method)
{
    ulg crc;
    static int first_time = 1;

    static char* methods[9] = {
        (char *)"store",
        (char *)"compr",
        (char *)"pack ",
        (char *)"lzh  ",
        (char *)"", (char *)"", (char *)"", (char *)"",
        (char *)"defla"};

    char *date;
    int positive_off_t_width = 1;
    off_t o;

    for (o = (~ (off_t) 0 - (~ (off_t) 0 << (sizeof (off_t) * CHAR_BIT - 1)));  9 < o;  o /= 10)
    {
	    positive_off_t_width++;
    }

    if (first_time && method >= 0)
    {
        first_time = 0;

        if (verbose)
        {
            printf("method  crc     date  time  ");
        }

        if (!quiet)
        {
            printf("%*.*s %*.*s  ratio uncompressed_name\n",
                positive_off_t_width, positive_off_t_width, "compressed",
                positive_off_t_width, positive_off_t_width, "uncompressed");
	    }
    }
    else if (method < 0)
    {
        if (total_in <= 0 || total_out <= 0) return;

        if (verbose)
        {
            printf("                            ");
        }

        if (verbose || !quiet)
        {
            fprint_off(stdout, total_in, positive_off_t_width);
            printf(" ");
            fprint_off(stdout, total_out, positive_off_t_width);
            printf(" ");
        }

        display_ratio(total_out-(total_in-header_bytes), total_out, stdout);
        printf(" (totals)\n");
        return;
    }

    crc = (ulg)~0;
    bytes_out = -1L;
    bytes_in = ifile_size;

    if (method == DEFLATED && !last_member)
    {
        bytes_in = lseek(ifd, (off_t)(-8), SEEK_END);

        if (bytes_in != -1L)
        {
            uch buf[8];
            bytes_in += 8L;

            if (read(ifd, (char*)buf, sizeof(buf)) != sizeof(buf))
            {
                read_error();
            }

            crc = ((ulg)(SH(buf)) | ((ulg)(SH((buf)+2)) << 16));
            bytes_out = ((ulg)(SH(buf + 4)) | ((ulg)(SH((buf + 4)+2)) << 16));
        }
    }

    date = ctime((time_t*)&time_stamp) + 4;
    date[12] = '\0';
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

static int same_file(struct stat *stat1, struct stat *stat2)
{
    return stat1->st_ino   == stat2->st_ino
	&& stat1->st_dev   == stat2->st_dev
	&& stat1->st_mode  == stat2->st_mode
	&& stat1->st_uid   == stat2->st_uid
	&& stat1->st_gid   == stat2->st_gid
	&& stat1->st_size  == stat2->st_size
	&& stat1->st_atime == stat2->st_atime
	&& stat1->st_mtime == stat2->st_mtime
	&& stat1->st_ctime == stat2->st_ctime;
}


static int name_too_long(char *name, struct stat *statb)
{
    int s = strlen(name);
    char c = name[s-1];
    struct stat	tstat;
    int res;

    tstat = *statb;
    name[s-1] = '\0';
    res = lstat(name, &tstat) == 0 && same_file(statb, &tstat);
    name[s-1] = c;
    return res;
}

static void shorten_name(char *name)
{
    int len;                 
    char *trunc = NULL;      
    int plen;                
    int min_part = 3; 
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
	p = strrchr(name, '/');
	p = p ? p+1 : name;
	while (*p) {
	    plen = strcspn(p, "-");
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
	trunc = strrchr(name, '-');
	if (trunc == NULL) error((char *)"internal error in shorten_name");
	if (trunc[1] == '\0') trunc--;
    }
    strcpy(trunc, z_suffix);
}

static int check_ofname()
{
    struct stat	ostat;
    errno = 0;
    while (lstat(ofname, &ostat) != 0) {
        if (errno != ENAMETOOLONG) return 0;
	shorten_name(ofname);
    }
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
	    if (exit_code == OK) exit_code = 2;
	    return ERROR;
	}
    }
    if (xunlink (ofname)) {
	progerror(ofname);
	return ERROR;
    }
    return OK;
}


static void reset_times (char *name, struct stat *statb)
{
    struct utimbuf	timep;
    timep.actime  = statb->st_atime;
    timep.modtime = statb->st_mtime;
    if (utime(name, &timep) && !S_ISDIR(statb->st_mode)) {
	int e = errno;

	if (!quiet) {
	    errno = e;
	    perror(ofname);
	}
    }
}

static void copy_stat(struct stat *ifstat)
{
    if (decompress && time_stamp != 0 && ifstat->st_mtime != time_stamp) {
	ifstat->st_mtime = time_stamp;
	if (verbose > 1) {
	    fprintf(stderr, "%s: time stamp restored\n", ofname);
	}
    }
    reset_times(ofname, ifstat);
    if (fchmod(ofd, ifstat->st_mode & 07777)) {
	int e = errno;

	if (!quiet) {
	    errno = e;
	    perror(ofname);
	}
    }
    fchown(ofd, ifstat->st_uid, ifstat->st_gid);
    remove_ofname = 0;
    if (xunlink (ifname)) {
	int e = errno;

	if (!quiet) {
	    errno = e;
	    perror(ifname);
	}
    }
}

static void treat_dir(char *dir)
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
	if (len + strlen(dp->d_name) + 1 < MAX_PATH_LEN - 1) {
	    strcpy(nbuf,dir);
	    if (len != 0) {
		nbuf[len++] = '/';
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
    if (closedir(dirp) != 0)
	progerror(dir);
}

static void do_exit(int exitcode)
{
    static int in_exit = 0;

    if (in_exit) exit(exitcode);
    in_exit = 1;
    if (env != NULL)  free(env),  env  = NULL;
    if (args != NULL) free((char*)args), args = NULL;
    exit(exitcode);
}

static void do_remove() {
   if (remove_ofname) {
       close(ofd);
       xunlink (ofname);
   }
}

void abort_gzip()
{
	do_remove();
	do_exit(ERROR);
}

void abort_gzip_signal()
{
	do_remove();
	_exit(ERROR);
}

struct huft {
    uch e;
    uch b;
  union {
    ush n;          
    struct huft *t; 
  } v;
};





static unsigned border[] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static ush cplens[] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
static ush cplext[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99};
static ush cpdist[] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577};
static ush cpdext[] = {  
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
        12, 12, 13, 13};





ush mask_bits[] = {
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
    0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};






int huft_build(unsigned *b, unsigned n, unsigned s, ush *d, ush *e, struct huft **t, int *m)
{
  unsigned a;
  unsigned c[BMAX+1];        
  unsigned f;                
  int g;                     
  int h;                     
  register unsigned i;       
  register unsigned j;       
  register int k;            
  int l;                     
  register unsigned *p;      
  register struct huft *q;   
  struct huft r;             
  struct huft *u[BMAX];      
  unsigned v[288];         
  register int w;            
  unsigned x[BMAX+1];        
  unsigned *xp;              
  int y;                     
  unsigned z;                

    memset(c, 0, sizeof(c));
  p = b;  i = n;
  do {
    c[*p]++;            
    p++;                   
  } while (--i);
  if (c[0] == n)           
  {
    *t = (struct huft *)NULL;
    *m = 0;
    return 0;
  }

  l = *m;
  for (j = 1; j <= BMAX; j++)
    if (c[j])
      break;
  k = j;  
  if ((unsigned)l < j)
    l = j;
  for (i = BMAX; i; i--)
    if (c[i])
      break;
  g = i;   
  if ((unsigned)l > i)
    l = i;
  *m = l;

  for (y = 1 << j; j < i; j++, y <<= 1)
    if ((y -= c[j]) < 0)
      return 2;    
  if ((y -= c[i]) < 0)
    return 2;
  c[i] += y;

  x[1] = j = 0;
  p = c + 1;  xp = x + 2;
  while (--i) {
    *xp++ = (j += *p++);
  }

  p = b;  i = 0;
  do {
    if ((j = *p++) != 0)
      v[x[j]++] = i;
  } while (++i < n);
  n = x[g];

  x[0] = i = 0;
  p = v;                        
  h = -1;                       
  w = -l;                       
  u[0] = (struct huft *)NULL;   
  q = (struct huft *)NULL;      
  z = 0;                        

  for (; k <= g; k++)
  {
    a = c[k];
    while (a--)
    {
      while (k > w + l)
      {
        h++;
        w += l;

        z = (z = g - w) > (unsigned)l ? l : z; 
        if ((f = 1 << (j = k - w)) > a + 1)    
        {                 
          f -= a + 1;     
          xp = c + k;
	  if (j < z)
	    while (++j < z)   
	    {
	      if ((f <<= 1) <= *++xp)
		break;           
	      f -= *xp;      
	    }
        }
        z = 1 << j;      

        if ((q = (struct huft *)malloc((z + 1)*sizeof(struct huft))) ==
            (struct huft *)NULL)
        {
          if (h)
            huft_free(u[0]);
          return 3; 
        }
        hufts += z + 1;
        *t = q + 1;   
        *(t = &(q->v.t)) = (struct huft *)NULL;
        u[h] = ++q;  

        if (h)
        {
          x[h] = i;            
          r.b = (uch)l;        
          r.e = (uch)(16 + j); 
          r.v.t = q;           
          j = i >> (w - l);    
          u[h-1][j] = r;       
        }
      }

      r.b = (uch)(k - w);
      if (p >= v + n)
        r.e = 99; 
      else if (*p < s)
      {
        r.e = (uch)(*p < 256 ? 16 : 15);
        r.v.n = (ush)(*p);           
	p++;
      }
      else
      {
        r.e = (uch)e[*p - s];
        r.v.n = d[*p++ - s];
      }

      f = 1 << (k - w);
      for (j = i >> w; j < z; j += f)
        q[j] = r;
      for (j = 1 << (k - 1); i & j; j >>= 1)
        i ^= j;
      i ^= j;

      while ((i & ((1 << w) - 1)) != x[h])
      {
        h--;     
        w -= l;
      }
    }
  }

  return y != 0 && g != 1;
}



int huft_free(struct huft *t)
{
  register struct huft *p, *q;
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
    register unsigned e;
    unsigned n, d;
    unsigned w;
    struct huft *t;
    unsigned ml, md;
    register ulg b;
    register unsigned k;
    b = bb;
    k = bk;
    w = outcnt;
    ml = mask_bits[bl];
    md = mask_bits[bd];

    for (;;)                
    {
        {while(k<((unsigned)bl)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

        if ((e = (t = tl + ((unsigned)b & ml))->e) > 16)
        {
            do
            {
                if (e == 99)
                    return 1;

                {b>>=(t->b);k-=(t->b);}
                e -= 16;

                {while(k<(e)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                    (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}
            }
            while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
        }

        {b>>=(t->b);k-=(t->b);}

        if (e == 16)
        {
            window[w++] = (uch)t->v.n;

            if (w == WSIZE)
            {
                (outcnt=(w),flush_window());
                w = 0;
            }
        }
        else 
        {
            if (e == 15)
                break;

            {while(k<(e)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                    (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

            n = t->v.n + ((unsigned)b & mask_bits[e]);
            {b>>=(e);k-=(e);}

            {while(k<((unsigned)bd)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

            if ((e = (t = td + ((unsigned)b & md))->e) > 16)
            {
                do
                {
                    if (e == 99)
                        return 1;

                    {b>>=(t->b);k-=(t->b);}
                    e -= 16;

                    {while(k<(e)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}
                }
                while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
            }

            {b>>=(t->b);k-=(t->b);}

            {while(k<(e)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

            d = w - t->v.n - ((unsigned)b & mask_bits[e]);
            {b>>=(e);k-=(e);}

            do
            {
                n -= (e = (e = WSIZE - ((d &= WSIZE-1) > w ? d : w)) > n ? n : e);
                if (w - d >= e) 
                {
                    memcpy(window + w, window + d, e);
                    w += e;
                    d += e;
                }
                else
                {
                    do
                    {
                        window[w++] = window[d++];
                    }
                    while (--e);
                }

                if (w == WSIZE)
                {
                    (outcnt=(w),flush_window());
                    w = 0;
                }
            }
            while (n);
        }
    }

    outcnt = w;                   
    bb = b;                   
    bk = k;
    return 0;
}



int inflate_stored()
{
    unsigned n;
    unsigned w;
    register ulg b;
    register unsigned k;
    b = bb;
    k = bk;
    w = outcnt;
    n = k & 7;
    {b>>=(n);k-=(n);}

    {while(k<(16)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    n = ((unsigned)b & 0xffff);
    {b>>=(16);k-=(16);}

    {while(k<(16)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    if (n != (unsigned)((~b) & 0xffff))
        return 1;

    {b>>=(16);k-=(16);}

    while (n--)
    {
        {while(k<(8)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

        window[w++] = (uch)b;

        if (w == WSIZE)
        {
            (outcnt=(w),flush_window());
            w = 0;
        }

        {b>>=(8);k-=(8);}
    }

    outcnt = w;        
    bb = b;        
    bk = k;
    return 0;
}



int inflate_fixed()
{
  int i;                
  struct huft *tl;      
  struct huft *td;      
  int bl;               
  int bd;               
  unsigned l[288];      

  for (i = 0; i < 144; i++)
    l[i] = 8;
  for (; i < 256; i++)
    l[i] = 9;
  for (; i < 280; i++)
    l[i] = 7;
  for (; i < 288; i++)     
    l[i] = 8;
  bl = 7;
  if ((i = huft_build(l, 288, 257, cplens, cplext, &tl, &bl)) != 0)
    return i;

  for (i = 0; i < 30; i++)  
    l[i] = 5;
  bd = 5;
  if ((i = huft_build(l, 30, 0, cpdist, cpdext, &td, &bd)) > 1)
  {
    huft_free(tl);
    return i;
  }

  if (inflate_codes(tl, td, bl, bd))
    return 1;

  huft_free(tl);
  huft_free(td);
  return 0;
}



int inflate_dynamic()
{
    int i;
    unsigned j;
    unsigned l;
    unsigned m;
    unsigned n;
    unsigned w;
    struct huft *tl;
    struct huft *td;
    int bl;
    int bd;
    unsigned nb;
    unsigned nl;
    unsigned nd;
    unsigned ll[286+30];
    register ulg b;
    register unsigned k;
    b = bb;
    k = bk;
    w = outcnt;

    {while(k<(5)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    nl = 257 + ((unsigned)b & 0x1f);   
    {b>>=(5);k-=(5);}

    {while(k<(5)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    nd = 1 + ((unsigned)b & 0x1f);     
    {b>>=(5);k-=(5);}

    {while(k<(4)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    nb = 4 + ((unsigned)b & 0xf);      
    {b>>=(4);k-=(4);}

    if (nl > 286 || nd > 30)
        return 1;


    for (j = 0; j < nb; j++)
    {
        {while(k<(3)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
            (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

        ll[border[j]] = (unsigned)b & 7;
        {b>>=(3);k-=(3);}
    }

    for (; j < 19; j++)
        ll[border[j]] = 0;

    bl = 7;

    if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0)
    {
        if (i == 1)
            huft_free(tl);

        return i;    
    }

    if (tl == NULL)
	    return 2;

    n = nl + nd;
    m = mask_bits[bl];
    i = l = 0;

    while ((unsigned)i < n)
    {
        {while(k<((unsigned)bl)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

        j = (td = tl + ((unsigned)b & m))->b;
        {b>>=(j);k-=(j);}
        j = td->v.n;

        if (j < 16)         
            ll[i++] = l = j;
        else if (j == 16)
        {
            {while(k<(2)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

            j = 3 + ((unsigned)b & 3);
            {b>>=(2);k-=(2);}

            if ((unsigned)i + j > n)
                return 1;

            while (j--)
                ll[i++] = l;
        }
        else if (j == 17) 
        {
            {while(k<(3)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

            j = 3 + ((unsigned)b & 7);
            {b>>=(3);k-=(3);}

            if ((unsigned)i + j > n)
                return 1;

            while (j--)
                ll[i++] = 0;

            l = 0;
        }
        else   
        {
            {while(k<(7)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

            j = 11 + ((unsigned)b & 0x7f);
            {b>>=(7);k-=(7);}

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
        if (i == 1)
        {
            fprintf(stderr, " incomplete literal tree\n");
            huft_free(tl);
        }

        return i;
    }

    bd = dbits;

    if ((i = huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0)
    {
        if (i == 1)
        {
            fprintf(stderr, " incomplete distance tree\n");
            huft_free(td);
        }

        huft_free(tl);
        return i;
    }

    if (inflate_codes(tl, td, bl, bd))
        return 1;

    huft_free(tl);
    huft_free(td);
    return 0;
}

int inflate_block(int *e)
{
    unsigned t;
    unsigned w;
    register ulg b;
    register unsigned k;
    b = bb;
    k = bk;
    w = outcnt;

    {while(k<(1)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    *e = (int)b & 1;
    {b>>=(1);k-=(1);}

    {while(k<(2)){b|=((ulg)(uch)(inptr < insize ? inbuf[inptr++] : 
                (outcnt = w, fill_inbuf(0))))<<k;k+=8;}}

    t = (unsigned)b & 3;
    {b>>=(2);k-=(2);}
    bb = b;
    bk = k;

    if (t == 2)
        return inflate_dynamic();
    if (t == 0)
        return inflate_stored();
    if (t == 1)
        return inflate_fixed();

    return 2;
}



int inflate()
{
  int e;     
  int r;     
  unsigned h;

  outcnt = 0;
  bk = 0;
  bb = 0;

  h = 0;
  do {
    hufts = 0;
    if ((r = inflate_block(&e)) != 0)
      return r;
    if (hufts > h)
      h = hufts;
  } while (!e);

  while (bk >= 8) {
    bk -= 8;
    inptr--;
  }

(outcnt=(outcnt),flush_window());
  return 0;
}

static int msg_done = 0;

int lzw(int in, int out)
{
    if (msg_done) return ERROR;
    msg_done = 1;
    fprintf(stderr,"output in compress .Z format not supported\n");
    if (in != out) { 
        exit_code = ERROR;
    }
    return ERROR;
}



static int extra_lbits[29] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
static int extra_dbits[30] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
static int extra_blbits[19] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};



struct ct_data
{
    union {
        ush  freq;
        ush  code;
    } fc;
    union {
        ush  dad; 
        ush  len; 
    } dl;
};

static const uint16_t HEAP_SIZE = 573;
static ct_data dyn_ltree[HEAP_SIZE];   
static ct_data dyn_dtree[2*D_CODES+1]; 

static ct_data static_ltree[L_CODES+2];

static ct_data static_dtree[D_CODES];

static ct_data bl_tree[2*BL_CODES+1];
typedef struct tree_desc {
    ct_data *dyn_tree;     
    ct_data *static_tree;  
    int     *extra_bits;   
    int     extra_base;         
    int     elems;              
    int     max_length;         
    int     max_code;           
} tree_desc;



static tree_desc l_desc = {dyn_ltree, static_ltree, extra_lbits, LITERALS+1, L_CODES, MAX_BITS, 0};
static tree_desc d_desc = {dyn_dtree, static_dtree, extra_dbits, 0, D_CODES, MAX_BITS, 0};
static tree_desc bl_desc = {bl_tree, (ct_data *)0, extra_blbits, 0,      BL_CODES, 7, 0};
static ush bl_count[MAX_BITS+1];
static uch bl_order[BL_CODES] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
static int heap[2*L_CODES+1]; 
static int heap_len;               
static int heap_max;               
static uch depth[2*L_CODES+1];
static uch length_code[MAX_MATCH-MIN_MATCH+1];
static uch dist_code[512];
static int base_length[LENGTH_CODES];
static int base_dist[D_CODES];
static uch flag_buf[(LIT_BUFSIZE/8)];
static unsigned last_lit;   
static unsigned last_dist;  
static unsigned last_flags; 
static uch flags;           
static uch flag_bit;        
static ulg opt_len;    
static ulg static_len; 
static off_t compressed_len; 
static off_t input_len;
ush *file_type;        
int *file_method;      
static void init_block     (void);
static void pqdownheap     (ct_data *tree, int k);
static void gen_bitlen     (tree_desc *desc);
static void gen_codes      (ct_data *tree, int max_code);
static void build_tree_1     (tree_desc *desc);
static void scan_tree      (ct_data *tree, int max_code);
static void send_tree      (ct_data *tree, int max_code);
static int  build_bl_tree  (void);
static void send_all_trees (int lcodes, int dcodes, int blcodes);
static void compress_block (ct_data *ltree, ct_data *dtree);
static void set_file_type  (void);



void ct_init(ush *attr, int *methodp)
{
    int n;      
    int bits;   
    int length; 
    int code;   
    int dist;   

    file_type = attr;
    file_method = methodp;
    compressed_len = input_len = 0L;
        
    if (static_dtree[0].dl.len != 0) return;
    length = 0;
    for (code = 0; code < LENGTH_CODES-1; code++) {
        base_length[code] = length;
        for (n = 0; n < (1<<extra_lbits[code]); n++) {
            length_code[length++] = (uch)code;
        }
    }
    length_code[length-1] = (uch)code;

    dist = 0;
    for (code = 0 ; code < 16; code++) {
        base_dist[code] = dist;
        for (n = 0; n < (1<<extra_dbits[code]); n++) {
            dist_code[dist++] = (uch)code;
        }
    }
    dist >>= 7;
    for ( ; code < D_CODES; code++) {
        base_dist[code] = dist << 7;
        for (n = 0; n < (1<<(extra_dbits[code]-7)); n++) {
            dist_code[256 + dist++] = (uch)code;
        }
    }

    for (bits = 0; bits <= MAX_BITS; bits++) bl_count[bits] = 0;
    n = 0;
    while (n <= 143) static_ltree[n++].dl.len = 8, bl_count[8]++;
    while (n <= 255) static_ltree[n++].dl.len = 9, bl_count[9]++;
    while (n <= 279) static_ltree[n++].dl.len = 7, bl_count[7]++;
    while (n <= 287) static_ltree[n++].dl.len = 8, bl_count[8]++;
    gen_codes((ct_data *)static_ltree, L_CODES+1);

    for (n = 0; n < D_CODES; n++)
    {
        static_dtree[n].dl.len = 5;
        static_dtree[n].fc.code = bi_reverse(n, 5);
    }

    init_block();
}
static void init_block()
{
    int n;
    for (n = 0; n < L_CODES;  n++) dyn_ltree[n].fc.freq = 0;
    for (n = 0; n < D_CODES;  n++) dyn_dtree[n].fc.freq = 0;
    for (n = 0; n < BL_CODES; n++) bl_tree[n].fc.freq = 0;
    dyn_ltree[END_BLOCK].fc.freq = 1;
    opt_len = static_len = 0L;
    last_lit = last_dist = last_flags = 0;
    flags = 0; flag_bit = 1;
}

template <typename A, typename B, typename C> bool smaller2(A tree, B n, C m)
{
    return tree[n].fc.freq < tree[m].fc.freq ||
        (tree[n].fc.freq == tree[m].fc.freq && depth[n] <= depth[m]);
}

static void pqdownheap(ct_data *tree, int k)
{
    int v = heap[k];
    int j = k << 1;

    while (j <= heap_len)
    {
        if (j < heap_len && smaller2(tree, heap[j+1], heap[j])) j++;
        if (smaller2(tree, v, heap[j])) break;
        heap[k] = heap[j];  k = j;
        j <<= 1;
    }

    heap[k] = v;
}

static void gen_bitlen(tree_desc *desc)
{
    ct_data *tree  = desc->dyn_tree;
    int *extra     = desc->extra_bits;
    int base            = desc->extra_base;
    int max_code        = desc->max_code;
    int max_length      = desc->max_length;
    ct_data *stree = desc->static_tree;
    int h;             
    int n, m;          
    int bits;          
    int xbits;         
    ush f;             
    int overflow = 0;  

    for (bits = 0; bits <= MAX_BITS; bits++) bl_count[bits] = 0;


    tree[heap[heap_max]].dl.len = 0;

    for (h = heap_max+1; h < HEAP_SIZE; h++) {
        n = heap[h];
        bits = tree[tree[n].dl.dad].dl.len + 1;
        if (bits > max_length) bits = max_length, overflow++;
        tree[n].dl.len = (ush)bits;
        if (n > max_code) continue;

        bl_count[bits]++;
        xbits = 0;
        if (n >= base) xbits = extra[n-base];
        f = tree[n].fc.freq;
        opt_len += (ulg)f * (bits + xbits);
        if (stree) static_len += (ulg)f * (stree[n].dl.len + xbits);
    }
    if (overflow == 0) return;

    do {
        bits = max_length-1;
        while (bl_count[bits] == 0) bits--;
        bl_count[bits]--;      
        bl_count[bits+1] += 2; 
        bl_count[max_length]--;
    
        overflow -= 2;
    } while (overflow > 0);

   
    for (bits = max_length; bits != 0; bits--) {
        n = bl_count[bits];
        while (n != 0) {
            m = heap[--h];
            if (m > max_code) continue;
            if (tree[m].dl.len != (unsigned) bits) {
                opt_len += ((long)bits-(long)tree[m].dl.len)*(long)tree[m].fc.freq;
                tree[m].dl.len = (ush)bits;
            }
            n--;
        }
    }
}

static void gen_codes (ct_data *tree, int max_code)
{
    ush next_code[MAX_BITS+1];
    ush code = 0;             
    int bits;                 
    int n;                    

    for (bits = 1; bits <= MAX_BITS; bits++) {
        next_code[bits] = code = (code + bl_count[bits-1]) << 1;
    }

    for (n = 0;  n <= max_code; n++) {
        int len = tree[n].dl.len;
        if (len == 0) continue;
        tree[n].fc.code = bi_reverse(next_code[len]++, len);

    }
}

uch MAX2(uch a, uch b)
{
    return a >= b ? a : b;
}

static void build_tree_1(tree_desc *desc)
{
    ct_data *tree   = desc->dyn_tree;
    ct_data *stree  = desc->static_tree;
    int elems            = desc->elems;
    int n, m;          
    int max_code = -1; 
    int node = elems;  
    heap_len = 0, heap_max = HEAP_SIZE;

    for (n = 0; n < elems; n++) {
        if (tree[n].fc.freq != 0) {
            heap[++heap_len] = max_code = n;
            depth[n] = 0;
        } else {
            tree[n].dl.len = 0;
        }
    }

    while (heap_len < 2) {
        int new2 = heap[++heap_len] = (max_code < 2 ? ++max_code : 0);
        tree[new2].fc.freq = 1;
        depth[new2] = 0;
        opt_len--; if (stree) static_len -= stree[new2].dl.len;
    }
    desc->max_code = max_code;

    for (n = heap_len/2; n >= 1; n--) pqdownheap(tree, n);

    do {
        n = heap[1];
        heap[1] = heap[heap_len--];
        pqdownheap(tree, 1);
        m = heap[1];  
        heap[--heap_max] = n;
        heap[--heap_max] = m;
        tree[node].fc.freq = tree[n].fc.freq + tree[m].fc.freq;
        depth[node] = (uch) (MAX2(depth[n], depth[m]) + 1);
        tree[n].dl.dad = tree[m].dl.dad = (ush)node;
        heap[1] = node++;
        pqdownheap(tree, 1);

    } while (heap_len >= 2);

    heap[--heap_max] = heap[1];

    gen_bitlen((tree_desc *)desc);
    gen_codes ((ct_data *)tree, max_code);
}


static void scan_tree (ct_data *tree, int max_code)
{
    int n;                     
    int prevlen = -1;          
    int curlen;                
    int nextlen = tree[0].dl.len; 
    int count = 0;             
    int max_count = 7;         
    int min_count = 4;         

    if (nextlen == 0) max_count = 138, min_count = 3;
    tree[max_code+1].dl.len = (ush)0xffff;

    for (n = 0; n <= max_code; n++) {
        curlen = nextlen; nextlen = tree[n+1].dl.len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            bl_tree[curlen].fc.freq += count;
        } else if (curlen != 0) {
            if (curlen != prevlen) bl_tree[curlen].fc.freq++;
            bl_tree[16].fc.freq++;
        } else if (count <= 10) {
            bl_tree[17].fc.freq++;
        } else {
            bl_tree[18].fc.freq++;
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



static void send_tree (ct_data *tree, int max_code)
{
    int n;
    int prevlen = -1;
    int curlen;
    int nextlen = tree[0].dl.len;
    int count = 0;
    int max_count = 7;
    int min_count = 4;
    if (nextlen == 0) max_count = 138, min_count = 3;

    for (n = 0; n <= max_code; n++)
    {
        curlen = nextlen; nextlen = tree[n+1].dl.len;

        if (++count < max_count && curlen == nextlen)
        {
            continue;
        }
        else if (count < min_count)
        {
            do
            {
                send_bits(bl_tree[curlen].fc.code, bl_tree[curlen].dl.len);
            }
            while (--count != 0);
        }
        else if (curlen != 0)
        {
            if (curlen != prevlen)
            {
                send_bits(bl_tree[curlen].fc.code, bl_tree[curlen].dl.len);
                count--;
            }

            send_bits(bl_tree[16].fc.code, bl_tree[16].dl.len);
            send_bits(count-3, 2);
        }
        else if (count <= 10)
        {
            send_bits(bl_tree[17].fc.code, bl_tree[17].dl.len);
            send_bits(count-3, 3);
        }
        else
        {
            send_bits(bl_tree[18].fc.code, bl_tree[18].dl.len);
            send_bits(count-11, 7);
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



static int build_bl_tree()
{
    int max_blindex;
    scan_tree((ct_data *)dyn_ltree, l_desc.max_code);
    scan_tree((ct_data *)dyn_dtree, d_desc.max_code);
    build_tree_1((tree_desc *)(&bl_desc));
    
    for (max_blindex = BL_CODES-1; max_blindex >= 3; max_blindex--) {
        if (bl_tree[bl_order[max_blindex]].dl.len != 0) break;
    }
    opt_len += 3*(max_blindex+1) + 5+5+4;

    return max_blindex;
}


static void send_all_trees(int lcodes, int dcodes, int blcodes)
{
    int rank;

    send_bits(lcodes-257, 5);
    send_bits(dcodes-1,   5);
    send_bits(blcodes-4,  4);
    for (rank = 0; rank < blcodes; rank++) {
        send_bits(bl_tree[bl_order[rank]].dl.len, 3);
    }

    send_tree((ct_data *)dyn_ltree, lcodes-1);
    send_tree((ct_data *)dyn_dtree, dcodes-1);
}


off_t flush_block(char *buf, ulg stored_len, int pad, int eof)
{
    ulg opt_lenb, static_lenb;
    int max_blindex;

    flag_buf[last_flags] = flags;
    if (*file_type == (ush)0xffff) set_file_type();
    build_tree_1((tree_desc *)(&l_desc));
    build_tree_1((tree_desc *)(&d_desc));
    max_blindex = build_bl_tree();

    opt_lenb = (opt_len+3+7)>>3;
    static_lenb = (static_len+3+7)>>3;
    input_len += stored_len;


    if (static_lenb <= opt_lenb) opt_lenb = static_lenb;


    if (stored_len <= opt_lenb && eof && compressed_len == 0L && 0) {
        if (buf == (char*)0) error ((char *)"block vanished");

        copy_block(buf, (unsigned)stored_len, 0);
        compressed_len = stored_len << 3;
        *file_method = STORED;

    } else if (stored_len+4 <= opt_lenb && buf != (char*)0) {
        send_bits((STORED_BLOCK<<1)+eof, 3);
        compressed_len = (compressed_len + 3 + 7) & ~7L;
        compressed_len += (stored_len + 4) << 3;

        copy_block(buf, (unsigned)stored_len, 1);

    } else if (static_lenb == opt_lenb) {
        send_bits((1<<1)+eof, 3);
        compress_block((ct_data *)static_ltree, (ct_data *)static_dtree);
        compressed_len += 3 + static_len;
    } else {
        send_bits((2<<1)+eof, 3);
        send_all_trees(l_desc.max_code+1, d_desc.max_code+1, max_blindex+1);
        compress_block((ct_data *)dyn_ltree, (ct_data *)dyn_dtree);
        compressed_len += 3 + opt_len;
    }
    init_block();

    if (eof) {
        bi_windup();
        compressed_len += 7; 
    } else if (pad && (compressed_len % 8) != 0) {
        send_bits((STORED_BLOCK<<1)+eof, 3); 
        compressed_len = (compressed_len + 3 + 7) & ~7L;
        copy_block(buf, 0, 1); 
    }

    return compressed_len >> 3;
}

int d_code(int dist)
{
    return ((dist) < 256 ? dist_code[dist] : dist_code[256+((dist)>>7)]);
}

int ct_tally(int dist, int lc)
{
    inbuf[last_lit++] = (uch)lc;
    if (dist == 0) {
        dyn_ltree[lc].fc.freq++;
    } else {
        dist--;
        dyn_ltree[length_code[lc]+LITERALS+1].fc.freq++;
        dyn_dtree[d_code(dist)].fc.freq++;

        d_buf[last_dist++] = (ush)dist;
        flags |= flag_bit;
    }
    flag_bit <<= 1;

    if ((last_lit & 7) == 0) {
        flag_buf[last_flags++] = flags;
        flags = 0, flag_bit = 1;
    }
    if (level > 2 && (last_lit & 0xfff) == 0) {
        ulg out_length = (ulg)last_lit*8L;
        ulg in_length = (ulg)strstart-block_start;
        int dcode;
        for (dcode = 0; dcode < D_CODES; dcode++) {
            out_length += (ulg)dyn_dtree[dcode].fc.freq*(5L+extra_dbits[dcode]);
        }
        out_length >>= 3;
        
        if (last_dist < last_lit/2 && out_length < in_length/2) return 1;
    }
    return (last_lit == LIT_BUFSIZE-1 || last_dist == DIST_BUFSIZE);
}

static void compress_block(ct_data *ltree, ct_data *dtree)
{
    unsigned dist;
    int lc;
    unsigned lx = 0;
    unsigned dx = 0;
    unsigned fx = 0;
    uch flag = 0;
    unsigned code;
    int extra;

    if (last_lit != 0) do
    {
        if ((lx & 7) == 0) flag = flag_buf[fx++];
        lc = inbuf[lx++];

        if ((flag & 1) == 0)
        {
            send_bits(ltree[lc].fc.code, ltree[lc].dl.len);
        }
        else
        {
            code = length_code[lc];
            send_bits(ltree[code + LITERALS + 1].fc.code, ltree[code + LITERALS + 1].dl.len);
            extra = extra_lbits[code];
            if (extra != 0) {
                lc -= base_length[code];
                send_bits(lc, extra);
            }
            dist = d_buf[dx++];
            code = d_code(dist);

            send_bits(dtree[code].fc.code, dtree[code].dl.len);
            extra = extra_dbits[code];
            if (extra != 0) {
                dist -= base_dist[code];
                send_bits(dist, extra);
            }
        }
        flag >>= 1;
    } while (lx < last_lit);

    send_bits(ltree[END_BLOCK].fc.code, ltree[END_BLOCK].dl.len);
}

static void set_file_type()
{
    int n = 0;
    unsigned ascii_freq = 0;
    unsigned bin_freq = 0;
    while (n < 7)        bin_freq += dyn_ltree[n++].fc.freq;
    while (n < 128)    ascii_freq += dyn_ltree[n++].fc.freq;
    while (n < LITERALS) bin_freq += dyn_ltree[n++].fc.freq;
    *file_type = bin_freq > (ascii_freq >> 2) ? BINARY : ASCII;
    if (*file_type == BINARY && 0) {
        warning ((char *)"-l used on binary file");
    }
}

static unsigned  decode  (unsigned count, uch buffer[]);
static void decode_start (void);
static void huf_decode_start (void);
static unsigned decode_c     (void);
static unsigned decode_p     (void);
static void read_pt_len      (int nn, int nbit, int i_special);
static void read_c_len       (void);
static void fillbuf      (int n);
static unsigned getbits  (int n);
static void init_getbits (void);

static const uint32_t DICSIZ = 1 << 13;

static const int32_t NC = UCHAR_MAX + 255;
static const uint8_t NP = 14;
static const uint8_t NT = 19;
static const uint8_t PBIT = 4;
static const uint8_t TBIT = 5;
static uch pt_len[19];
static unsigned blocksize;
static ush pt_table[256];
static ush       io_bitbuf;
static unsigned  subbitbuf;
static int       bitcount;
uint32_t BITBUFSIZ = CHAR_BIT * 2 * sizeof(char);

static void fillbuf(int n)
{
    io_bitbuf <<= n;
    while (n > bitcount)
    {
        io_bitbuf |= subbitbuf << (n -= bitcount);
        subbitbuf = (unsigned)(inptr < insize ? inbuf[inptr++] : fill_inbuf(1));

        if ((int)subbitbuf == EOF) subbitbuf = 0;
            bitcount = CHAR_BIT;
    }
    io_bitbuf |= subbitbuf >> (bitcount -= n);
}

static unsigned getbits(int n)
{
    unsigned x;

    x = io_bitbuf >> (BITBUFSIZ - n);  fillbuf(n);
    return x;
}

static void init_getbits()
{
    io_bitbuf = 0;  subbitbuf = 0;  bitcount = 0;
    fillbuf(BITBUFSIZ);
}



static void make_table(int nchar, uch bitlen[], int tablebits, ush table[])
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

    for (i = 1; i <= (unsigned)tablebits; i++)
    {
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
		    (prev + WSIZE)[avail] = prev[avail] = 0;
		    *p = avail++;
		}
		if (k & mask) p = &(prev + WSIZE)[*p];
		else          p = &prev[*p];
		k <<= 1;  i--;
	    }
	    *p = ch;
	}
	start[len] = nextcode;
    }
}



static void read_pt_len(int nn, int nbit, int i_special)
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



static void read_c_len()
{
    int i, c, n;
    unsigned mask;
    n = getbits(9);

    if (n == 0)
    {
        c = getbits(9);
        for (i = 0; i < NC; i++) outbuf[i] = 0;
        for (i = 0; i < 4096; i++) d_buf[i] = c;
    }
    else
    {
        i = 0;

        while (i < n)
        {
            c = pt_table[io_bitbuf >> (BITBUFSIZ - 8)];

            if (c >= NT)
            {
                mask = (unsigned) 1 << (BITBUFSIZ - 1 - 8);

                do
                {
                    c = io_bitbuf & mask ? (prev + WSIZE)[c] : prev[c];
                    mask >>= 1;
                }
                while (c >= NT);
            }

            fillbuf((int) pt_len[c]);

            if (c <= 2)
            {
                if (c == 0)
                    c = 1;
                else if (c == 1)
                    c = getbits(4) + 3;
                else
                    c = getbits(9) + 20;

                while (--c >= 0)
                    outbuf[i++] = 0;
            }
            else
            {
                outbuf[i++] = c - 2;
            }
        }

        while (i < NC)
            outbuf[i++] = 0;

        make_table(NC, outbuf, 12, d_buf);
    }
}



static unsigned decode_c()
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
    j = d_buf[io_bitbuf >> (BITBUFSIZ - 12)];
    if (j >= NC) {
	mask = (unsigned) 1 << (BITBUFSIZ - 1 - 12);
	do {
	    if (io_bitbuf & mask) j = (prev + WSIZE)[j];
	    else               j = prev[j];
	    mask >>= 1;
	} while (j >= NC);
    }
    fillbuf((int) outbuf[j]);
    return j;
}

static unsigned decode_p()
{
    unsigned j, mask;

    j = pt_table[io_bitbuf >> (BITBUFSIZ - 8)];
    if (j >= NP) {
	mask = (unsigned) 1 << (BITBUFSIZ - 1 - 8);
	do {
	    if (io_bitbuf & mask) j = (prev + WSIZE)[j];
	    else               j = prev[j];
	    mask >>= 1;
	} while (j >= NP);
    }
    fillbuf((int) pt_len[j]);
    if (j != 0) j = ((unsigned) 1 << (j - 1)) + getbits((int) (j - 1));
    return j;
}

static void huf_decode_start()
{
    init_getbits();  blocksize = 0;
}

static int j;    
static int done; 

static void decode_start()
{
    huf_decode_start();
    j = 0;
    done = 0;
}

static unsigned decode(unsigned count, uch buffer[])
{
    static unsigned i;
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
	    j = c - (UCHAR_MAX + 1 - 3);
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

typedef	unsigned char	char_type;
typedef          long   code_int;
typedef unsigned long 	count_int;
typedef unsigned short	count_short;
typedef unsigned long 	cmp_code_int;


    
union	bytes {
    long  word;
    struct {
	int  dummy;
    } bytes;
};

int block_mode = BLOCK_MODE;

int unlzw(int in, int out) 
{
    char_type  *stackp;
    code_int   code;
    int        finchar;
    code_int   oldcode;
    code_int   incode;
    long       inbits;
    long       posbits;
    int        outpos;
    unsigned   bitmask;
    code_int   free_ent;
    code_int   maxcode;
    code_int   maxmaxcode;
    int        n_bits;
    int        rsize;
    
    maxbits = get_byte();
    block_mode = maxbits & BLOCK_MODE;
    if ((maxbits & LZW_RESERVED) != 0) {
    }
    maxbits &= 0x1f;
    maxmaxcode = 1<<maxbits;
    
    if (maxbits > BITS) {
	fprintf(stderr,
		"\n%s: %s: compressed with %d bits, can only handle %d bits\n",
		progname, ifname, maxbits, BITS);
	exit_code = ERROR;
	return ERROR;
    }
    rsize = insize;
    maxcode = (1<<(n_bits = INIT_BITS))-1;
    bitmask = (1<<n_bits)-1;
    oldcode = -1;
    finchar = 0;
    outpos = 0;
    posbits = inptr<<3;

    free_ent = ((block_mode) ? FIRST : 256);
    
    memset(prev, 0, 256);
    
    for (code = 255 ; code >= 0 ; --code) {
	window[code] = (char_type)code;
    }
    do {
	register int i;
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
		    maxcode = (1<<n_bits)-1;
		}
		bitmask = (1<<n_bits)-1;
		goto resetbuf;
	    }

    { 
     register char_type *p = &(inbuf)[(posbits)>>3];
     (code) = ((((long)(p[0]))|((long)(p[1])<<8)| 
     ((long)(p[2])<<16))>>((o)&0x7))&(bitmask); 
     (posbits) += (n_bits); 
   }

	    if (oldcode == -1) {
		if (code >= 256) error((char *)"corrupt input.");
		outbuf[outpos++] = (char_type)(finchar = (int)(oldcode=code));
		continue;
	    }
	    if (code == 256 && block_mode) {
    memset(prev, 0, 256);

		free_ent = FIRST - 1;
		posbits = ((posbits-1) +
			   ((n_bits<<3)-(posbits-1+(n_bits<<3))%(n_bits<<3)));
		maxcode = (1<<(n_bits = INIT_BITS))-1;
		bitmask = (1<<n_bits)-1;
		goto resetbuf;
	    }
	    incode = code;
	    stackp = ((char_type *)(&d_buf[DIST_BUFSIZE-1]));
	    
	    if (code >= free_ent) {
		if (code > free_ent) {
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
		*--stackp = window[code];
		code = prev[code];
	    }
	    *--stackp =	(char_type)(finchar = window[code]);
	    
	    {
		register int i;
	    
		if (outpos+(i = (((char_type *)(&d_buf[DIST_BUFSIZE-1])) - stackp)) >= OUTBUFSIZ) {
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
		    } while ((i = (((char_type *)(&d_buf[DIST_BUFSIZE-1])) - stackp)) > 0);
		} else {
		    memcpy(outbuf+outpos, stackp, i);
		    outpos += i;
		}
	    }

	    if ((code = free_ent) < maxmaxcode) {

		prev[code] = (unsigned short)oldcode;
		window[code] = (char_type)finchar;
		free_ent = code+1;
	    } 
	    oldcode = incode;
	}
    } while (rsize != 0);
    
    if (!test && outpos > 0) {
	write_buf(out, (char*)outbuf, outpos);
	bytes_out += (off_t)outpos;
    }
    return OK;
}



static ulg orig_len;  
static int max_len;   
static uch literal[256];
static int lit_base[26];
static int leaves [26];
static int parents[26];
static int peek_bits;
static ulg bitbuf;
static int valid;



static void read_tree  (void);
static void build_tree (void);

static void read_tree()
{
    int len;  
    int base; 
    int n;

    orig_len = 0;
    for (n = 1; n <= 4; n++) orig_len = (orig_len << 8) | (ulg)get_byte();

    max_len = (int)get_byte(); 
    if (max_len > 25) {
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

static void build_tree()
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

    peek_bits = max_len <= 12 ? max_len : 12;
    prefixp = &outbuf[1<<peek_bits];
    for (len = 1; len <= peek_bits; len++) {
	int prefixes = leaves[len] << (peek_bits-len);
	while (prefixes--) *--prefixp = (uch)len;
    }
    while (prefixp > outbuf) *--prefixp = 0;
}

void put_ubyte(char c)
{
    window[outcnt++] = (uch)(c);

    if (outcnt == WSIZE)
        flush_window();
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
    valid = 0;
    bitbuf = 0;
    peek_mask = (1<<peek_bits)-1;
    eob = leaves[max_len]-1;

    for (;;)
    {
  while (valid < (peek_bits)) bitbuf = (bitbuf<<8) | (ulg)get_byte(), valid += 8;
  peek = (bitbuf >> (valid-(peek_bits))) & (peek_mask);

        len = outbuf[peek];

        if (len > 0)
        {
        peek >>= peek_bits - len;
        }
        else
        {
        ulg mask = peek_mask;
        len = peek_bits;
        do
        {
            len++, mask = (mask<<1)+1;

while (valid < (len)) bitbuf = (bitbuf<<8) | (ulg)get_byte(), valid += 8;
  peek = (bitbuf >> (valid-(len))) & (mask);

        } while (peek < (unsigned)parents[len]);
    }

	if (peek == eob && len == max_len) break;
	put_ubyte(literal[peek+lit_base[len]]);
    valid -= len;
    }

    flush_window();
    if (orig_len != (ulg)(bytes_out & 0xffffffff)) {
	error((char *)"invalid compressed data--length error");
    }
    return OK;
}








int check_zipfile(int in)
{
    uch *h = inbuf + inptr;

    ifd = in;
    inptr += 30 + SH(h + 26) + SH(h + 28);

    if (inptr > insize || ((ulg)(SH(h)) | ((ulg)(SH((h)+2)) << 16))  != LOCSIG) {
	fprintf(stderr, "\n%s: %s: not a valid zip file\n",
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }
    method = h[8];
    if (method != STORED && method != DEFLATED)
    {
        fprintf(stderr,
    	    "\n%s: %s: first entry not deflated or stored -- use unzip\n",
    	    progname, ifname);

        exit_code = ERROR;
	    return ERROR;
    }

    if ((decrypt = h[LOCFLG] & 1) != 0) {
	fprintf(stderr, "\n%s: %s: encrypted file -- use unzip\n",
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }

    ext_header = (h[LOCFLG] & 8) != 0;
    pkzip = 1;

    return OK;
}

ulg LG(uch *p)
{
    return ((ulg)(SH(p)) | ((ulg)(SH((p)+2)) << 16));
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

    if (pkzip && !ext_header)
    { 
	    orig_crc = LG(inbuf + 14);
	    orig_len = LG(inbuf + LOCLEN);
    }

    if (method == DEFLATED)  {

	int res = inflate();

	if (res == 3) {
	    error((char *)"out of memory");
	} else if (res != 0) {
	    error((char *)"invalid compressed data--format violated");
	}

    } else if (pkzip && method == STORED) {

	register ulg n = LG(inbuf + LOCLEN);

	if (n != LG(inbuf + LOCSIZ) - (decrypt ? 12 : 0)) {

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

    } else if (ext_header) {

        for (n = 0; n < EXTHDR; n++)
            buf[n] = (uch)get_byte();
	
	orig_crc = LG(buf+4);
	orig_len = LG(buf+12);
    }

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

    if (pkzip && inptr + 4 < insize && LG(inbuf+inptr) == LOCSIG) {
	if (to_stdout) {
	} else {
	    fprintf(stderr,
		    "%s: %s has more than one entry -- unchanged\n",
		    progname, ifname);
	    err = ERROR;
	}
    }
    ext_header = pkzip = 0;
    if (err == OK) return OK;
    exit_code = ERROR;
    if (!test) abort_gzip();
    return err;
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
    return c ^ 0xffffffffL;
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
        *t = isupper((uint8_t)*t) ? tolower((uint8_t)*t) : (uint8_t)*t;

    return s;
}

char *base_name(char *fname)
{
    char *p;

    if ((p = strrchr(fname, '/'))  != NULL) fname = p+1;
    if ('A' == 'a') strlwr(fname);
    return fname;
}

int xunlink(char *filename)
{
    int r = unlink (filename);
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



void error(char *m)
{
    fprintf(stderr, "\n%s: %s: %s\n", progname, ifname, m);
    abort_gzip();
}

void warning (char *m)
{
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



int rpmatch();

int yesno()
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



void put_long(int n)
{
    put_short((n) & 0xffff);
    put_short(((ulg)(n)) >> 16);
}

int zip(int in, int out)
{
    uch  flags = 0;         
    ush  attr = 0;          
    ush  deflate_flags = 0; 
    ifd = in;
    ofd = out;
    outcnt = 0;
    method = DEFLATED;
    put_byte(GZIP_MAGIC[0]);
    put_byte(GZIP_MAGIC[1]);
    put_byte(DEFLATED);     
    if (save_orig_name) flags |= ORIG_NAME;
    put_byte(flags);
    put_long(time_stamp == (time_stamp & 0xffffffff) ? (ulg)time_stamp : (ulg)0);
    crc = updcrc(0, 0);
    bi_init(out);
    ct_init(&attr, &method);
    lm_init(level, &deflate_flags);
    put_byte((uch)deflate_flags);
    put_byte(3);

    if (save_orig_name) {
	char *p = base_name(ifname);
	do {
	    put_byte(*p);
	} while (*p++);
    }
    header_bytes = (off_t)outcnt;
    (void)deflate();
    put_long(crc);
    put_long((ulg)bytes_in);
    header_bytes += 2*sizeof(long);
    flush_outbuf();
    return OK;
}

int file_read(char *buf, unsigned size)
{
    unsigned len;


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





