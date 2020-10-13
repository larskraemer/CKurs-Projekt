#include "gamestate.hpp"

namespace BlackJack
{
    GameState::GameState(currency_t _min_bet, size_t deck_size)
        : min_bet(_min_bet), dealer(), deck(deck_size) {}

    void GameState::add_player(std::string name, currency_t money)
    {
        players.emplace_back(std::make_unique<PlayerState>(name, money));
    }

    void GameState::add_CPU(std::string name, currency_t money)
    {
        players.emplace_back(std::make_unique<AIPlayerState>(name, money));
    }

    void GameState::start_round()
    {
        deck.shuffle();
        for (auto &player : players)
        {
            player->reset_round_state(*this);
        }
        dealer.reset_round_state(*this);

        deck.deal(dealer.current_hand());
        deck.deal(dealer.current_hand());
        dealer.current_hand().check_blackjack();
        for (auto &player : players)
        {
            deck.deal(player->current_hand());
            deck.deal(player->current_hand());
            player->current_hand().check_blackjack();
        }

        if (dealer.current_hand().blackjack())
        {
            std::cout << ("Dealer Blackjack!\n");
            do_round_end();
            return;
        }

        for (auto &player : players)
        {
            do_player_round(*player);
        }

        do_player_round(dealer);
        do_round_end();
    }

    void GameState::do_round_end()
    {
        for (auto &player : players)
        {
            for (auto &hand : player->hands())
            {
                auto win_state = hand->check_win_state(dealer.current_hand());
                switch (win_state.outcome)
                {
                case WinState::Outcome::Win:
                {
                    std::cout << player->name() << " wins (+" << win_state.win_amount << ")";
                }
                break;
                case WinState::Outcome::Push:
                {
                    std::cout << player->name() << " gets their bet back (+"
                              << win_state.win_amount << ")";
                }
                break;
                case WinState::Outcome::Loss:
                {
                    std::cout << player->name() << " loses";
                }
                break;
                }
                if (!win_state.reason.empty())
                {
                    std::cout << " [" << win_state.reason << "]";
                }
                std::cout << "\n";

                player->recv_money(win_state.win_amount);
            }
        }
    }
    void GameState::do_player_hand(PlayerStateBase &player)
    {
        auto &hand = player.current_hand();
        while (1)
        {
            player.display_state(*this);
            if (hand.busted())
            {
                std::cout << "BUSTED!\n";
                return;
            }
            if (hand.blackjack())
            {
                std::cout << "Blackjack!\n";
                return;
            }
            if (hand.value == 21)
            {
                std::cout << "Done.\n";
                return;
            }

            auto action = player.get_action(*this);
            switch (action)
            {
            case PlayerAction::Hit:
            { // Hit
                deck.deal(hand);
                std::cout << player.name() << " hits!\n";
            }
            break;
            case PlayerAction::Stand:
            { // Stand
                std::cout << player.name() << " stands.\n";
                return;
            }
            break;
            case PlayerAction::DoubleDown:
            { // Double down
                auto add = player.double_down();
                std::cout << player.name() << " doubles down! [+" << add << "]\n";
                deck.deal(hand);
                player.display_state(*this);
                if (hand.busted())
                {
                    std::cout << "BUSTED!\n";
                }
                return;
            }
            break;
            case PlayerAction::Split:
            { // Split
                player.split(deck);
            }
            break;
            }
        }
    }

    void GameState::do_player_round(PlayerStateBase &player)
    {
        do
        {
            do_player_hand(player);
        } while (player.next_hand());
    }
} // namespace BlackJack