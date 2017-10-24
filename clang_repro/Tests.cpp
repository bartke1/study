// Copyright 2017 Nokia. All rights reserved.

#include <gtest/gtest.h>

#include "A.hpp"

struct ATest : public testing::Test
{
    A x;
};

class ATest_TheTest_Test : public ATest
{
public:
    ATest_TheTest_Test() {}
private:
    virtual void TestBody();
    static ::testing::TestInfo* const test_info_ __attribute__((unused));
    ATest_TheTest_Test(ATest_TheTest_Test const&);
    void operator=(ATest_TheTest_Test const&);
};
::testing::TestInfo* const ATest_TheTest_Test::test_info_ =
    ::testing::internal::MakeAndRegisterTestInfo(
        "ATest", "TheTest", __null, __null, (::testing::internal::GetTypeId<ATest>()),
        ATest::SetUpTestCase, ATest::TearDownTestCase,
        new ::testing::internal::TestFactoryImpl<ATest_TheTest_Test>);
void ATest_TheTest_Test::TestBody() {}
