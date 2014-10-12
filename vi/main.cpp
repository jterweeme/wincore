#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

struct Pos
{
    uint8_t x, y;
    Pos(uint8_t x, uint8_t y) : x(x), y(y) { }
    Pos& operator ++() { x++; return *this; }
    Pos& operator --() { x--; return *this; }
    void newLine() { x = 0; y++; }
};

class Display
{
    uint16_t * const _base;
    const uint8_t loByte(const uint16_t a) { return a & 0xff; }
    Pos _pos;
    void gotoxy(int x, int y);
public:
    Display() : _base((uint16_t *)0xb8000000), _pos(0,0) { }
    void clear() { for (int i = 0; i < 80*25; i++) _base[i] = 0; }
    inline void write(uint16_t addr, uint16_t val) { _base[addr] = val; }
    inline void write(uint16_t addr, uint8_t attr, char c) { write(addr, attr << 8 | c); }
    inline void write(Pos pos, uint8_t attr, char c) { write(pos.y * 80 + pos.x, attr, c); }

    inline void write(uint16_t addr, uint8_t attr, const char *s)
    {   for (size_t i = 0; i < strlen(s); i++)
            write(addr + i, attr, s[i]);
    }

    void write(Pos pos, uint8_t attr, const char *s) {   write(pos.y * 80 + pos.x, attr, s); }
    void setAttr(uint16_t addr, uint8_t attr) { _base[addr] = attr << 8 | loByte(_base[addr]); }
    void setAttr(Pos pos, uint8_t attr) { setAttr(pos.y * 80 + pos.x, attr); }
    void setAttrLine(uint8_t y, uint8_t attr);
    void gotoxy(Pos pos) { _pos = pos; gotoxy(pos.x, pos.y); }
    void bs() { gotoxy(--_pos); write(_pos, 0xf, ' '); }
    void Display::setMode(int mode);
    void putc(char c)
    {
        if (c == '\n')
        {
            _pos.newLine();
            return;
        }
        write(_pos, 0xf, c);
        gotoxy(++_pos);
    }
};

class CommandLine
{
    char _buf[255];
    int _bufPos;
public:
    CommandLine() : _bufPos(0) { memset(_buf, 0, sizeof(_buf)); }
    void push(char c) { _buf[_bufPos++] = c; }
    void dump(ostream &os);
    const char *buf() { return _buf; }
    char pop() { char c = _buf[_bufPos]; _buf[_bufPos--] = 0; return c; }
};

class Document
{
    fstream _fs;
public:
    Document() { }
    ~Document() { _fs.close(); }
    int get() { return _fs.get(); }
    void open(const char *s) { _fs.open(s); }
};

class App
{
    const int _argc;
    const char **_argv;
    Display _display;
    CommandLine _cmdLine;
    int _mode;
    void execute(CommandLine &cmdLine);
    bool _quit;
    Document _doc;
public:
    App(const int argc, const char **argv) : _argc(argc), _argv(argv), _quit(false) { }
    int run();
};

class Keys
{
public:
    static const uint8_t BS = 0x08;
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

void App::execute(CommandLine &cmdLine)
{
    if (strcmp(cmdLine.buf(), ":q!") == 0)
        _quit = true;
}

void Display::setMode(int mode)
{
    union REGS regs;
    regs.h.ah = 0;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}

void Display::gotoxy(int x, int y)
{
    union REGS regs;
    regs.h.ah = 2;
    regs.h.bh = 0;
    regs.h.dh = y;
    regs.h.dl = x;
    int86(0x10, &regs, &regs);
}

void CommandLine::dump(ostream &os)
{
    os << _buf;
}

void Display::setAttrLine(uint8_t y, uint8_t attr)
{
    for (int i = 0; i < 80; i++)
        setAttr(Pos(i, y), attr);
}

int App::run()
{
    _mode = 1;
    _display.clear();
    _display.gotoxy(Pos(0, 0));

    if (_argc > 1)
    {
        _doc.open(_argv[1]);
        
        int lines = 0;

        for (int i = 0; i < 900 && lines < 24; i++)
        {
            int c = _doc.get();

            if (c < 0)
                break;

            if (c == '\n')
                lines++;

            _display.putc(c);
        }
    }
    else
    {
        for (int i = 1; i < 24; i++)
            _display.write(Pos(0, i), 0x09, '~');
    }

    _display.gotoxy(Pos(0, 0));
    _display.setAttrLine(0, 0xf);
    uint16_t k;

    while (!_quit && (k = getch()))
    {

        switch (k)
        {
        case Keys::COLON:
            _display.gotoxy(Pos(0,24));
            _display.setAttrLine(24, 0xf);
            _cmdLine.push(':');
            _display.putc(':');
            break;
        case Keys::ENTER:
            execute(_cmdLine);
            break;
        case Keys::BS:
            _cmdLine.pop();
            _display.bs();
            break;
        case Keys::ESC:
            _quit = true;
            break;
        default:
            _cmdLine.push(k);
            _display.putc(k);
            break;
        }
    }

    
    _display.setMode(3);
    return 0;
}

int main(const int argc, const char **argv)
{
    App app(argc, argv);
    return app.run();
}


