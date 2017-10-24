#include <cstring>

struct X
{
    char x[66289] __attribute__((aligned(16)));

    X()
    {
        std::memset(x, 0, 16);
    }
};

int main()
{
    new X[2];
}
