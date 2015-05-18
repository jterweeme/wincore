/*
Jasper ter Weeme

TODO: bug: when last read equals 16, another read will be attempted
and another empty line will be displayed.
*/

#include "od.h"

void OptionsOd::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (unsigned j = 1; j < strlen(argv[i]); j++)
            {
#if 0
                switch (argv[i][j])
                {
                }
#endif
            }
        }
        else
        {
            _fn = string(argv[i]);
        }
    }

    if (_fn.size() <= 0)
        _cin = true;
}

int AppOd::run(int argc, char **argv)
{
    OptionsOd o;
    HexStream hs;
    o.parse(argc, argv);
    
    if (o.cin())
        hs.dump(cin);

    if (o.fn().size() >= 1)
    {
        ifstream s(o.fn().c_str(), ifstream::in | ifstream::binary);
        hs.dump(s);
    }

    return 0;
}

void HexStream::dump(istream &input)
{
    uint8_t arr[16] = { 0 };
    int pos = 0;

    while (input)
    {
        pos = input.tellg();
        cout << oct << setw(7) << setfill('0') << pos << " ";
        input.read((char *)arr, 16);
        pos += input.gcount();

        for (int i = 0; i < input.gcount(); i++)
            cout << hex << setw(2) << setfill('0') << (int)arr[i] << " ";
        
        for (int i = input.gcount(); i < 16; i++)
            cout << "   ";

        cout << " >";

        for (int i = 0; i < input.gcount(); i++)
        {
            if (isprint(arr[i]))
                cout << arr[i];
            else
                cout << ".";
        }

        cout << "<\n";
    }

    cout << oct << setw(7) << setfill('0') << pos << "\n";
}


