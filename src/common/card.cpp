#include "card.hpp"
#include <ostream>

std::ostream& operator<<(std::ostream& os, Suit s) {
    os << to_string(s);
    return os;
}

std::ostream& operator<<(std::ostream& os, Value v) {
    os << to_string(v);
    return os;
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
    os << c.value();
    os << c.suit();
    return os;
}