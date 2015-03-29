#include "common.h"

size_t Time2::strftime(char *p, size_t max, const char *fmt, const tm *tp) const
{
    if (strcmp(fmt, "%F %T") == 0)
        snprintf(p, max, "%u-%02u-%02u %02u:%02u:%02u",
            tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);

    if (strcmp(fmt, "%F %R") == 0)
        snprintf(p, max, "%u-%02u-%02u %02u:%02u",
            tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min);

    return 0;
}

bool Time2::isLeap(uint32_t year) const
{
    switch (year)
    {
    case 72:
    case 76:
    case 80:
    case 84:
    case 88:
    case 92:
    case 96:
    case 100:
    case 104:
    case 108:
    case 112:
    case 116:
        return true;
    }

    return false;
}

int Time2::daysInMonth(uint32_t month) const
{
    switch (month)
    {
    case 1:
        return isLeap(_tm.tm_year) ? 29 : 28;
    case 3:
    case 5:
    case 8:
    case 10:
        return 30;
    default:
        return 31;
    }
}

void Time2::set(int y, int yd, int mon, int d, int h, int min, int s)
{
    _tm.tm_year = y;
    _tm.tm_yday = yd;
    _tm.tm_mon = mon;
    _tm.tm_mday = d;
    _tm.tm_hour = h;
    _tm.tm_min = min;
    _tm.tm_sec = s;
}

void Time2::incSec()
{
    if (++_tm.tm_sec < 60) return;
    _tm.tm_sec = 0;
    if (++_tm.tm_min < 60) return;
    _tm.tm_min = 0;
    incHour();
}

void Time2::incHour()
{
    if (++_tm.tm_hour < 24) return;
    _tm.tm_hour = 0;
    incDay();
}

void Time2::incDay()
{
    _tm.tm_yday++;
    if (++_tm.tm_mday <= daysInMonth(_tm.tm_mon)) return;
    _tm.tm_mday = 1;
    if (++_tm.tm_mon < 12) return;
    _tm.tm_yday = 0;
    _tm.tm_mon = 0;
    _tm.tm_year++;
}

void Time2::incSec(uint32_t s)
{
    for (; s>= 86400; s -= 86400)
        incDay();

    for (uint32_t i = 0; i < s; i++)
        incSec();
}

tm Time2::_tm;

tm *Time2::gmtime(const time_t *timer)
{
    set(*timer);
    return &_tm;
}

tm *Time2::localtime(const time_t *timer)
{
    set(*timer);
    if (isDST()) incHour();
    incHour();
    return &_tm;
}



