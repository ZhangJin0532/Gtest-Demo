#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <sstream>
#include "leptjson/leptjson.h"

#define EXPECT(c, ch) do { assert(*c.json == (ch)); c.json++; } while(0)

namespace leptjson {

struct lept_context {
    const char *json;
};

void lept_free(lept_value& v) {
    if(v.type == lept_type::LEPT_STRING ||
        v.type == lept_type::LEPT_OBJECT ||
        v.type == lept_type::LEPT_ARRAY) {
            v.v.clear();
            v.v.shrink_to_fit();
    }
    v.type = lept_type::LEPT_NULL;
}

static void lept_parse_whitespace(lept_context& c) {
    const char *p = c.json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c.json = p;
}

static int lept_parse_number(lept_context& c, lept_value& v) {
    const char * p = c.json;
    if(*p == '-') {
        p++;
    }
    if(*p == '0') {
        p++;
    } else {
        if(!std::isdigit(*p)) {
            return LEPT_PARSE_INVALID_VALUE;
        }
        while(std::isdigit(*p)) {
            p++;
        }
    }
    if(*p == '.') {
        p++;
        if(!std::isdigit(*p)) {
            return LEPT_PARSE_INVALID_VALUE;
        }
        while(std::isdigit(*p)) {
            p++;
        }
    }
    if(*p == 'e' || *p == 'E') {
        p++;
        if(*p == '+' || *p == '-') {
            p++;
        }
        if(!std::isdigit(*p)) {
            return LEPT_PARSE_INVALID_VALUE;
        }
        while(std::isdigit(*p)) {
            p++;
        }
    }

    errno = 0;
    v.n = strtod(c.json, nullptr);
    if (errno == ERANGE && (v.n == HUGE_VAL || v.n == -HUGE_VAL)) {
        return LEPT_PARSE_NUMBER_TOO_BIG;
    }
    v.type = lept_type::LEPT_NUMBER;
    c.json = p;
    return LEPT_PARSE_OK;
}

static int lept_parse_literal(lept_context& c, lept_value& v, const std::string& l, lept_type t)
{
    if(strncmp(c.json, l.c_str(), l.length())) {
        return LEPT_PARSE_INVALID_VALUE;
    }

    v.type = t;
    c.json += l.length();
    return LEPT_PARSE_OK;
}

static int lept_parse_string(lept_context& c, lept_value& v) {
    EXPECT(c, '\"');
    std::ostringstream oss;
    while(true) {
        switch (*c.json) {
            case '\"':
                lept_set_string(v, oss.str());
                v.type = lept_type::LEPT_STRING;
                c.json++;
                return LEPT_PARSE_OK;
            case '\0':
                return LEPT_PARSE_MISS_QUOTATION_MARK;
            default:
                oss << *c.json;
        }
        c.json++;
    }
}

static int lept_parse_value(lept_context& c, lept_value& v) {
    switch (*c.json) {
        case 'n':  return lept_parse_literal(c, v, "null", lept_type::LEPT_NULL);
        case 't':  return lept_parse_literal(c, v, "true", lept_type::LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", lept_type::LEPT_FALSE);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        case '\"': return lept_parse_string(c, v);
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

void lept_init(lept_value& v)
{
    lept_free(v);
}

void lept_set_null(lept_value& v)
{
    lept_free(v);
}

lept_type lept_get_type(const lept_value& v)
{
    return v.type;
}

double lept_get_number(const lept_value& v)
{
    assert(v.type == lept_type::LEPT_NUMBER);
    return v.n;
}

void lept_set_number(lept_value& v, double n)
{
    lept_free(v);
    v.n = n;
    v.type = lept_type::LEPT_NUMBER;
}

bool lept_get_boolean(const lept_value& v)
{
    assert(v.type == lept_type::LEPT_TRUE || v.type == lept_type::LEPT_FALSE);
    return (v.type == lept_type::LEPT_TRUE) ? true : false;
}

void lept_set_boolean(lept_value& v, bool b)
{
    lept_free(v);
    v.type = b ? lept_type::LEPT_TRUE : lept_type::LEPT_FALSE;
}

std::string lept_get_string(const lept_value& v)
{
    assert(v.type == lept_type::LEPT_STRING);
    return v.v.at(0);
}

void lept_set_string(lept_value& v, const std::string& s)
{
    lept_free(v);
    v.v.push_back(s);
    v.type = lept_type::LEPT_STRING;
}

}