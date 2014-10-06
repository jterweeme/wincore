#include "common.h"
#include <stdlib.h>

const uint32_t k[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

class Util
{
public:
    void to_bytes(uint32_t val, uint8_t *bytes);
    uint32_t to_int32(const uint8_t *bytes);
};

class Hash
{
    uint32_t _h0, _h1, _h2, _h3;
public:
    void read(const char *hash);
    Hash(const char *hash) { read(hash); }
    void reset() { _h0 = 0x67452301; _h1 = 0xefcdab89; _h2 = 0x98badcfe; _h3 = 0x10325476; }
    Hash() { reset(); }
    uint32_t h0() const { return _h0; }
    uint32_t h1() const { return _h1; }
    uint32_t h2() const { return _h2; }
    uint32_t h3() const { return _h3; }
    void add(Hash &h) { _h0 += h.h0(); _h1 += h.h1(); _h2 += h.h2(); _h3 += h.h3(); }
    string toString();
    void dump(ostream &os);

    Hash(uint32_t h0, uint32_t h1, uint32_t h2, uint32_t h3)
      : _h0(h0), _h1(h1), _h2(h2), _h3(h3)
    { }
};

typedef vector<string> Files;

class Options
{
    bool _check;
    Files _files;
public:
    Options() : _check(false) { }
    bool check() const { return _check; }
    Files &files() { return _files; }
    void parse(int argc, char **argv);
};

class Paar
{
    Hash _hash;
    string _fn;
public:
    Paar() { }
    Paar(Hash &hash, string &fn) : _hash(hash), _fn(fn) { }
    void read(istream &is);
    void dump(ostream &os);
};

class Paars : public vector<Paar>
{
public:
    void dump(ostream &os);
    void check();
};

class App
{
    Hash _hash;
    Options _options;
    Paars _paars;
    void md5(uint8_t *msg, size_t new_len);
    uint32_t const LEFTROTATE(uint32_t x, uint32_t c) { return x << c | x >> 32 - c; }
    void checkFile(const char *fn);
public:
    int run(int argc, char **argv);
};

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

void Hash::read(const char *hash)
{
    string s0 = string(hash, hash + 8);
    //_h0 = be32toh(stoul(s0, 0, 16));
    _h0 = be32toh(strtol(s0.c_str(), 0, 16));
    string s1 = string(hash + 8, hash + 16);
    //_h1 = be32toh(stoul(s1, 0, 16));
    _h1 = be32toh(strtol(s1.c_str(), 0, 16));
    string s2 = string(hash + 16, hash + 24);
    //_h2 = be32toh(stoul(s2, 0, 16));
    _h2 = be32toh(strtol(s2.c_str(), 0, 16));
    string s3 = string(hash + 24, hash + 32);
    //_h3 = be32toh(stoul(s3, 0, 16));
    _h3 = be32toh(strtol(s3.c_str(), 0, 16));
}

void App::md5(uint8_t *msg, size_t new_len)
{
    Util util;

    uint32_t w[16];
    uint32_t a, b, c, d, f, g, temp;

    for(size_t offset=0; offset<new_len; offset += 64)
    {
       for (int i = 0; i < 16; i++)
            w[i] = util.to_int32(msg + offset + i*4);

        a = _hash.h0();
        b = _hash.h1();
        c = _hash.h2();
        d = _hash.h3();

        for(int i = 0; i<64; i++)
        {
            if (i < 16)
            {
                f = (b & c) | ((~b) & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;
            }
            else
            {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
        }

        Hash foo(a, b, c, d);
        _hash.add(foo);
    }
}

void Hash::dump(ostream &os)
{
    os << hex << setfill('0')
       << setw(8) << be32toh(_h0)
       << setw(8) << be32toh(_h1)
       << setw(8) << be32toh(_h2)
       << setw(8) << be32toh(_h3);
}

string Hash::toString()
{
    ostringstream oss;
    dump(oss);
    return oss.str();
}

void Util::to_bytes(const uint32_t val, uint8_t *bytes)
{
    bytes[0] = (uint8_t) val;
    bytes[1] = (uint8_t) (val >> 8);
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}

uint32_t Util::to_int32(const uint8_t * const bytes)
{
    return (uint32_t) bytes[0]
        | ((uint32_t) bytes[1] << 8)
        | ((uint32_t) bytes[2] << 16)
        | ((uint32_t) bytes[3] << 24);
}



void Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        char *opt = argv[i];

        if (opt[0] == '-')
        {
            switch (opt[1])
            {
            case 'c':
                _check = true;
                break;
            }
        }
        else
        {
            _files.push_back(string(opt));
        }
    }
}

void App::checkFile(const char *fn)
{
    Util util;
    _hash.reset();
    ifstream foo;
    foo.open(fn, fstream::in | ios::binary | ios::ate);
    size_t fileSize = foo.tellg();
    foo.seekg(0, foo.beg);
    char *file = new char[fileSize];
    foo.readsome(file, fileSize);
    size_t new_len;
    for (new_len = fileSize + 1; new_len % 64 != 56; new_len++);
    uint8_t *msg = new uint8_t[new_len + 8];
    memset(msg, 0, new_len + 8);
    memcpy(msg, file, fileSize);
    msg[fileSize] = 0x80;
    util.to_bytes(fileSize *8, msg + new_len);
    util.to_bytes(fileSize>>29, msg + new_len + 4);
    md5(msg, new_len);
    delete[] msg;
    delete[] file;
    foo.close();
    cout << _hash.toString() << "  " << fn << "\n";
}

void Paar::read(istream &is)
{
    char line[255] = {0};
    is.getline(line, sizeof(line));
    
}

int App::run(int argc, char **argv)
{
    _options.parse(argc, argv);
    Files files = _options.files();

    for (Files::iterator it = files.begin(); it != files.end() && !_options.check(); it++)
        checkFile(it->c_str());
    
    for (Files::iterator it = files.begin(); it != files.end() && _options.check(); it++)
    {
        ifstream foo;
        char line[255] = {0};
        foo.open(it->c_str(), fstream::in);
        
        while (foo)
        {
            foo.getline(line, sizeof(line));

            if (foo)
            {
                Hash hash(line);
                string fn = string(line + 34);
                Paar paar(hash, fn);
                _paars.push_back(paar);
            }
        }
        foo.close();
    }

    _paars.dump(cout);

    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


