#include "gunzip2.h"

int GzipStreambuf2::underflow()
{
    if (gptr() < egptr()) return (int)(*gptr());
    
    return 0;
}



