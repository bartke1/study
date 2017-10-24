#include "B.hpp"

#include <cstring>

class A
{
public:
    struct AA
    {
        float   theData[12];
        B       otherData[10] __attribute__((aligned(16)));

        AA()
        {
            std::memset(&(theData[0]), 0, sizeof(theData));
        }
    };

    AA aa[2];
};
