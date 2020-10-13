#pragma once
#include <ostream>

#include "common/card.hpp"

namespace BlackJack{
struct Card : public ::Card{
  template<class... Ts>
  Card(Ts... ts)
    : ::Card(ts...)
  {}

  int value_BJ() const {
    constexpr int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
    return values[value()];
  }
};
}//namespace BlackJack