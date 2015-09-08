#ifndef _TIME_H
#define _TIME_H
#include "util2.h"

struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
};

namespace mystl
{

#ifdef __MINGW64__
#define __time_t_defined
#endif

#ifndef __time_t_defined
    typedef uint32_t time_t;
#endif
    tm *gmtime(time_t *t);
    tm *localtime(time_t *t);
    time_t time(time_t *timer);
    time_t mktime(tm *timeptr);
    size_t strftime(char *p, size_t max, const char *fmt, const tm *tp);
}

class Time2 : public Util2
{
    static tm _tm;
public:
    //bool isDST() const { return _tm.tm_yday > 50 && _tm.tm_yday < 298; }
    bool isDST() const { return _tm.tm_yday > 88 && _tm.tm_yday < 298; }
    void set(int y, int yd, int mon, int d, int h, int min, int s);
    void reset() { set(70, 0, 0, 1, 0, 0, 0); }
    size_t strftime(char *p, size_t max, const char *fmt, const tm *tp) const;
    tm *gmtime(const time_t *timer);
    tm *localtime(const time_t *t);
    bool isLeap(uint32_t year) const;
    int daysInMonth(uint32_t month) const;
    void incSec();
    void incHour();
    void incDay();
    void incSec(uint32_t s);
    void set(uint32_t s) { reset(); incSec(s); }
    time_t time(time_t *timer) { return 0; }
    time_t mktime(tm *timeptr) { return 0; }
};



#endif



