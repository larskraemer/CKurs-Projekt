#pragma once
#include <vector>

#include "playerhand.hpp"

namespace BlackJack{
struct Deck {
  Deck(size_t _deck_size = 52);
  void shuffle();
  void deal(PlayerHand &hand);

  std::vector<Card> deck;
  size_t deck_size;
};
}