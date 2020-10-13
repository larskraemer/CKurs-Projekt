#pragma once
#include <array>
#include <compare>
#include <ostream>
#include <string_view>

#include "common/card.hpp"

namespace Poker{
class Card : public ::Card {
public:
    template<class... Ts>
    constexpr Card(Ts... ts)
        : ::Card(ts...)
    {}
};
}//namespace Poker