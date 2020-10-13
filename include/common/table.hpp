#pragma once 

#include <iostream>
#include <sstream>
#include <string_view>

#include "util/overloaded.hpp"
#include "util/util.hpp"
#include "util/zip.hpp"


class Table {
public:
    Table(size_t len = 0)
        : m_len{len}
    {}

    void set_len(size_t len) { m_len = len; }
    void set_min_width(size_t min_width) { m_min_field_width = min_width; }


    template<class T>
    friend Table& operator<<(Table& t, const T& x);

    friend std::ostream& operator<<(std::ostream& os, Table& t);
private:
    size_t m_min_field_width = 0;
    size_t m_len;
    std::stringstream fields;
};

template<class T>
Table& operator<<(Table& t, const T& x)
{
    t.fields << x;
    return t;
}