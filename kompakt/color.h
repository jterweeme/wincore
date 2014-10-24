#ifndef _COLOR_H_
#define _COLOR_H_
#include <conio.h>

class Color
{
    int _color;
public:
    Color() : _color(1) { }
    void color(int i) { _color = i; }

    Color& operator << (const char *s)
    {
#ifdef __GO32__
        switch (_color)
        {
        case 1:
            textattr(2);
            break;
        case 2:
            textattr(4);
            break;
        }
        cprintf(s);
#else
        switch (_color)
        {
        case 1:
            cerr << "\033[1;32m";
            break;
        case 2:
            cerr << "\033[1;31m";
            break;
        }
        cerr << s << "\033[0m";
#endif       
        return *this;
    }
};
#endif

