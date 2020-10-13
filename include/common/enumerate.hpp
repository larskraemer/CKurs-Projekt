#pragma once

#include <iterator>
#include <tuple>

namespace detail {
template<class Iter_t>
class enumerate_iter{
public:
    using reference = std::pair<size_t, typename std::iterator_traits<Iter_t>::reference>;

    enumerate_iter(Iter_t it, size_t n)
        : m_iter(it), m_idx(n)
    {}

    auto& operator++() {
        ++m_iter;
        ++m_idx;
        return *this;
    }

    auto operator++(int){
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator+(size_t add) const {
        auto tmp = *this;
        for(auto i = 0u; i < add; i++) ++tmp;
        return tmp;
    }

    auto operator*() {
        return reference(m_idx, *m_iter);
    }

    auto operator==(const enumerate_iter& o) const {
        return m_iter == o.m_iter;
    }

    auto operator!=(const enumerate_iter& o) const {
        return !(*this == o);
    }
private:
    Iter_t m_iter;
    size_t m_idx;
};

template<class Iter_t>
class enumerator{
public:
    enumerator(Iter_t beg, Iter_t end) 
        : m_begin(beg), m_end(end)
    {}

    auto begin(){
        return enumerate_iter(m_begin, 0);
    }

    auto end(){
        return enumerate_iter(m_end, 0);
    }

    auto begin() const {
        return enumerate_iter(m_begin, 0);
    }

    auto end() const {
        return enumerate_iter(m_end, 0);
    }
private:
    Iter_t m_begin;
    Iter_t m_end;
};
}//namespace detail

template<class T>
auto enumerate(const T& container) {
    return detail::enumerator(container.begin(), container.end());
}