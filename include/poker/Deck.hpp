#pragma once

#include <algorithm>
#include <vector>
#include <random>

#include "card.hpp"

namespace Poker{
struct Deck {
    Deck(){
        for(auto i{0}; i < 52; i++){
            cards.emplace_back(i);
        }
    }

    void shuffle(){
        static std::random_device rd;
        static std::mt19937 mt(rd());
        std::shuffle(cards.begin(), cards.end(), mt);
    }

    Card draw() {
        auto ret = cards.back();
        cards.pop_back();
        return ret;
    }

    void sort();
private:
    std::vector<Card> cards;
};
} //namespace Poker