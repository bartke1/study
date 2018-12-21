#include "sut.hpp"
#include "foo.hpp"

void sut(int c, int x)
{
    for (int i=0; i<c; i++)
        foo(x);
}
