#pragma once
#include <random>
#include <string>
#include <vector>

#include "playerstate.hpp"
#include "playerhand.hpp"
#include "Deck.hpp"

namespace BlackJack
{
    struct GameState
    {
        GameState(currency_t _min_bet = 1_ct, size_t deck_size = 52);
        void add_player(std::string name, currency_t money);
        void add_CPU(std::string name, currency_t money);

        void start_round();
        void do_round_end();
        void do_player_hand(PlayerStateBase&);
        void do_player_round(PlayerStateBase&);

        currency_t min_bet;
        std::vector<std::unique_ptr<PlayerState>> players;
        DealerState dealer;
        Deck deck;
    };
} // namespace BlackJack