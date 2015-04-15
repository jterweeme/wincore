#ifndef _MYTYPES_H_
#define _MYTYPES_H_

class MyTypes
{
public:
    typedef char int8_t;
    typedef unsigned char uint8_t;
    typedef short int16_t;
    typedef unsigned short uint16_t;
    typedef int int32_t;
    typedef unsigned uint32_t;
    typedef long long int64_t;
    typedef unsigned long long uint64_t;
#ifndef WIN32
    typedef long unsigned size_t;
#endif
    typedef long int ptrdiff_t;
    typedef uint32_t streampos;
    typedef int32_t streamsize;
    typedef int32_t streamoff;
};

struct stat2
{
    unsigned long st_dev;
    unsigned long st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned long st_rdev;
    long st_size;
    int st_blksize;
    long st_blocks;
    //long st_atime;
    //long st_mtime;
};

namespace mystl
{
    typedef MyTypes::streampos streampos;
#ifndef WIN32
    typedef MyTypes::size_t size_t;
#endif
#ifndef _STDINT_H
#ifndef _SYS_TYPES_H
    typedef MyTypes::int8_t int8_t;
#endif
    typedef MyTypes::uint8_t uint8_t;
    typedef MyTypes::int16_t int16_t;
    typedef MyTypes::uint16_t uint16_t;
    typedef MyTypes::int32_t int32_t;
    typedef MyTypes::uint32_t uint32_t;
    typedef MyTypes::int64_t int64_t;
    typedef MyTypes::uint64_t uint64_t;
#endif
}


#endif



