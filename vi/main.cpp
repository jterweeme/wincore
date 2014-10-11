#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <iostream>
#include <sstream>
using namespace std;

struct Pos
{
    uint8_t x, y;
    Pos(uint8_t x, uint8_t y) : x(x), y(y) { }
    Pos& operator ++() { x++; return *this; }
};

class Display
{
    uint16_t * const _base;
    const uint8_t loByte(const uint16_t a) { return a & 0xff; }
    Pos _pos;

    void gotoxy(int x, int y)
    {
        union REGS regs;
        regs.h.ah = 2;
        regs.h.bh = 0;
        regs.h.dh = y;
        regs.h.dl = x;
        int86(0x10, &regs, &regs);
    }
public:
    Display() : _base((uint16_t *)0xb8000000), _pos(0,0) { }
    void clear() { for (int i = 0; i < 80*25; i++) _base[i] = 0; }

    inline void write(uint16_t addr, uint8_t attr, char c)
    {   _base[addr] = attr << 8 | c;
    }

    inline void write(uint8_t x, uint8_t y, uint8_t attr, char c)
    {   write(y * 80 + x, attr, c);
    }

    inline void write(uint16_t addr, uint8_t attr, const char *s)
    {   for (size_t i = 0; i < strlen(s); i++)
            write(addr + i, attr, s[i]);
    }

    inline void write(uint8_t x, uint8_t y, uint8_t attr, const char *s)
    {   write(y * 80 + x, attr, s);
    }

    void setAttr(uint16_t addr, uint8_t attr)
    {   _base[addr] = attr << 8 | loByte(_base[addr]);
    }

    void setAttr(uint8_t x, uint8_t y, uint8_t attr)
    {   setAttr(y * 80 + x, attr);
    }

    void setAttrLine(uint8_t y, uint8_t attr)
    {   for (int i = 0; i < 80; i++)
            setAttr(i, y, attr);
    }



    void gotoxy(Pos pos) { _pos = pos; gotoxy(pos.x, pos.y); }

    void setMode(int mode)
    {
        union REGS regs;
        regs.h.ah = 0;
        regs.h.al = mode;
        int86(0x10, &regs, &regs);
    }

    void putc(char c)
    {
        union REGS regs;
        regs.h.ah = 0x0a;
        regs.h.al = c;
        regs.h.bh = 0;
        regs.x.cx = 1;
        int86(0x10, &regs, &regs);
        gotoxy(++_pos);
    }

};

class CommandLine
{
    char _buf[255];
    int _bufPos;
public:
    CommandLine() : _bufPos(0) { memset(_buf, 0, sizeof(_buf)); }
    void put(char c) { _buf[_bufPos++] = c; }
    void dump(ostream &os);
};

class App
{
    const int _argc;
    const char **_argv;
    Display _display;
    CommandLine _cmdLine;
    int _mode;
public:
    App(const int argc, const char **argv) : _argc(argc), _argv(argv) { }
    int run();
};

class Keys
{
public:
    static const uint8_t ENTER = 0x0d;
    static const uint8_t ESC = 0x1b;
    static const uint8_t K0 = 0x30;
    static const uint8_t K1 = 0x31;
    static const uint8_t K9 = 0x39;
    static const uint8_t COLON = 0x3a;
    static const uint8_t SEMICOLON = 0x3b;
    static const uint8_t LT = 0x3c;
    static const uint8_t F3 = 0x3d;
    static const uint8_t F4 = 0x3e;
    static const uint8_t F5 = 0x3f;
    static const uint8_t F6 = 0x40;
};

void CommandLine::dump(ostream &os)
{
    os << _buf;
}

int App::run()
{
    _mode = 1;
    _display.clear();

    for (int i = 1; i < 24; i++)
    {
        _display.write(0, i, 0x09, '~');
    }

    _display.gotoxy(Pos(0, 0));
    _display.setAttrLine(0, 0xf);

    while (int k = getch())
    {
        switch (k)
        {
        case Keys::COLON:
            _display.gotoxy(Pos(0,23));
            _display.setAttrLine(23, 0xf);
            _cmdLine.put(':');
            _display.putc(':');
            break;
        case Keys::ENTER:
            break;
        case Keys::ESC:
            _display.setMode(3);
            _cmdLine.dump(cout);
            return 0;
        default:
            _cmdLine.put(k);
            _display.putc(k);
            break;
        
        }
    }

    return 0;
}

int main(const int argc, const char **argv)
{
    App app(argc, argv);
    return app.run();
}


