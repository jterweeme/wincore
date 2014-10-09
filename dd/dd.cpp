#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

class Options
{
    string _if;
    string _of;
public:
    int parse(int argc, char **argv);
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class App
{
    Options _options;
public:
    int run(int argc, char **argv);
};

void Options::dump(ostream &os)
{
    os << "Input file: " << _if;
}

int Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "if", 2) == 0)
            _if = string(argv[i] + 3);

        
    }
    return 0;
}

int App::run(int argc, char **argv)
{
    _options.parse(argc, argv);
    _options.dump(cout);
    cout << "\n";
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


