#include <iostream>
#include <gtest/gtest.h>
#include <utility>
#include <string>

#include "leptjson/leptjson.h"

using namespace leptjson;

void CheckParseResultAndType(lept_value& v, const std::string& data, const int parse_result, const lept_type t)
{
    EXPECT_EQ(parse_result, lept_parse(v, data));
    EXPECT_EQ(t, lept_get_type(v));
}

void CheckParseResult(lept_value& v, const std::string& data, const int parse_result)
{
    EXPECT_EQ(parse_result, lept_parse(v, data));
}

TEST(Test_Parse, Parse_Null)
{
    lept_value v;
    v.type = lept_type::LEPT_FALSE;
    CheckParseResultAndType(v, "null", LEPT_PARSE_OK, lept_type::LEPT_NULL);
}

TEST(Test_Parse, Parse_True)
{
    lept_value v;
    v.type = lept_type::LEPT_NULL;
    CheckParseResultAndType(v, "true", LEPT_PARSE_OK, lept_type::LEPT_TRUE);
}

TEST(Test_Parse, Parse_False)
{
    lept_value v;
    v.type = lept_type::LEPT_NULL;
    CheckParseResultAndType(v, "false", LEPT_PARSE_OK, lept_type::LEPT_FALSE);
}

TEST(Test_Parse, Parse_Expect_Value)
{
    lept_value v;
    v.type = lept_type::LEPT_FALSE;
    CheckParseResultAndType(v, "", LEPT_PARSE_EXPECT_VALUE, lept_type::LEPT_NULL);

    v.type = lept_type::LEPT_FALSE;
    CheckParseResultAndType(v, " ", LEPT_PARSE_EXPECT_VALUE, lept_type::LEPT_NULL);
}

TEST(Test_Parse, Parse_Root_Not_Singular)
{
    std::pair<int, const std::string> TestCase[] = {
        { LEPT_PARSE_ROOT_NOT_SINGULAR, "0123"},
        { LEPT_PARSE_ROOT_NOT_SINGULAR, "0x0"},
        { LEPT_PARSE_ROOT_NOT_SINGULAR, "0x123"},
        { LEPT_PARSE_ROOT_NOT_SINGULAR, "null x"}
    };

    lept_value v;
    for(auto p : TestCase) {
        CheckParseResult(v, p.second, p.first);
    }
}

TEST(Test_Parse, Parse_Number)
{
    std::pair<double, const std::string> TestCase[] = {
        { 0.0, "0"},
        { 0.0, "-0"},
        { 0.0, "-0.0"},
        { 1.0, "1"},
        {-1.0, "-1"},
        { 1.5, "1.5"},
        {-1.5, "-1.5"},
        { 3.1416, "3.1416"},
        { 1E10, "1E10"},
        { 1e10, "1e10"},
        { 1E+10, "1E+10"},
        { 1E-10, "1E-10"},
        {-1E10, "-1E10"},
        {-1e10, "-1e10"},
        {-1E+10, "-1E+10"},
        {-1E-10, "-1E-10"},
        { 1.234E+10, "1.234E+10"},
        { 1.234E-10, "1.234E-10"},
        { 0.0, "1e-10000"},
        { 1.0000000000000002, "1.0000000000000002"},
        { 4.9406564584124654e-324, "4.9406564584124654e-324"},
        {-4.9406564584124654e-324, "-4.9406564584124654e-324"},
        { 2.2250738585072009e-308, "2.2250738585072009e-308"},
        {-2.2250738585072009e-308, "-2.2250738585072009e-308"},
        { 2.2250738585072014e-308, "2.2250738585072014e-308"},
        {-2.2250738585072014e-308, "-2.2250738585072014e-308"},
        { 1.7976931348623157e+308, "1.7976931348623157e+308"},
        {-1.7976931348623157e+308, "-1.7976931348623157e+308"},
    };

    lept_value v;
    v.type = lept_type::LEPT_NULL;
    for(auto& p : TestCase) {
        CheckParseResultAndType(v, p.second, LEPT_PARSE_OK, lept_type::LEPT_NUMBER);        
        EXPECT_EQ(p.first, v.n);
    }
}

TEST(Test_Parse, Parse_Too_Big_Value)
{
    std::pair<int, std::string> TestCase[] = {
        {LEPT_PARSE_NUMBER_TOO_BIG, "1e309"},
        {LEPT_PARSE_NUMBER_TOO_BIG, "-1e309"}
    };

    lept_value v;
    for(auto p : TestCase) {
        CheckParseResult(v, p.second, p.first); 
    }
}

TEST(Test_Parse, Parse_Invalid_Value)
{
    std::pair<int, const std::string> TestCase[] = {
        {LEPT_PARSE_INVALID_VALUE, "nul"},
        {LEPT_PARSE_INVALID_VALUE, "?"},
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
    for(auto& p : TestCase) {
        CheckParseResult(v, p.second, p.first); 
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}