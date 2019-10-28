#pragma once

#include <vector>
#include <string>

namespace leptjson {
    enum class lept_type {
        LEPT_NULL,
        LEPT_FALSE,
        LEPT_TRUE,
        LEPT_NUMBER,
        LEPT_STRING,
        LEPT_ARRAY,
        LEPT_OBJECT
    };

    enum {
        LEPT_PARSE_OK = 0,
        LEPT_PARSE_EXPECT_VALUE,
        LEPT_PARSE_INVALID_VALUE,
        LEPT_PARSE_ROOT_NOT_SINGULAR,
        LEPT_PARSE_NUMBER_TOO_BIG,
        LEPT_PARSE_MISS_QUOTATION_MARK,
        LEPT_PARSE_INVALID_STRING_ESCAPE,
        LEPT_PARSE_INVALID_STRING_CHAR
    };

    struct lept_value {
        double n;
        std::vector<std::string> v;
        lept_type type;
    };

    int lept_parse(lept_value& value, const std::string& json);

    void lept_init(lept_value& v);

    void lept_set_null(lept_value& v);

    lept_type lept_get_type(const lept_value& v);

    double lept_get_number(const lept_value& v);

    void lept_set_number(lept_value& v, double n);

    bool lept_get_boolean(const lept_value& v);

    void lept_set_boolean(lept_value& v, bool b);

    std::string lept_get_string(const lept_value& v);

    void lept_set_string(lept_value& v, const std::string& s);

}