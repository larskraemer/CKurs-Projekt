#pragma once

#include <iterator>

namespace detail {
template<class T>
class iota{
public:
    using reference = T;
    using value_type = T;
    using pointer = T*;
    using difference_type = ssize_t;
    using iterator_category = std::input_iterator_tag;

    iota(const T& t) : m_value(t) {}
    iota(T&& t) : m_value(t) {}

    auto begin() { return *this; }
    auto end() { auto ret = *this; ret.m_end = true; return ret; }

    auto& operator++(){
        m_value++;
        return *this;
    }

    auto operator++(int) {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator*() const {
        return m_value;
    }

    auto operator==(const iota& other) const {
        if(m_end && other.m_end) return true;
        if(m_end != other.m_end) return false;
        return m_value == other.m_value;
    }

    auto operator!=(const iota& other) const {
        return !(*this == other);
    }
private:
    T m_value;
    bool m_end = false;
};
} //namespace detail

template<class T>
auto iota(T begin) {
    return detail::iota(begin);
}