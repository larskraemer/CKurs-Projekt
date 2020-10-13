#pragma once
#include "card.hpp"

#include <algorithm>
#include <compare>
#include <map>
#include <optional>
#include <set>
#include <tuple>

#include "Deck.hpp"

namespace Poker{
class Hand{
    enum class HandClass : int {
        Kicker = 0,
        Pair = 1,
        TwoPair = 2,
        ThreeOfAKind = 3,
        Straight = 4,
        Flush = 5,
        FullHouse = 6,
        FourOfAKind = 7,
        StraightFlush = 8,
    };
public:
    using Strength = std::pair<HandClass, int>;

    constexpr Hand() {}

    explicit constexpr Hand(const std::array<Card, 2>& cards)
        : m_cards(cards)
    {}

    explicit constexpr Hand(Deck& deck){
        m_cards[0] = deck.draw();
        m_cards[1] = deck.draw();
        if(m_cards[0] < m_cards[1]) std::swap(m_cards[0], m_cards[1]);
    }

    constexpr auto operator==(const Hand& other) const {
        return hand_value == other.hand_value;
    }

    constexpr auto operator!=(const Hand& other) const {
        return hand_value != other.hand_value;
    }

    constexpr auto operator<(const Hand& other) const {
        return hand_value < other.hand_value;
    }

    constexpr auto operator>(const Hand& other) const {
        return hand_value > other.hand_value;
    }

    constexpr auto operator>=(const Hand& other) const {
        return hand_value >= other.hand_value;
    }

    constexpr auto operator<=(const Hand& other) const {
        return hand_value <= other.hand_value;
    }


    void eval_hand(const std::vector<Card>& open_cards) {
        std::vector<Card> cards(open_cards.begin(), open_cards.end());
        cards.insert(cards.end(), m_cards.begin(), m_cards.end());
        std::sort(std::begin(cards), std::end(cards), std::greater<Card>());
        hand_value = std::max(flush_like_value(cards), pair_type_value(cards));
    }

    auto strength() const { return hand_value; }

    auto& cards() const { return m_cards; }

    friend std::ostream& operator<<(std::ostream& os, const Hand& h);
private:
    static auto pair_type_value(const std::vector<Card>&) -> Strength;
    static auto flush_like_value(const std::vector<Card>&) -> Strength;
    static auto straight_value(const std::vector<Card>&) -> int;
    static auto raw_value(const std::vector<Card>&) -> int;

    Strength hand_value;
    std::array<Card, 2> m_cards;
};


std::ostream& operator<<(std::ostream& os, const Hand& h);

} //namespace Poker