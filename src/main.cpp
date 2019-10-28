#include <iostream>
#include <gtest/gtest.h>
#include <utility>
#include <string>
#include <cstring>
#include "leptjson/leptjson.h"

using namespace leptjson;

lept_value v;

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
    lept_init(v);
    v.type = lept_type::LEPT_FALSE;
    CheckParseResultAndType(v, "null", LEPT_PARSE_OK, lept_type::LEPT_NULL);
}

TEST(Test_Parse, Parse_True)
{
    lept_init(v);
    CheckParseResultAndType(v, "true", LEPT_PARSE_OK, lept_type::LEPT_TRUE);
}

TEST(Test_Parse, Parse_False)
{
    lept_init(v);
    CheckParseResultAndType(v, "false", LEPT_PARSE_OK, lept_type::LEPT_FALSE);
}

TEST(Test_Parse, Parse_Expect_Value)
{
    lept_init(v);
    v.type = lept_type::LEPT_FALSE;
    CheckParseResultAndType(v, "", LEPT_PARSE_EXPECT_VALUE, lept_type::LEPT_NULL);

    lept_init(v);
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

    for(auto p : TestCase) {
        lept_init(v);
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

    for(auto& p : TestCase) {
        lept_init(v);
        CheckParseResultAndType(v, p.second, LEPT_PARSE_OK, lept_type::LEPT_NUMBER);
        EXPECT_DOUBLE_EQ(p.first, lept_get_number(v));
    }
}

TEST(Test_Parse, Parse_String)
{
    std::pair<const std::string, const std::string> TestCase[] = {
        {"", R"("")"},
        {"hello", R"("hello")"}
    };
    
    for(auto& p : TestCase) {
        lept_init(v);
        CheckParseResultAndType(v, p.second, LEPT_PARSE_OK, lept_type::LEPT_STRING);
        EXPECT_EQ(p.first, lept_get_string(v));
    }

#if 0
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
#endif
}

TEST(Test_Parse, Parse_Too_Big_Value)
{
    std::pair<int, std::string> TestCase[] = {
        {LEPT_PARSE_NUMBER_TOO_BIG, "1e309"},
        {LEPT_PARSE_NUMBER_TOO_BIG, "-1e309"}
    };

    for(auto p : TestCase) {
        lept_init(v);
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

    for(auto& p : TestCase) {
        lept_init(v);
        CheckParseResult(v, p.second, p.first); 
    }
}

TEST(Test_Parse, Parse_Missing_Quotation_Mark)
{
    std::pair<int, std::string> TestCase[] = {
        {LEPT_PARSE_MISS_QUOTATION_MARK, "\""},
        {LEPT_PARSE_MISS_QUOTATION_MARK, "\"abc"}
    };

    for(auto& p : TestCase) {
        lept_init(v);
        CheckParseResult(v, p.second, p.first); 
    }

}

TEST(Test_Parse, Parse_Invalid_String_Escape)
{
#if 0
    TEST_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE, "\"\\v\"");
    TEST_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
    TEST_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
    TEST_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
#endif
}

TEST(Test_Parse, Parse_Invalid_String_Char)
{
#if 0
    TEST_ERROR(LEPT_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
    TEST_ERROR(LEPT_PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
#endif
}

TEST(Test_Access, Access_Null)
{
    lept_init(v);
    lept_set_string(v, "a");
    lept_set_null(v);
    EXPECT_EQ(lept_type::LEPT_NULL, lept_get_type(v));
}

TEST(Test_Access, Access_Boolean)
{
    lept_init(v);
    lept_set_string(v, "a");
    lept_set_boolean(v, true);
    EXPECT_EQ(lept_type::LEPT_TRUE, lept_get_type(v));
    EXPECT_TRUE(lept_get_boolean(v));
    lept_set_boolean(v, false);
    EXPECT_EQ(lept_type::LEPT_FALSE, lept_get_type(v));
    EXPECT_FALSE(lept_get_boolean(v));
}

TEST(Test_Access, Access_Number)
{
    lept_init(v);
    lept_set_string(v, "a");
    lept_set_number(v, 123.456);
    EXPECT_EQ(lept_type::LEPT_NUMBER, lept_get_type(v));
    EXPECT_DOUBLE_EQ(123.456, lept_get_number(v));
}

TEST(Test_Access, Access_String)
{
    lept_init(v);
    lept_set_string(v, "");
    EXPECT_EQ("", lept_get_string(v));
    lept_set_string(v, "Hello");
    EXPECT_EQ("Hello", lept_get_string(v));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}