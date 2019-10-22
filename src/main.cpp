#include <iostream>
#include <gtest/gtest.h>

#include "leptjson/leptjson.h"

TEST(JSONTest, BasicTest)
{
    EXPECT_EQ(4, test_json(2));
    EXPECT_EQ(1, test_json(-1));
    EXPECT_EQ(0, test_json(0));
    EXPECT_EQ(25, test_json(-5));
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}