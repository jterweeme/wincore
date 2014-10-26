#include "main.h"

void Paars::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
        it->dump(os);
}

void Paar::dump(ostream &os)
{
    _hash.dump(os);
    os << "  " << _fn << "\n";
}

void Options::parse(const int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'c':
                _check = true;
                break;
            case '\0':
                _cin = true;
                break;
            }
        }
    }
}

void Options::dump(ostream &os)
{
    if (_cin)
        os << "Standard Input\n";
}

void App::checkFile2(const char *fn)
{
    _hasher.reset();
    ifstream file;
    file.open(fn, fstream::in | ios::binary);

    if (!file.is_open())
        throw "Cannot open file";

    _hasher.stream(file);
    file.close();
    _hasher.hash().dump(cout);
    cout << "  " << fn << "\n";
}

void Paar::read(istream &is)
{
    char line[255] = {0};
    is.getline(line, sizeof(line));
}

int App::run()
{
    _options.parse();

    if (_options.cin())
    {
        _hasher.stream(cin);
        cout << _hasher.hash().toString() << "  -\n";
    }

    while (_options.hasNextFileName() && !_options.check())
        checkFile2(_options.nextFileName().c_str());
    
    while (_options.hasNextFileName() && _options.check())
    {
        ifstream foo;
        char line[255] = {0};
        foo.open(_options.nextFileName().c_str(), fstream::in);
        
        while (foo)
        {
            foo.getline(line, sizeof(line));

            if (foo)
            {
                Hash hash(line);
                string fn = string(line + 34);
                Paar paar(hash, fn);
                checkPaar(paar);
            }
        }
        foo.close();
    }

    return 0;
}

void App::checkPaar(Paar &paar)
{
    ifstream file;
    file.open(paar.fn().c_str(), fstream::in | fstream::binary);
    _hasher.stream(file);
    file.close();
    Hash hash2 = _hasher.hash();

    if (paar.hash().isEqual(hash2))
        cout << paar.fn() << ": OK\n";
    else
        cout << paar.fn() << ": FAILED\n";
}

int main(const int argc, char **argv)
{
    App app(argc, argv);
    return app.run();
}


