#ifndef _TIME_H
#define _TIME_H

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
#ifndef __time_t_defined
    typedef uint32_t time_t;
#endif
    tm *gmtime(time_t *t);
    tm *localtime(time_t *t);
    time_t time(time_t *timer);
    time_t mktime(tm *timeptr);
    size_t strftime(char *p, size_t max, const char *fmt, const tm *tp);
}

#endif



