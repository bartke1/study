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

TEST_F(SutTest, Dupa)
{
    FMOCK_T(foo, void, int) x;
    EXPECT_CALL(x, foo(3)).Times(2);
    sut(2, 3);

    FMOCK_T(foo, void, float) y;
    FMOCK_T(foo, void) z;
    FMOCK_T(foo, void, int, int) v;
    EXPECT_CALL(y, foo(2.1f)).Times(1);
    foo(2.1f);
    EXPECT_CALL(z, foo()).Times(1);
    foo();
    EXPECT_CALL(v, foo(7,8)).Times(1);
    foo(7,8);
}
