#include <iostream>
#include <gtest/gtest.h>
#include <utility>
#include <string>

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

TEST(Test_Parse, Parse_Invalid_Value1)
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

TEST(Test_Parse, Parse_Number)
{
    std::pair<double, const std::string> TestCase[] = {
        {0.0, "0"},
        {0.0, "-0"},
        {0.0, "-0.0"},
        {1.0, "1"},
        {-1.0, "-1"},
        {1.5, "1.5"},
        {-1.5, "-1.5"},
        {3.1416, "3.1416"},
        {1E10, "1E10"},
        {1e10, "1e10"},
        {1E+10, "1E+10"},
        {1E-10, "1E-10"},
        {-1E10, "-1E10"},
        {-1e10, "-1e10"},
        {-1E+10, "-1E+10"},
        {-1E-10, "-1E-10"},
        {1.234E+10, "1.234E+10"},
        {1.234E-10, "1.234E-10"},
        {0.0, "1e-10000"}
    };

    lept_value v;
    v.type = lept_type::LEPT_NULL;
    for(auto& p : TestCase) {
        EXPECT_EQ(LEPT_PARSE_OK, lept_parse(v, p.second));
        EXPECT_EQ(lept_type::LEPT_NUMBER, lept_get_type(v));
        EXPECT_EQ(p.first, v.n);
    }
}

TEST(Test_Parse, Parse_Invalid_Value2)
{
    std::pair<int, const std::string> TestCase[] = {
        {LEPT_PARSE_INVALID_VALUE, "+0"},
        {LEPT_PARSE_INVALID_VALUE, "+1"},
        {LEPT_PARSE_INVALID_VALUE, ".123"},
        {LEPT_PARSE_INVALID_VALUE, "1."},
        {LEPT_PARSE_INVALID_VALUE, "INF"},
        {LEPT_PARSE_INVALID_VALUE, "inf"},
        {LEPT_PARSE_INVALID_VALUE, "NAN"},
        {LEPT_PARSE_INVALID_VALUE, "nan"}
    };

    lept_value v;
    v.type = lept_type::LEPT_NULL;
    for(auto& p : TestCase) {
        double d = std::strtod(p.second.c_str(), nullptr);
        std::cout << "---" << d << std::endl;
        EXPECT_EQ(p.first, lept_parse(v, p.second));
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}