#include "card_ranking.hpp"

namespace Poker{

constexpr auto pack(std::initializer_list<int> vals) {
    int ret = 0;
    for(auto val : vals) {
        ret *= 13;
        ret += val;
    }
    return ret;
}

template<class T>
constexpr auto unpack(int v) {
    std::array<T, 5> ret;
    for(size_t i = 0; i < 5; i++){
        ret[5 - i - 1] = T(v % 13);
        v /= 13;
    }
    return ret;
}

template<class ForwardIt, class MapFn>
auto max_mapped_value(ForwardIt begin, ForwardIt end, MapFn f) {
    auto max_it = std::max_element(begin, end, [&f](auto& a, auto& b){
        return f(a) < f(b);
    });
    return f(*max_it);
}

auto Hand::pair_type_value(const std::vector<Card>& cards) -> Hand::Strength
{
    const auto grouped_values = [&cards](){
        std::map<int, int> value_counts;
        std::vector<std::pair<int, int>> ret;
        for(auto& card : cards) {
            value_counts[card.value()]++;
        }

        ret = std::vector<std::pair<int, int>>(value_counts.begin(), value_counts.end());
        std::sort(ret.begin(), ret.end(), [](auto& lhs, auto& rhs){
            if(std::get<1>(lhs) == std::get<1>(rhs)){
                return std::get<0>(lhs) > std::get<0>(rhs);
            }
            return std::get<1>(lhs) > std::get<1>(rhs);
        });
        return ret;
    }();

    if (std::get<1>(grouped_values[0]) == 4)
    {
        //Four of a kind
        auto foak_value = std::get<0>(grouped_values[0]);
        auto kicker_value = max_mapped_value(cards.begin(), cards.end(), [foak_value](auto element) {
            auto value = element.value();
            if (value == foak_value)
            {
                return -1;
            }
            else
            {
                return (int)value;
            }
        });
        return std::make_pair(
            HandClass::FourOfAKind,
            pack({foak_value,
                  foak_value,
                  foak_value,
                  foak_value,
                  kicker_value}));
    }
    if (std::get<1>(grouped_values[0]) == 3)
    {
        if (std::get<1>(grouped_values[1]) >= 2)
        {
            // Full house
            auto triple_value = std::get<0>(grouped_values[0]);
            auto pair_value = max_mapped_value(grouped_values.begin(), grouped_values.end(), [triple_value](auto &element) {
                auto &[value, count] = element;
                if (count < 2)
                    return -1;
                if (value == triple_value)
                    return -1;
                return value;
            });
            return std::pair(
                HandClass::FullHouse,
                pack({
                    triple_value,
                    triple_value,
                    triple_value,
                    pair_value,
                    pair_value,
                }));
        }
        else
        {
            // Three of a kind
            auto triple_value = std::get<0>(grouped_values[0]);
            auto kicker1_value = max_mapped_value(grouped_values.begin(), grouped_values.end(), [triple_value](auto &element) {
                auto &[value, count] = element;
                if (value == triple_value)
                    return -1;
                return value;
            });
            auto kicker2_value = max_mapped_value(grouped_values.begin(), grouped_values.end(), [triple_value, kicker1_value](auto &element) {
                auto &[value, count] = element;
                if (value == triple_value)
                    return -1;
                if (value == kicker1_value)
                    return -1;
                return value;
            });

            return std::pair(HandClass::ThreeOfAKind, pack({triple_value, triple_value, triple_value, kicker1_value, kicker2_value}));
        }
    }
    if (std::get<1>(grouped_values[0]) == 2)
    {
        if (std::get<1>(grouped_values[1]) == 2)
        {
            // Two Pair
            auto pair1_value = std::get<0>(grouped_values[0]);
            auto pair2_value = std::get<0>(grouped_values[1]);
            auto kicker_value = max_mapped_value(cards.begin(), cards.end(), [pair1_value, pair2_value](auto &card) {
                auto value = card.value();
                if (value == pair1_value)
                    return -1;
                if (value == pair2_value)
                    return -1;
                return (int)value;
            });

            return std::pair(
                HandClass::TwoPair, 
                pack({
                    pair1_value, 
                    pair1_value, 
                    pair2_value, 
                    pair2_value, 
                    kicker_value
                }));
        }
        else
        {
            // Pair
            auto pair_value = std::get<0>(grouped_values[0]);
            auto kicker1_value = max_mapped_value(cards.begin(), cards.end(), [pair_value](auto &card) {
                auto value = card.value();
                if (value == pair_value)
                    return -1;
                return (int)value;
            });
            auto kicker2_value = max_mapped_value(cards.begin(), cards.end(), [pair_value, kicker1_value](auto &card) {
                auto value = card.value();
                if (value == pair_value)
                    return -1;
                if (value == kicker1_value)
                    return -1;
                return (int)value;
            });
            auto kicker3_value = max_mapped_value(cards.begin(), cards.end(), [pair_value, kicker1_value, kicker2_value](auto &card) {
                auto value = card.value();
                if (value == pair_value)
                    return -1;
                if (value == kicker1_value)
                    return -1;
                if (value == kicker2_value)
                    return -1;
                return (int)value;
            });
            return std::pair(
                HandClass::Pair, 
                pack({
                    pair_value, 
                    pair_value, 
                    kicker1_value, 
                    kicker2_value,
                    kicker3_value
                }));
        }
    }
    return std::pair(HandClass::Kicker, 0);
}

auto Hand::flush_like_value(const std::vector<Card>& cards) -> Hand::Strength 
{
    auto sorted_by_suit = [&](){
        std::array<std::vector<Card>, 4> ret;
        for(auto& card : cards) {
            ret[static_cast<std::underlying_type_t<Suit>>(card.suit())].push_back(card);
        }
        return ret;
    }();
    auto type = HandClass::Kicker;
    auto val = raw_value(cards);
    for (auto &suited_cards : sorted_by_suit)
    {
        if (suited_cards.size() < 5)
            continue;
        auto straight_val = straight_value(suited_cards);
        if (straight_val >= 0)
        {
            if (HandClass::StraightFlush > type)
            {
                val = straight_val;
            }
            else
            {
                val = std::max(val, straight_val);
            }
        }
        else if (type == HandClass::Flush)
        {
            val = std::max(val, raw_value(suited_cards));
        }
        else
        {
            type = HandClass::Flush;
            val = raw_value(suited_cards);
        }
    }
    return std::pair(type, val);
}

int Hand::straight_value(const std::vector<Card>& cards)
{
    unsigned cards_bitmask = 0;
    for (auto &card : cards)
    {
        cards_bitmask |= (1u << (card.value() + 1));
        if (card.value() == 'A'_v)
        {
            cards_bitmask |= 1;
        }
    }
    for (int i = 12; i >= 0; i--)
    {
        if ((cards_bitmask & (0b11111u << i)) >> i == 0b11111u)
        {
            return i + ((int)'5'_v);
        }
    }
    return -1;
}

auto Hand::raw_value(const std::vector<Card>& cards) -> int {
    int ret = 0;
    for(int i = 0; i < 5; i++){
        ret *= 13;
        ret += cards[i].value();
    }
    return ret;
}

constexpr auto class_names = std::array {
    "High-Card",
    "Pair",
    "Two Pair",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a kind",
    "Straight Flush"
};

std::ostream& operator<<(std::ostream& os, const Hand& h) {
    using UT = std::underlying_type_t<Hand::HandClass>;
    for(auto val : unpack<Value>(h.hand_value.second)){
        os << val;
    }
    os << " " << class_names[static_cast<UT>(h.hand_value.first)];
    return os;
}

}//namespace Poker