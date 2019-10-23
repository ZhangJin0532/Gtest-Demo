#include <iostream>
#include <gtest/gtest.h>

#include "leptjson/leptjson.h"

using namespace leptjson;

TEST(Test_Parse, Parse_Null)
{
    lept_value v;
    v.type = lept_type::LEPT_FALSE;
    EXPECT_EQ(LEPT_PARSE_OK, lept_parse(v, "null"));
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));
}

TEST(Test_Parse, Parse_True)
{
    lept_value v;
    v.type = lept_type::LEPT_NULL;
    EXPECT_EQ(LEPT_PARSE_OK, lept_parse(v, "true"));
    EXPECT_EQ(lept_type::LEPT_TRUE, lept_get_type(v));
}

TEST(Test_Parse, Parse_False)
{
    lept_value v;
    v.type = lept_type::LEPT_NULL;
    EXPECT_EQ(LEPT_PARSE_OK, lept_parse(v, "false"));
    EXPECT_EQ(lept_type::LEPT_FALSE, lept_get_type(v));
}

TEST(Test_Parse, Parse_Expect_Value)
{
    lept_value v;
    v.type = lept_type::LEPT_FALSE;
    EXPECT_EQ(LEPT_PARSE_EXPECT_VALUE, lept_parse(v, ""));
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));

    v.type = lept_type::LEPT_FALSE;
    EXPECT_EQ(LEPT_PARSE_EXPECT_VALUE, lept_parse(v, " "));
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));
}

TEST(Test_Parse, Parse_Invalid_Value)
{
    lept_value v;
    v.type = lept_type::LEPT_FALSE;
    EXPECT_EQ(LEPT_PARSE_INVALID_VALUE, lept_parse(v, "nul"));
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));

    v.type = lept_type::LEPT_FALSE;
    EXPECT_EQ(LEPT_PARSE_INVALID_VALUE, lept_parse(v, "?"));
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));
}

TEST(Test_Parse, Parse_Root_Not_Singular)
{
    lept_value v;
    v.type = lept_type::LEPT_FALSE;
    EXPECT_EQ(LEPT_PARSE_ROOT_NOT_SINGULAR, lept_parse(v, "null x"));
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}