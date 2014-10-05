#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>

enum {
    DEFAULT_PUTBACK_SIZE = 4,
    DEFAULT_MAINBUF_SIZE = 512,
    DEFAULT_BUF_SIZE     = DEFAULT_MAINBUF_SIZE + DEFAULT_PUTBACK_SIZE
};


#ifndef _OSTREAM_INCLUDED
#define _OSTREAM_INCLUDED

#ifndef _IOS_INCLUDED
#define _IOS_INCLUDED

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


#ifndef __cplusplus
#error The header ios requires C++
#endif

#if !defined( EOF )
    #define EOF (-1)
#endif

#define __NOT_EOF 0

#ifndef _WATCOM_EXCEPTION_DEFINED
#define _WATCOM_EXCEPTION_DEFINED
#define _WATCOM_EXCEPTION_DEFINED_
struct __WATCOM_exception {
#if defined(__AXP__) || defined(__PPC__)
    void *__filler;
#endif
};
#endif

#ifndef __lock_it

class __lock;

#ifdef __SW_BM
class _WPRTLINK __get_lock {
public:
    __get_lock( __lock * );
    ~__get_lock();
private:
    __lock *__lk;
};
#define __lock_it( __l ) __get_lock __lock_name( __LINE__ )( __l )
#define __lock_name( __ln ) __lock_glue( __lock__, __ln )
#define __lock_glue( __pre, __lin ) __pre ## __lin
#else
#define __lock_it( __l )
#endif
#endif

class _WPRTLINK __WATCOM_ios;

namespace std {

  typedef long streampos;

  typedef long streamoff;

  class _WPRTLINK istream;
  class _WPRTLINK ostream;
  class _WPRTLINK streambuf;

#ifdef _M_IX86
#pragma pack( __push, 1 )
#else
#pragma pack( __push, 8 )
#endif

  class _WPRTLINK ios {
  public:
    enum io_state {                   // Error state
        goodbit = 0x00,               // - no errors
        badbit  = 0x01,               // - operation failed, may not proceed
        failbit = 0x02,               // - operation failed, may proceed
        eofbit  = 0x04                // - end of file
    };
    typedef int iostate;
    enum open_mode {                  // How to open a stream
        in        = 0x0001,           // - open for input
        out       = 0x0002,           // - open for output
        atend     = 0x0004,           // - seek to end after opening
        append    = 0x0008,           // - open for output, append to the end
        truncate  = 0x0010,           // - discard contents after opening
        nocreate  = 0x0020,           // - open only an existing file
        noreplace = 0x0040,           // - open only a new file
        text      = 0x0080,           // - open as text file
        binary    = 0x0100,           // - open as binary file

        app       = append,           // Historical purposes
        ate       = atend,
        trunc     = truncate
    };
    typedef int openmode;
    enum seek_dir {                   // Seek direction
        beg       = 0x00,             // - seek from beginning
        cur       = 0x01,             // - seek from current position
        end       = 0x02              // - seek from end
    };

   typedef int seekdir;
    enum fmt_flags {                  // Format flags
        skipws     = 0x0001,          // - skip whitespace
        left       = 0x0002,          // - align field to left edge
        right      = 0x0004,          // - align field to right edge
        internal   = 0x0008,          // - sign at left, value at right
        dec        = 0x0010,          // - decimal conversion for integers
        oct        = 0x0020,          // - octal conversion for integers
        hex        = 0x0040,          // - hexadecimal conversion for integers
        showbase   = 0x0080,          // - show dec/octal/hex base on output
        showpoint  = 0x0100,          // - show decimal and digits on output
        uppercase  = 0x0200,          // - use E,X (not e,x) on output numbers
        showpos    = 0x0400,          // - use + for output positive numbers
        scientific = 0x0800,          // - use scientific notation for output
        fixed      = 0x1000,          // - use floating notation for output
        unitbuf    = 0x2000,          // - flush stream after output
        stdio      = 0x4000,          // - flush stdout/stderr after output

        #define _LAST_FORMAT_FLAG 0x00004000
        #define _LAST_FLAG_BIT    0x80000000

        basefield  = dec | oct | hex,
        adjustfield= left | right | internal,
        floatfield = scientific | fixed,
    };
    typedef long fmtflags;

    class failure : public __WATCOM_exception {
        iostate __cause;
    public:
        failure( iostate );           // - set the cause of failure
        iostate cause() const;        // - query the cause of failure
    };

    ios( streambuf *__sb );
    ios( ios const & );
    virtual ~ios();

   ios &operator = ( ios const & );
        operator void * () const;
    int operator !      () const;

    ostream   *tie( ostream *__ostrm );
    ostream   *tie() const;
    streambuf *rdbuf() const;
    iostate    rdstate() const;
    iostate    clear( iostate __state = 0 );
    int        good() const;
    int        bad()  const;
    int        fail() const;
    int        eof()  const;
    iostate    exceptions( iostate __enable );
    iostate    exceptions() const;
    fmtflags   setf( fmtflags __onbits, fmtflags __mask );
    fmtflags   setf( fmtflags __onbits );
    fmtflags   unsetf( fmtflags __offbits );
    fmtflags   flags( fmtflags __bits );
    fmtflags   flags() const;
    char       fill( char __fillchar );
    char       fill() const;
    int        precision( int __precision );
    int        precision() const;
    int        width( int __width );
    int        width() const;
    long      &iword( int __index );
    void     *&pword( int __index );

    static void sync_with_stdio( void ) {};     // obsolete function

    static fmtflags bitalloc();
    static int      xalloc();

    __lock *__i_lock;        // ios data member operations
    static __lock *__x_lock; // xalloc/bitalloc operations

  protected:
    ios();

    void init( streambuf *__sb );
    void setstate( ios::iostate __orbits );

  private:
    streambuf *__strmbuf;
    ostream   *__tied_stream;
    long       __format_flags;
    int        __error_state;
    int        __enabled_exceptions;
    int        __float_precision;
    int        __field_width;
    void      *__xalloc_list;
    char       __fill_character;

    static int       __xalloc_index;
    static fmtflags  __last_format_flag;

    friend class __WATCOM_ios;
    int : 0;
  };
#pragma pack( __pop )

  inline streambuf *ios::rdbuf() const {
    return( __strmbuf );
  }

  inline ios::iostate ios::rdstate() const {
    return( __error_state );
  }

  inline int ios::good() const {
    return( __error_state == 0 );
  }

  inline int ios::bad() const {
    return( __error_state & ios::badbit );
  }

  inline int ios::fail() const {
    return( __error_state & (ios::failbit|ios::badbit) );
  }

  inline int ios::eof() const {
    return( __error_state & ios::eofbit );
  }

  inline ios::iostate ios::exceptions() const {
    return( __enabled_exceptions );
  }

  inline ios::operator void * () const {
    return( (void *) (fail()==0) );
  }

  inline int ios::operator ! () const {
    return( fail() );
  }

  inline ios::fmtflags ios::setf( ios::fmtflags __onbits ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags           |= __onbits;
    return( __old_flags );
  }

  inline ios::fmtflags ios::setf( ios::fmtflags __onbits, ios::fmtflags __mask ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags           &= ~__mask;
    __format_flags           |= __onbits & __mask;
    return( __old_flags );
  }

  inline ios::fmtflags ios::unsetf( ios::fmtflags __offbits ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags           &= ~__offbits;
    return( __old_flags );
  }

  inline ios::fmtflags ios::flags( ios::fmtflags __flags ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags            = __flags;
    return( __old_flags );
  }

  inline ios::fmtflags ios::flags() const {
    return( __format_flags );
  }

  inline char ios::fill( char __fillchar ) {
    __lock_it( __i_lock );
    char __old_fill  = __fill_character;
    __fill_character = __fillchar;
    return( __old_fill );
  }

  inline char ios::fill() const {
    return( __fill_character );
  }

  inline int ios::precision( int __precision ) {
    __lock_it( __i_lock );
    int __old_precision = __float_precision;
    __float_precision   = __precision;
    return( __old_precision );
  }

  inline int ios::precision() const {
    return( __float_precision );
  }

  inline int ios::width( int __width ) {
    __lock_it( __i_lock );
    int __old_width = __field_width;
    __field_width   = __width;
    return( __old_width );
  }

  inline int ios::width() const {
    return( __field_width );
  }

  inline ostream *ios::tie( ostream *__ostrm ) {
    __lock_it( __i_lock );
    ostream *__old_tie = __tied_stream;
    __tied_stream      = __ostrm;
    return( __old_tie );
  }

  inline ostream *ios::tie() const {
    return( __tied_stream );
  }

  _WPRTLINK extern ios &dec( ios & );
  _WPRTLINK extern ios &hex( ios & );
  _WPRTLINK extern ios &oct( ios & );

}

#endif



namespace std {

  class _WPRTLINK ostream : virtual public ios {
  public:
    ostream( streambuf *__sb );
    ostream( ostream const &__ostrm );
    virtual ~ostream();

    ostream &operator = ( streambuf *__sb );
    ostream &operator = ( ostream const &__ostrm );
    ostream &operator << (                char  __c );
    ostream &operator << (         signed char  __c );
    ostream &operator << (       unsigned char  __c );
    ostream &operator << (        signed short  __s );
    ostream &operator << (      unsigned short  __s );
    ostream &operator << (          signed int  __i );
    ostream &operator << (        unsigned int  __i );
    ostream &operator << (         signed long  __l );
    ostream &operator << (       unsigned long  __l );
    ostream &operator << (      signed __int64  __l );
    ostream &operator << (    unsigned __int64  __l );
    ostream &operator << (               float  __f );
    ostream &operator << (              double  __f );
    ostream &operator << (         long double  __f );
    ostream &operator << (                void *__p );
    ostream &operator << (           streambuf *__sb );
    ostream &operator << (          char const *__buf );
    ostream &operator << (   signed char const *__buf );
    ostream &operator << ( unsigned char const *__buf );
    ostream &operator << ( _WPRTLINK ostream &(*__f)( ostream & ) );
    ostream &operator << ( _WPRTLINK ios &(*__f)( ios & ) );

    int        opfx();
    void       osfx();
    ostream   &put(          char __c );
    ostream   &put(   signed char __c );
    ostream   &put( unsigned char __c );
    ostream   &write(          char const *__buf, int __len );
    ostream   &write(   signed char const *__buf, int __len );
    ostream   &write( unsigned char const *__buf, int __len );
    ostream   &flush();
    ostream   &seekp( streampos __position );
    ostream   &seekp( streamoff __offset, ios::seekdir __direction );
    streampos  tellp();

 protected:
    ostream();
    ostream &__outfloat( long double const & );
    ostream &do_lshift( char __c);
    int      do_opfx();
  };

#ifdef __BIG_INLINE__
  inline ostream &ostream::operator << ( char __c ) {
    __lock_it( __i_lock );
    if( opfx() ) {
        if( width() == 0 ) {
            if( rdbuf()->sputc( __c ) == EOF ) {
                setstate( ios::failbit );
            }
        } else {
            do_lshift( __c );
        }
        osfx();
    }
    return( *this );
  }
#endif

  inline ostream &ostream::operator << ( signed char __c ) {
    return( *this << (char) __c );
  }

  inline ostream &ostream::operator << ( unsigned char __c ) {
    return( *this << (char) __c );
  }

  inline ostream &ostream::operator << ( signed short __s ) {
    return( *this << (signed long) __s );
  }

  inline ostream &ostream::operator << ( unsigned short __s ) {
    return( *this << (unsigned long) __s );
  }

 inline ostream &ostream::operator << ( signed int __i ) {
    return( *this << (signed long) __i );
  }

  inline ostream &ostream::operator << ( unsigned int __i ) {
    return( *this << (unsigned long) __i );
  }

  inline ostream &ostream::operator << ( float __f ) {
    return( __outfloat( (long double)__f ) );
  }

  inline ostream &ostream::operator << ( double __f ) {
    return( __outfloat( (long double)__f ) );
  }

  inline ostream &ostream::operator << ( long double __f ) {
    return( __outfloat( __f ) );
  }

  inline ostream &ostream::operator << ( signed char const *__buf ) {
    return( *this << (char const *) __buf );
  }

  inline ostream &ostream::operator << ( unsigned char const *__buf ) {
    return( *this << (char const *) __buf );
  }

#ifdef __BIG_INLINE__
  inline ostream &ostream::put( char __c ) {
    __lock_it( __i_lock );
    if( opfx() ) {
        if( rdbuf()->sputc( __c ) == EOF ) {
            setstate( ios::failbit );
        }
        osfx();
    }
    return( *this );
  }
#endif

  inline ostream &ostream::put( signed char __c ) {
    return( put( (char) __c ) );
  }

  inline ostream &ostream::put( unsigned char __c ) {
    return( put( (char) __c ) );
  }

#ifdef __BIG_INLINE__
  inline ostream &ostream::write( char const *__buf, int __len ) {
    __lock_it( __i_lock );
    if( opfx() ) {
        if( __len  ) {
            if( rdbuf()->sputn( __buf, __len ) != __len ) {
                setstate( ios::failbit );
            }
        }
        osfx();
    }
    return( *this );
  }
#endif

  inline ostream &ostream::write( signed char const *__buf, int __len ) {
    return( write( (char const *) __buf, __len ) );
  }

  inline ostream &ostream::write( unsigned char const *__buf, int __len ) {
    return( write( (char const *) __buf, __len ) );
  }

  inline int ostream::opfx() {
    __lock_it( __i_lock );
    if( !good() ) {
        return 0;
    } else if( tie() || ( flags() & ios::stdio ) ) {
        return do_opfx();
    } else {
        return 1;
    }
  }

  inline void ostream::osfx() {
    __lock_it( __i_lock );
    if( flags() & ios::unitbuf ) {
        flush();
    }
  }

  _WPRTLINK extern ostream & endl( ostream & );
  _WPRTLINK extern ostream & ends( ostream & );
  _WPRTLINK extern ostream &flush( ostream & );

}

#endif


namespace std {
    _WPRTLINK extern ostream _WCDATA cout;
    _WPRTLINK extern ostream _WCDATA cerr;
}


template<class T> class _WPRTLINK smanip;

template<class T> class _WPRTLINK sapp
{
public:
    sapp( std::ios &(*__f)( std::ios &, T ) ) : __fn( __f ) {};
    smanip<T> operator()( T __p ) { return smanip<T>( __fn, __p ); };
private:
    std::ios &(*__fn)( std::ios &, T );
};

template<class T> class _WPRTLINK smanip {
public:
    smanip( std::ios &(*__f)( std::ios &, T ), T __p ) :
            __fn( __f ), __parm( __p ) {};
    friend _WPRTLINK std::ostream &operator<<( std::ostream &, const smanip<T> & );
private:
    std::ios &(*__fn)( std::ios &, T );
    T __parm;
};

template<class T>
    _WPRTLINK std::ostream &operator<<( std::ostream &__os, const smanip<T> &__sm )
{
    __sm.__fn( __os, __sm.__parm );
    return( __os );
}

template<class T> class _WPRTLINK omanip;

template<class T>
    class _WPRTLINK oapp {
    public:
        oapp( std::ostream &(*__f)( std::ostream &, T ) ) :
            __fn( __f ) {} ;
        omanip<T> operator()( T __p ) { return omanip<T>( __fn, __p ); };
    private:
        std::ostream &(*__fn)( std::ostream &, T );
    };

template<class T> class _WPRTLINK omanip
{
public:
    omanip( std::ostream &(*__f)( std::ostream &, T ), T __p ) :
            __fn( __f ), __parm( __p ) {};
    friend _WPRTLINK std::ostream &operator<<( std::ostream &, const omanip<T> & );
private:
    std::ostream &(*__fn)( std::ostream &, T );
    T __parm;
};

template<class T>
    _WPRTLINK std::ostream &operator<<( std::ostream &__os, const omanip<T> &__om )
{
    __om.__fn( __os, __om.__parm );
    return( __os );
}

template<class T> class _WPRTLINK iomanip;

template<class T> class _WPRTLINK ioapp {
    public:
        ioapp( std::iostream &(*__f)( std::iostream &, T ) ) :
            __fn( __f ) {};
        iomanip<T> operator()( T __p ) { return iomanip<T>( __fn, __p ) };
    private:
        std::iostream &(*__fn)( std::iostream &, T );
    };

template<class T>
    class _WPRTLINK iomanip {
    public:
        iomanip( std::iostream &(*__f)( std::iostream &, T ), T __p ) :
            __fn( __f ), __parm( __p ) {};
        friend _WPRTLINK std::iostream &operator>>( std::iostream &, const iomanip<T> & );
    private:
        std::iostream &(*__fn)( std::iostream &, T );
        T __parm;
    };

namespace std {
  _WPRTLINK extern sapp<long> _WCDATA resetiosflags;
  _WPRTLINK extern sapp<int>  _WCDATA setbase;
  _WPRTLINK extern sapp<int>  _WCDATA setfill;
  _WPRTLINK extern sapp<long> _WCDATA setiosflags;
  _WPRTLINK extern sapp<int>  _WCDATA setprecision;
  _WPRTLINK extern sapp<int>  _WCDATA setw;
}

namespace mystl
{

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

class iomanip
{
    int _type;
public:
    static const uint8_t HEX = 1;
    static const uint8_t DEC = 2;
    static const uint8_t OCT = 3;
    iomanip() { }
    iomanip(int type) : _type(type) { }
    int type() const { return _type; }
};

class ostream
{
    FILE *_fp;
    iomanip _manip;
public:
    ostream(FILE *fp) : _fp(fp) { }
    ostream& operator << (iomanip &i) { _manip = i; return *this; }
    ostream& operator << (const char *s) { fprintf(_fp, s); return *this; }
};

class istream
{
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    istream(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
    ~istream() { fclose(_fp); }
    uint16_t tellg() { return ftell(_fp); }
    uint16_t gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void read(char *s, size_t length)
    {
        _lastRead = fread(s, 1, length, _fp);
        _eof = _lastRead < length;
    }
};

class ifstream : public istream
{
public:
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream(char *s, int m) : istream(fopen(s, "rb")) { }
};

istream cin(stdin);
ostream cout(stdout);
ostream cerr(stderr);
iomanip hex(iomanip::HEX);
iomanip oct(iomanip::OCT);

iomanip setw(int length) { return hex; }
iomanip setfill(char c) { return hex; }

}

#endif


