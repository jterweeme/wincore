#include <stdint.h>
#include <string.h>

class Display
{
    uint16_t * const _base;
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

};

class App
{
    const int _argc;
    const char **_argv;
    Display _display;
public:
    App(const int argc, const char **argv) : _argc(argc), _argv(argv) { }
    int run();
};

int App::run()
{
    _display.clear();
    return 0;
}

int main(const int argc, const char **argv)
{
    App app(argc, argv);
    return app.run();
}


