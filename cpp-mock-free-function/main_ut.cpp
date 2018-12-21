#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sut.hpp"
#include "foo_library_mock.hpp"
#include "function_mocks.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class SutTest : public ::testing::Test {
};

PREPARE_FMOCK(foo, void);
PREPARE_FMOCK(foo, void, float);
PREPARE_FMOCK(foo, void, int, int);
PREPARE_FMOCK(bar3, void, int, int, int);
PREPARE_FMOCK(bar4, void, int, int, int, int);

TEST_F(SutTest, Dupa)
{
    FMOCK_T(foo, void, int) x;
    EXPECT_CALL(x, foo(3)).Times(2);
    sut(2, 3);

    FMOCK_T(foo, void, float) y;
    FMOCK_T(foo, void) z;
    FMOCK_T(foo, void, int, int) v;
    FMOCK_T(bar3, void, int, int, int) b3;
    FMOCK_T(bar4, void, int, int, int, int) b4;
    EXPECT_CALL(y, foo(2.1f)).Times(1);
    foo(2.1f);
    EXPECT_CALL(z, foo()).Times(1);
    foo();
    EXPECT_CALL(v, foo(7, 8)).Times(1);
    foo(7,8);
    EXPECT_CALL(b3, bar3(1, 2, 3)).Times(1);
    bar3(1, 2, 3);
    EXPECT_CALL(b4, bar4(1, 2, 3, 4)).Times(1);
    bar4(1, 2, 3, 4);
}
