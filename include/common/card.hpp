#pragma once
#include <array>
#include <compare>
#include <optional>
#include <ostream>
#include <string_view>

struct Value{
    explicit constexpr Value(int i=-1) : v(i) {}
    constexpr operator int(){return v;}
    int v;
};

enum class Suit:int {
    Hearts, Clubs, Diamonds, Spades, Invalid
};

constexpr auto suits = std::array{
    Suit::Hearts,
    Suit::Clubs,
    Suit::Diamonds,
    Suit::Spades
};

constexpr Value make_value(char c) {
    switch (c)
    {
    case '2': return Value(0);
    case '3': return Value(1);
    case '4': return Value(2);
    case '5': return Value(3);
    case '6': return Value(4);
    case '7': return Value(5);
    case '8': return Value(6);
    case '9': return Value(7);
    case 'T': case 't': return Value(8);
    case 'J': case 'j': return Value(9);
    case 'Q': case 'q': return Value(10);
    case 'K': case 'k': return Value(11);
    case 'A': case 'a': return Value(12);
    default: return Value(-1);
    }
    __builtin_unreachable();
}

constexpr Suit make_suit(char c) {
    switch(c){
        case 'D': case 'd': return Suit::Diamonds;
        case 'H': case 'h': return Suit::Hearts;
        case 'S': case 's': return Suit::Spades;
        case 'C': case 'c': return Suit::Clubs;
        default: return Suit::Invalid;
    }
    __builtin_unreachable();
}

constexpr auto to_string(Suit s) {
    return "HCDS?"[static_cast<std::underlying_type_t<Suit>>(s)];
}

constexpr auto to_string(Value v) {
    if(v.v < 0) return '?';
    return "23456789TJQKA"[v.v];
}

constexpr Suit operator"" _s(char c){ return make_suit(c); }
constexpr Value operator"" _v(char c){ return make_value(c); }

struct Card{
    constexpr explicit Card(int v = -1) : val(v) {}
    constexpr Card(Suit s, Value v)
        : val(0)
    {
        if(s == Suit::Invalid || v < 0){
            val = -1;
        }
        else{
            val = static_cast<std::underlying_type_t<Suit>>(s) + 4 * static_cast<int>(v);
        }
    }

    constexpr Suit suit() const {
        return static_cast<Suit>((val >= 0) ? (val % 4) : 5);
    }

    constexpr Value value() const {
        return Value((val >= 0) ? (val / 4) : -1);
    }

    constexpr auto operator==(const Card& other) const {
        return val == other.val;
    }

    constexpr auto operator!=(const Card& other) const {
        return !(val == other.val);
    }

    constexpr auto operator<(const Card& other) const {
        return val < other.val;
    }

    constexpr auto operator>(const Card& other) const {
        return !((*this == other) || (*this < other));
    }

    constexpr auto operator<=(const Card& other) const {
        return (*this == other) || (*this < other);
    }

    constexpr auto operator>=(const Card& other) const {
        return !(*this < other);
    }

    int val;
};

constexpr Card operator"" _c(const char* sv, size_t){
    return Card(make_suit(sv[1]), make_value(sv[0]));
}

std::ostream& operator<<(std::ostream& os, Suit s);
std::ostream& operator<<(std::ostream& os, Value v);
std::ostream& operator<<(std::ostream& os, const Card& c);