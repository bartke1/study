#include <cstring>
#include <stdint.h>

struct X
{
    char x[4143 * 16 + 1] __attribute__((aligned(16)));

    void crashme()
    {
        std::memset(x, 0, 16);
    }
};

int main()
{
    X* x = new X[2];
    x->crashme();
}
