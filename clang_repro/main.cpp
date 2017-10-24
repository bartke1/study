// Copyright 2017 Nokia. All rights reserved.

#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <gtest/gtest.h>

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
            std::memset(&(theData[0]), 0, sizeof(theData));
        }
    };

    AA aa[2];
};

struct T : public testing::Test
{
    A x;
    virtual void TestBody(){};
    static ::testing::TestInfo* const test_info_ __attribute__((unused));
};

::testing::TestInfo* const T::test_info_ =
    ::testing::internal::MakeAndRegisterTestInfo(
        new ::testing::internal::TestFactoryImpl<T>);

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return testing::UnitTest::GetInstance()->Run();
}
