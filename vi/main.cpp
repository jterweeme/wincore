#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

class Display
{
    uint16_t * const _base;
    const uint8_t loByte(const uint16_t a) { return a & 0xff; }
public:
    Display() : _base((uint16_t *)0xb8000000) { }
    void clear() { for (int i = 0; i < 80*25; i++) _base[i] = 0;    }

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

    void gotoxy(int x, int y)
    {
        union REGS regs;
        regs.h.ah = 2;
        regs.h.bh = 0;
        regs.h.dh = y;
        regs.h.dl = x;
        int86(0x10, &regs, &regs);
    }

    void setMode(int mode)
    {
        union REGS regs;
        regs.h.ah = 0;
        regs.h.al = mode;
        int86(0x10, &regs, &regs);
    }

};

class App
{
    const int _argc;
    const char **_argv;
    Display _display;
    int _mode;
public:
    App(const int argc, const char **argv) : _argc(argc), _argv(argv) { }
    int run();
};

class Keys
{
public:
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

int App::run()
{
    _mode = 1;
    _display.clear();

    for (int i = 0; i < 20; i++)
    {
        _display.write(0, i, 0x09, '~');
    }

    _display.gotoxy(0, 0);

    for (int i = 0; i < 20; i++)
        _display.setAttr(i, 0, 0xf);

    while (getch() != Keys::ESC) { }
    _display.setMode(3);

    return 0;
}

int main(const int argc, const char **argv)
{
    App app(argc, argv);
    return app.run();
}


