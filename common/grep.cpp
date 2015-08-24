#include "common.h"

class GrepOptions
{
    bool _caseInsensitive = false;
    string _expressie;
    string _file;
public:
    bool cin() const { return _file.size() < 2; }
    bool caseInsensitive() const { return _caseInsensitive; }
    int parse(int argc, const char **argv);
    void dump(ostream &os) const;
    string expressie() const { return _expressie; }
    string file() const { return _file; }
    string toString() const { ostringstream oss; dump(oss); return oss.str(); }
};

class AppGrep
{
    void grep(istream &is, const regex rx);
    void grep(istream &is, const char *rgx) { regex rx(rgx); grep(is, rx); }
    void igrep(istream &is, const char *rgx)
    { regex rx(rgx, regex_constants::icase); grep(is, rx); }
public:
    int run(int argc, const char **argv);
};

void AppGrep::grep(istream &is, const regex rx)
{
    while (is)
    {
        char line[255] = {0};
        is.getline(line, sizeof(line));

        if (regex_search(line, rx))
            cout << line << "\n";
    }
}

void GrepOptions::dump(ostream &os) const
{
    if (_caseInsensitive)
        os << "Case Insensitive\n";

    os << _expressie << "\n" << _file << "\n";
}

int GrepOptions::parse(int argc, const char **argv)
{
    if (argc < 2)
        return -1;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (uint32_t j = 1; j < strlen(argv[i]); j++)
            {
                switch (argv[i][j])
                {
                case 'i':
                    _caseInsensitive = true;
                    break;
                }
            }
        }
        else
        {
            if (_expressie.size() < 2)
                _expressie = string(argv[i]);
            else
                _file = string(argv[i]);
        }
    }

    return 0;
}

int AppGrep::run(int argc, const char **argv)
{
    GrepOptions o;
    istream *i;
    ifstream ifs;

    if (o.parse(argc, argv) < 0)
        throw "Usage\n";

    if (o.cin())
    {
        i = &cin;
    }
    else
    {
        ifs.open(o.file().c_str());
        i = &ifs;
    }

    if (o.caseInsensitive())
        igrep(*i, o.expressie().c_str());
    else
        grep(*i, o.expressie().c_str());

    ifs.close();
    return 0;
}

int main(const int argc, const char **argv)
{
    AppGrep app;
    int ret = -1;

    try
    {
        ret = app.run(argc, argv);
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }

    return ret;
}


