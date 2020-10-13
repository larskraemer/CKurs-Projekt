#include "Deck.hpp"

#include <algorithm>
#include <random>

namespace BlackJack{
Deck::Deck(size_t _deck_size) : deck_size(_deck_size) {
    shuffle();
}

void Deck::shuffle() {
    static std::random_device rd;
    static std::mt19937 mt(rd());

    deck.clear();
    for (auto i{0u}; i < deck_size; i++) {
        deck.emplace_back(Card((int) i % 52));
    }

    std::shuffle(deck.begin(), deck.end(), mt);
}

void Deck::deal(PlayerHand &hand) {
    if(deck.empty()) throw std::runtime_error("Empty Deck!");
    hand.add_card(deck.back());
    deck.pop_back();
}
}