#include "gunzip2.h"

int GzipStreambuf::underflow()
{
    if (gptr() < egptr()) return (int)(*gptr());
    
    return 0;
}



