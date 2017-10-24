#include <cstdio>
#include <cstring>
#include <stdint.h>

struct X
{
    uint16_t     a;
    uint16_t     b;
    uint8_t      c;
    uint8_t      d;
    uint8_t      e[4];
    uint16_t     f;
    uint8_t      g;
    uint8_t      h[2];
    uint16_t     i;
    uint8_t      j;
    uint8_t      k;
    uint8_t      l;
    uint8_t      m;
    uint8_t      n[2];
    uint8_t      o;
    uint8_t      p;
    bool         r;
};

struct Y
{
    struct Z
    {
        char x[16];
        X otherData[10][247] __attribute__((aligned(16)));

        Z()
        {
            std::memset(x, 0, 16);
        }
    };

    Z aa[2];
};

int main()
{
    new Y;
}
