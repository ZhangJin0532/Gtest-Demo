#pragma once

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
        LEPT_PARSE_NUMBER_TOO_BIG
    };

    struct lept_value {
        double n;
        lept_type type;
    };

    int lept_parse(lept_value& value, const std::string& json);

    lept_type lept_get_type(const lept_value& v);

}