#include "currency.hpp"

std::ostream& operator<<(std::ostream& os, const currency_t& c) {
    os << c.amount << "$";
    return os;
}

std::istream& operator>>(std::istream& is, currency_t& c) {
    is >> c.amount;
    return is;
}