#pragma once

#include <algorithm>
#include <tuple>


namespace detail {
template<class... Iterators>
class zip_iter {
public:
    using index_seq = std::make_index_sequence<sizeof...(Iterators)>;

    using reference = std::tuple<typename std::iterator_traits<Iterators>::reference ...>;
    using value_type = std::tuple<typename std::iterator_traits<Iterators>::value_type ...>;
    using pointer = typename std::tuple<typename std::iterator_traits<Iterators>::value_type ...>*;
    using difference_type = std::common_type_t<typename std::iterator_traits<Iterators>::difference_type ...>;
    using iterator_category = std::common_type_t<typename std::iterator_traits<Iterators>::iterator_category ...>;

    zip_iter(std::tuple<Iterators...> iters)
        : m_iters(iters)
    {}

    auto& operator++(){
        inc(index_seq(), 1);
        return *this;
    }

    auto operator++(int) {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator+(size_t amount) const {
        auto tmp = *this;
        tmp.inc(index_seq(), amount);
        return tmp;
    }

    auto operator*() -> reference {
        return deref(index_seq());
    }

    auto operator==(const zip_iter& other) const {
        return eq(index_seq(), other);
    }

    auto operator!=(const zip_iter& other) const {
        return !(*this == other);
    }

    
private:
    template<size_t... nums>
    void inc(std::index_sequence<nums...>, size_t amount) {
        auto l = {(std::get<nums>(m_iters) = std::next(std::get<nums>(m_iters), amount), 0)...};
        ((void)l);
    }

    template<size_t... nums>
    void dec(std::index_sequence<nums...>) {
        auto l = {(std::get<nums>(m_iters)--, 0)...};
        ((void)l);
    }

    template<size_t... nums>
    auto deref(std::index_sequence<nums...>) {
        return reference{ *std::get<nums>(m_iters)... };
    }

    template<size_t... nums>
    auto eq(std::index_sequence<nums...>, const zip_iter& other) const {
        auto vals = {
            (std::get<nums>(m_iters) == std::get<nums>(other.m_iters))...
        };
        return std::any_of(vals.begin(), vals.end(), [](auto x){ return x; });
    }

    std::tuple<Iterators...> m_iters;
};

template<class... Iterators>
class zip {
public:
    zip(std::tuple<Iterators...> begin, std::tuple<Iterators...> end)
        : m_begin(begin), m_end(end)
    {}

    auto begin() { return zip_iter(m_begin); }
    auto end() { return zip_iter(m_end); }


    auto begin() const { return zip_iter(m_begin); }
    auto end() const { return zip_iter(m_end); }
private:
    std::tuple<Iterators...> m_begin;
    std::tuple<Iterators...> m_end;
};
}//namespace detail


template<class... Containers>
auto zip(Containers&... containers) {
    return detail::zip(
        std::tuple{ containers.begin()... },
        std::tuple{ containers.end()... }
    );
}