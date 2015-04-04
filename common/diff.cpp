#include "common.h"

typedef vector<string> Vest;

class Options
{
    bool _s = false;
    Vest _filenames;
public:
    Options() { }
    void parse(int argc, char **argv);
    Options(int argc, char **argv) { parse(argc, argv); }
    Vest filenames() const { return _filenames; }
};

class App
{
public:
    istream *getStream(string fn) const
    { return fn.compare("-") == 0 ? (istream *)&cin : new ifstream(fn.c_str()); }
    bool diff(istream &is1, istream &is2) const;
    int run(int argc, char **argv);
};

bool App::diff(istream &is1, istream &is2) const
{
    while (is1 && is2)
        if (is1.get() != is2.get())
            return false;

    return true;
}

void Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && strlen(argv[i]) > 1)
        {
            switch (argv[i][1])
            {
            case 's':
                _s = true;
                break;
            }
        }
        else
        {
            _filenames.push_back(argv[i]);
        }
    }
}

int App::run(int argc, char **argv)
{
    Options o(argc, argv);
#if 1
    istream *ifs1 = getStream(o.filenames().front());
    istream *ifs2 = getStream(o.filenames().back());
#else
    istream *ifs1 = getStream(o.filenames().at(0));
    istream *ifs2 = getStream(o.filenames().at(1));
#endif

    if (diff(*ifs1, *ifs2))
    {
#if 0
        cout << "OK\n";
#else
        cout << "\e[1;32mFiles " << o.filenames().front() << " and " << o.filenames().back()
             << " are identical\e[0m\n";
#endif
    }
    else
    {
        cout << "Fout\n";
        return 1;
    }
    
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


