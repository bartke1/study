#include <cstdio>
#include <cstring>
#include <stdint.h>

struct C
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

struct B
{
    C               c[246];
};

class A
{
public:
    struct AA
    {
        float   theData[12];
        B       otherData[10] __attribute__((aligned(16)));

        AA()
        {
            printf("pre\n");
            std::memset(&(theData[0]), 0, sizeof(theData));
            printf("post\n");
        }
    };

    AA aa[2];
};

struct T
{
    A x;
};

int main()
{
    new T;
}
