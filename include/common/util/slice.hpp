#pragma once

#include <vector>

namespace detail{
template<class Iter_t>
class slice_t {
public:
    slice_t(Iter_t beg, Iter_t end)
        : m_begin(beg), m_end(end)
    {}

    auto begin(){
        return m_begin;
    }

    auto end() {
        return m_end;
    }

private:
    Iter_t m_begin;
    Iter_t m_end;
};
}//namespace detail

template<class T>
auto slice(T& container, size_t begin, size_t end){
    return detail::slice_t(container.begin() + begin, container.begin() + end);
}

template<class T>
auto slice(const T& container, size_t begin, size_t end){
    return detail::slice_t(container.begin() + begin, container.begin() + end);
}