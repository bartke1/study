#include <cstring>

struct Z
{
    char x[16];
    char y[69160] __attribute__((aligned(16)));

    Z()
    {
        std::memset(x, 0, 16);
    }
};

int main()
{
    new Z[2];
}
