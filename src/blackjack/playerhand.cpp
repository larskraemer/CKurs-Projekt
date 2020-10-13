#include "playerhand.hpp"
#include "playerstate.hpp"

namespace BlackJack
{
    void PlayerHand::calc_value()
    {
        value = 0;
        softness = 0;

        for (auto &card : cards)
        {
            auto new_val = card.value_BJ();
            value += new_val;
            softness += (new_val == 11) ? 1 : 0;
            if (value > 21 && softness > 0)
            {
                value -= 10;
                softness -= 1;
            }
        }
    }

    void PlayerHand::add_card(Card card)
    {
        value += card.value_BJ();
        softness += (card.value_BJ() == 11) ? 1 : 0;
        if (value > 21 && softness > 0)
        {
            value -= 10;
            softness -= 1;
        }

        cards.push_back(card);
    }

    bool PlayerHand::busted() const noexcept { return value > 21; }

    auto PlayerHand::check_win_state(const PlayerHand &dealer_hand) -> WinState
    {
        using std::to_string;
        WinState ret;
        if (dealer_hand.blackjack())
        {
            if (blackjack())
            {
                // Push
                ret = WinState{WinState::Outcome::Push, current_bet, ""};
            }
            else
            {
                // lose
                ret = WinState{WinState::Outcome::Loss, currency_t{0}, "Dealer Blackjack"};
            }
        }
        else if (dealer_hand.busted())
        {
            if (busted())
            {
                ret = WinState{WinState::Outcome::Loss, currency_t{0}, "Busted"};
            }
            else if (blackjack())
            {
                ret =
                    WinState{WinState::Outcome::Win, 3 * current_bet, "Dealer Busted"};
            }
            else
            {
                ret =
                    WinState{WinState::Outcome::Win, 2 * current_bet, "Dealer Busted"};
            }
        }
        else
        {
            if (busted())
            {
                ret = WinState{WinState::Outcome::Loss, currency_t{0}, "Busted"};
            }
            else if (blackjack())
            {
                ret = WinState{WinState::Outcome::Win, 3 * current_bet, "Blackjack"};
            }
            else if (value > dealer_hand.value)
            {
                ret = WinState{WinState::Outcome::Win, 2 * current_bet,
                               to_string(value) + " vs " + to_string(dealer_hand.value)};
            }
            else if (value == dealer_hand.value)
            {
                ret = WinState{WinState::Outcome::Push, current_bet,
                               to_string(value) + " vs " + to_string(dealer_hand.value)};
            }
            else
            {
                ret = WinState{WinState::Outcome::Loss, currency_t{0},
                               to_string(value) + " vs " + to_string(dealer_hand.value)};
            }
        }
        return ret;
    }

    std::ostream &operator<<(std::ostream &os, const PlayerHand &ph)
    {
        for (auto card : ph.cards)
        {
            os << card << " ";
        }
        return os;
    }

    void PlayerHand::check_blackjack() noexcept
    {
        m_blackjack = (value == 21) and (cards.size() == 2);
    }
} // namespace BlackJack