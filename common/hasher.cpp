#include "hasher.h"

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

uint32_t Chunk::to_int32(const uint8_t * const bytes) const
{
    return (uint32_t) bytes[0]
        | ((uint32_t) bytes[1] << 8)
        | ((uint32_t) bytes[2] << 16)
        | ((uint32_t) bytes[3] << 24);
}


void Hash::read(const char *hash)
{
#if 0
    string s0 = string(hash, hash + 8);
    _h0 = be32toh(strtol(s0.c_str(), 0, 16));
    string s1 = string(hash + 8, hash + 16);
    _h1 = be32toh(strtol(s1.c_str(), 0, 16));
    string s2 = string(hash + 16, hash + 24);
    _h2 = be32toh(strtol(s2.c_str(), 0, 16));
    string s3 = string(hash + 24, hash + 32);
    _h3 = be32toh(strtol(s3.c_str(), 0, 16));
#else
    char s0[255] = {0};
    char s1[255] = {0};
    char s2[255] = {0};
    char s3[255] = {0};
    strncpy(s0, hash, 8);
    strncpy(s1, hash + 8, 8);
    strncpy(s2, hash + 16, 8);
    strncpy(s3, hash + 24, 8);
    _h0 = be32toh(strtol(s0, 0, 16));
    _h1 = be32toh(strtol(s1, 0, 16));
    _h2 = be32toh(strtol(s2, 0, 16));
    _h3 = be32toh(strtol(s3, 0, 16));

#endif
}

void Hash::dump(ostream &os) const
{
    os << hex << setfill('0')
       << setw(8) << be32toh(_h0)
       << setw(8) << be32toh(_h1)
       << setw(8) << be32toh(_h2)
       << setw(8) << be32toh(_h3);
}

void Hasher::stream(istream &file)
{
    _hash.reset();

    for (unsigned i = 0; file; i++)
    {
        uint8_t data[64] = {0}; 
        file.read((char *)data, 64);
        Chunk chunk;

        if (file.gcount() < 56)
        {
            data[file.gcount()] = 0x80;
            chunk.read(data);
            chunk.fillTail(i * 64 + file.gcount());
        }
        else if (file.gcount() < 64)
        {
            data[file.gcount()] = 0x80;
            chunk.read(data);
            Hash foo = chunk.calc(_hash);
            _hash.add(foo);
            chunk.clear();
            chunk.fillTail(i * 64 + file.gcount());
        }
        else
        {
            chunk.read(data);
        }

        Hash foo = chunk.calc(_hash);
        _hash.add(foo);
    }
}

void Hasher::stream(const char *s, size_t size)
{
    _hash.reset();

    for (size_t i = 0; i < size; i+= 64)
    {
        size_t readSize = (size - i) % 64;
        uint8_t data[64] = {0};
        memcpy(data, s + i, readSize);
        Chunk chunk;

        if (readSize < 56)
        {
            data[readSize] = 0x80;
            chunk.read(data);
            chunk.fillTail(size);
        }
        else if (readSize < 64)
        {
            data[readSize] = 0x80;
            chunk.read(data);
            Hash foo = chunk.calc(_hash);
            _hash.add(foo);
            chunk.clear();
            chunk.fillTail(size);
        }
        else
        {
            chunk.read(data);
        }

        Hash foo = chunk.calc(_hash);
        _hash.add(foo);
    }
}

void Chunk::dump(ostream &os) const
{
    for (int i = 0; i < 16; i++)
        os << hex << setw(8) << _w[i] << "\n";
}

Hash Chunk::calc(Hash &hash)
{
    uint32_t a, b, c, d, f, g, temp;
    a = hash.h0();
    b = hash.h1();
    c = hash.h2();
    d = hash.h3();

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
        b = b + LEFTROTATE((a + f + k[i] + w(g)), r[i]);
        a = temp;
    }

    Hash foo(a, b, c, d);
    return foo;

}


