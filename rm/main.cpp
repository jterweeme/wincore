#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class Options
{
    bool _verbose;
    bool _force;
    bool _recursive;
    int _argc;
    char **_argv;
    vector<string> _files;
public:
    Options(int argc, char **argv);
    void dump(ostream &os);
    void parse();
};

class App
{
    Options _options;
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
    _options.dump(cout);
    cout << "\n";
    return 0;
}

int main(int argc, char **argv)
{
    App app(argc, argv);
    return app.run();
}


