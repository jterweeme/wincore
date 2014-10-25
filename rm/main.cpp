//#include "filesys.h"
#include "common.h"
#include <stdlib.h>

typedef vector<string> Files;

class Options
{
    bool _verbose;
    bool _force;
    bool _recursive;
    int _argc;
    char **_argv;
    Files _files;
public:
    Options(int argc, char **argv);
    bool verbose() const { return _verbose; }
    bool force() const { return _force; }
    bool recursive() const { return _recursive; }
    Files files() const { return _files; }
    void dump(ostream &os);
    void parse();
};

class App
{
    Options _options;
    //FileSystem _fs;
public:
    App(int argc, char **argv) : _options(argc, argv) { }
    int run();
};

Options::Options(int argc, char **argv)
  :
    _verbose(false), _force(false), _recursive(false), _argc(argc), _argv(argv)
{ }

void Options::dump(ostream &os)
{
    if (_verbose)
        os << "v";
    if (_force)
        os << "f";
    if (_recursive)
        os << "R";
}

void Options::parse()
{
    for (int i = 1; i < _argc; i++)
    {
        if (_argv[i][0] == '-')
        {
            for (int j = 1; _argv[i][j]; j++)
            {
                switch (_argv[i][j])
                {
                case 'v':
                    _verbose = true;
                    break;
                case 'R':
                    _recursive = true;
                    break;
                case 'f':
                    _force = true;
                    break;
                }
            }
        }
        else
        {
            _files.push_back(_argv[i]);
        }
    }
}

int App::run()
{
    _options.parse();
#ifdef DEBUG
    _options.dump(cerr);
    cerr << "\n";
#endif
    
    Files f = _options.files();

    for (Files::iterator it = f.begin(); it != f.end(); it++)
    {
        char cmd[255] = {0};
        sprintf(cmd, "deltree /y %s", it->c_str());
    }

    return 0;
}

int main(int argc, char **argv)
{
    App app(argc, argv);
    return app.run();
}


