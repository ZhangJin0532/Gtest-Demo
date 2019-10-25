#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include "leptjson/leptjson.h"

#define EXPECT(c, ch) do { assert(*c.json == (ch)); c.json++; } while(0)

namespace leptjson {

struct lept_context {
    const char *json;
};

static void lept_parse_whitespace(lept_context& c) {
    const char *p = c.json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c.json = p;
}

static int lept_parse_number(lept_context& c, lept_value& v) {
    char *end = nullptr;
    v.n = std::strtod(c.json, &end);
    if(c.json == end) {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c.json = end;
    v.type = lept_type::LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

int lept_parse_literal(lept_context& c, lept_value& v, const std::string& l, lept_type t)
{
    if(strncmp(c.json, l.c_str(), l.length())) {
        return LEPT_PARSE_INVALID_VALUE;
    }

    v.type = t;
    c.json += l.length();
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context& c, lept_value& v) {
    switch (*c.json) {
        case 'n':  return lept_parse_literal(c, v, "null", lept_type::LEPT_NULL);
        case 't':  return lept_parse_literal(c, v, "true", lept_type::LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", lept_type::LEPT_FALSE);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return lept_parse_number(c, v);
    }
}

int lept_parse(lept_value& v, const std::string& json)
{
    lept_context c;
    c.json = json.c_str();
    v.type = lept_type::LEPT_NULL;
    lept_parse_whitespace(c);
    auto ret = lept_parse_value(c, v);
    if(ret == LEPT_PARSE_OK) {
        lept_parse_whitespace(c);
        if(*c.json != '\0') {
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value& v) {
    return v.type;
}

double lept_get_number(const lept_value& v) {
    assert(v.type == lept_type::LEPT_NUMBER);
    return v.n;
}

}