#pragma once
#include <iostream>
#include <limits>

struct currency_t {
    constexpr explicit currency_t(unsigned long long int _amount = 0) : amount(_amount) {}
    unsigned long long int amount;

    auto operator+=(const currency_t& other){ amount += other.amount; }
    auto operator-=(const currency_t& other){ amount -= other.amount; }

    static constexpr auto max() { return currency_t{std::numeric_limits<unsigned long long int>::max()}; }
};

constexpr auto operator-(currency_t lhs, currency_t rhs) {
    return currency_t{ lhs.amount - rhs.amount };
}

constexpr auto operator+(currency_t lhs, currency_t rhs) {
    return currency_t{ lhs.amount + rhs.amount };
}

constexpr auto operator/(currency_t lhs, size_t rhs) {
    return currency_t{ lhs.amount / rhs };
}

constexpr auto operator*(int lhs, currency_t rhs) { return currency_t{ lhs * rhs.amount }; }

constexpr bool operator<(currency_t lhs, currency_t rhs) { return lhs.amount < rhs.amount; }
constexpr bool operator>(currency_t lhs, currency_t rhs) { return lhs.amount > rhs.amount; }
constexpr bool operator<=(currency_t lhs, currency_t rhs) { return lhs.amount <= rhs.amount; }
constexpr bool operator>=(currency_t lhs, currency_t rhs) { return lhs.amount >= rhs.amount; }

constexpr currency_t operator""_ct(unsigned long long int v) {
    return currency_t{v};
}

std::ostream& operator<<(std::ostream& os, const currency_t& c);
std::istream& operator>>(std::istream& is, currency_t& c);