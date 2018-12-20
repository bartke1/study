#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sut.hpp"
#include "foo_library_mock.hpp"
#include "function_mocks.hpp"

DECLARE_FMOCK(foo, void, int);
DEFINE_FMOCK(foo);

DECLARE_FMOCK(foo, void, float);
DEFINE_FMOCK(foo);

//MOCK_METHOD1(foo, void(), dupa);

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class SutTest : public ::testing::Test {
};



TEST_F(SutTest, Dupa)
{
    FMOCK(foo) x;
    EXPECT_CALL(x, foo(3)).Times(2);
    sut(2, 3);
//    bar();
}
