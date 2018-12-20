#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sut.hpp"
#include "foo_library_mock.hpp"
#include "function_mocks.hpp"

DECLARE_FMOCK(foo, void, int);
DEFINE_FMOCK(foo, void, int);

DECLARE_FMOCK(foo, void, float);
DEFINE_FMOCK(foo, void, float);

DECLARE_FMOCK(foo, void);
DEFINE_FMOCK(foo, void);

//DECLARE_FMOCK(foo, void, int, int);
//DEFINE_FMOCK(foo, void);


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class SutTest : public ::testing::Test {
};



TEST_F(SutTest, Dupa)
{
    FMOCK_T(foo, void, int) x;
    FMOCK_T(foo, void, float) y;
    FMOCK_T(foo, void) z;
    EXPECT_CALL(x, foo(3)).Times(2);
    sut(2, 3);
    EXPECT_CALL(y, foo(2.1f)).Times(1);
    foo(2.1f);
    EXPECT_CALL(z, foo()).Times(1);
    foo();
}
