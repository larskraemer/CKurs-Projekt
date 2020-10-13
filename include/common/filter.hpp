#pragma once

#include <iterator>


namespace detail{
template<class Iter, class Fn>
class filter_iter{
public:
    using reference = typename std::iterator_traits<Iter>::reference;
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using difference_type = typename std::iterator_traits<Iter>::difference_type;
    using iterator_category = typename std::iterator_traits<Iter>::iterator_category;

    filter_iter(Iter it, Iter end, Fn func)
        : m_iter(it), m_end(end), m_func(func)
    {
        while(m_iter != m_end && !m_func(*m_iter)) m_iter++;
    }

    auto& operator++(){
        m_iter++;
        while(m_iter != m_end && !m_func(*m_iter)) m_iter++;
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
        return *m_iter;
    }

    auto operator*() const {
        return *m_iter;
    }

    auto operator==(const filter_iter& other) const {
        return m_iter == other.m_iter;
    }

    auto operator!=(const filter_iter& other) const {
        return !(*this == other);
    }
private:
    Iter m_iter;
    Iter m_end;
    Fn m_func;
};

template<class It, class Fn>
class filter{
public: 
    filter(It begin, It end, Fn func)
        : m_begin(begin), m_end(end), m_func(func)
    {}

    auto begin() { return filter_iter(m_begin, m_end, m_func); }
    auto end() { return filter_iter(m_end, m_end, m_func); }
private:
    It m_begin;
    It m_end;
    Fn m_func;
};
}//namespace detail

template<class T, class Fn>
auto filter(T& container, Fn f){
    return detail::filter(std::begin(container), std::end(container), f);
}

template<class T, class Fn>
auto filter(const T& container, Fn f){
    return detail::filter(std::begin(container), std::end(container), f);
}