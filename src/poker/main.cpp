#include <array>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <vector>
#include <variant>
#include <sstream>

#include "card.hpp"
#include "card_ranking.hpp"
#include "common/table.hpp"
#include "common/currency.hpp"
#include "common/enumerate.hpp"

namespace Poker{
class Player;

struct GameState {
    std::vector<Player> players;
    size_t button_pos = 0;
    size_t current_player = 0;

    currency_t small_blind = 1_ct;
    currency_t big_blind = 2_ct;
    currency_t min_bet = 0_ct;
    currency_t current_bet = 0_ct;
    currency_t pot = 0_ct;

    std::vector<Card> open_cards = {};
};

class Player {
public:
    std::string name;
    enum class Action{
        Fold,
        Check,
        Call,
        Raise,
        AllIn
    };

    explicit Player(size_t stack)
        : m_stack(stack)
    {}

    void drawHandCards(Deck& deck){
        m_hand = Hand(deck);
    }

    Action queryAction(GameState &game) {
        bool can_check = round_bet >= game.current_bet;
        bool can_raise = m_stack >= game.current_bet;

        Table table(80);
        table << "\v\n\n";
        for(auto [i, player] : enumerate(game.players)) {
            if(i == game.current_player) table << "*";
            table << player.name << "\t";
        }
        table << "\n";
        for(auto [i, player] : enumerate(game.players)) {
            if(i == game.current_player) table << "UTG\t";
            else if(player.hasFolded()) table << "FOLDED\t";
            else if(player.isAllIn()) table << "ALL-IN\t";
            else table << "\t";
        }
        table << "\n";
        for(auto& player : game.players) table << player.getRoundBet() << Table::newColumn;
        table << "\n";
        for(auto& player : game.players) table << player.getStack() << Table::newColumn;
        table << "\n\n\v";
        std::cout << table;
        table << "\n";
        table << "Pot: " << game.pot;
        table << "\n\n\v\n";
        std::cout << table;
        for(size_t i = 0; i < 5; i++){
            if(i < game.open_cards.size()){
                table << game.open_cards[i];
            }
            else{
                table << "??";
            }
            table << Table::newColumn;
        }
        table << "\n\n\v";
        std::cout << table;

        table << "\n" << name << "\n";
        table << "Your Cards:" << "\n";
        std::cout << table;
        table << m_hand.cards()[0] << "\t" << m_hand.cards()[1]; 
        table << "\n\n\v";

        std::cout << table;

        while(true){
            std::cout << "What do you want to do?\n";
            std::cout << "f: Fold\n";
            if(can_check){
                std::cout << "c: Check\n";
            }
            else {
                std::cout << "c: Call [" << game.current_bet - round_bet <<"]\n";
            }
            if(can_raise){
                std::cout << "r: Raise\n";
            }
            std::cout << "a: All-In [" << m_stack <<"]\n";

            char c;
            std::cin >> c;

            switch(c) {
                case 'f':{ 
                    m_has_folded = true;
                    return Action::Fold; 
                } break;
                case 'c': {
                    if(can_check){
                        return Action::Check;
                    }
                    else{
                        bet(game.current_bet - round_bet);
                        return Action::Call;
                    }
                } break;
                case 'r':{
                    currency_t raise_amount;
                    std::cout << "How much? ";
                    std::cin >> raise_amount;
                    bet(raise_amount);
                    return Action::Raise;
                } break;
                case 'a': {
                    bet(m_stack);
                    m_is_all_in = true;
                    return Action::AllIn;
                }
            }
        }
    }

    void bet(currency_t amount){
        if(amount >= m_stack){
            round_bet += m_stack;
            m_stack = 0_ct;
        }
        else{
            m_stack -= amount;
            round_bet += amount;
        }
    }

    auto getRoundBet() const -> currency_t { return round_bet; }
    void addWinnings(currency_t winnings) { m_stack += winnings; }
    auto getStack() const -> currency_t {return m_stack;}
    void resetRoundBet() {
        round_bet = 0_ct;
    }

    bool hasFolded() const { return m_has_folded; }
    bool isAllIn() const { return m_is_all_in; }

    auto& hand() const { return m_hand; }
    auto& hand() { return m_hand; }

    void reset() {
        m_hand = Hand();
        m_handStrength = Hand::Strength();
        round_bet = 0_ct;
        m_is_all_in = false;
        m_has_folded = false;
    }

private:
    Hand::Strength m_handStrength;
    currency_t round_bet = 0_ct;
    bool m_is_all_in = false;
    bool m_has_folded = false;
    currency_t m_stack;
    Hand m_hand;
};



void do_betting_round(size_t first_player, GameState& game) {
    game.current_player = first_player;
    do{
        auto& player = game.players[game.current_player];
        if(not(player.hasFolded() || player.isAllIn())){
            auto action = player.queryAction(game);
            switch (action)
            {
            case Player::Action::Fold: std::cout << player.name << " folds\n"; break;
            case Player::Action::Check: std::cout << player.name << " checks\n"; break;
            case Player::Action::Call: std::cout << player.name << " calls\n"; break;
            case Player::Action::Raise: {
                game.current_bet = player.getRoundBet();
                first_player = game.current_player;
                std::cout << player.name << " raises to " << game.current_bet << "\n";
            } break;
            case Player::Action::AllIn:{
                game.current_bet = player.getRoundBet();
                first_player = game.current_player;
                std::cout << player.name << " is all-in [" << game.current_bet << "]\n";
            } break;
            }
        }

        game.current_player++;
        if(game.current_player == game.players.size()) game.current_player = 0;
    } while(game.current_player!=first_player);

    game.pot += std::transform_reduce(game.players.begin(), game.players.end(), 0_ct, std::plus<>(), [](Player& p){ auto ret = p.getRoundBet(); p.resetRoundBet(); return ret; });
    game.current_bet = 0_ct;
}

void do_round(GameState& game){
    auto deck = Deck();
    deck.shuffle();

    //size_t pot = 0;

    for(auto& player : game.players){
        player.drawHandCards(deck);
    }

    game.players[(game.button_pos + 1) % game.players.size()].bet(game.small_blind);
    game.players[(game.button_pos + 2) % game.players.size()].bet(game.big_blind);

    game.current_bet = game.big_blind;

    do_betting_round((game.button_pos + 3) % game.players.size(), game);

    game.open_cards.push_back(deck.draw());
    game.open_cards.push_back(deck.draw());
    game.open_cards.push_back(deck.draw());


    do_betting_round((game.button_pos + 1) % game.players.size(), game);

    game.open_cards.push_back(deck.draw());



    do_betting_round((game.button_pos + 1) % game.players.size(), game);

    game.open_cards.push_back(deck.draw());

    do_betting_round((game.button_pos + 1) % game.players.size(), game);

    for(auto& player : game.players) {
        if(player.hasFolded()){
            std::cout << player.name << "\t" << "Folded.\n";
        }
        else {
            player.hand().eval_hand(game.open_cards);
            std::cout << player.name << "\t" << player.hand() << "\n";
        }
    }

    Hand::Strength max_val;
    std::vector<std::vector<Player>::iterator> winning_players;

    for(auto it = game.players.begin(); it != game.players.end(); it++){
        if(it->hasFolded()) continue;
        if(it->hand().strength() > max_val){
            winning_players.clear();
            winning_players.push_back(it);
            max_val = it->hand().strength();
        }
        else if(it->hand().strength() == max_val) {
            winning_players.push_back(it);
        }
    }

    auto winnings = game.pot / winning_players.size();
    for(auto it : winning_players) {
        it->addWinnings(winnings);
        std::cout << it->name << " wins " << winnings << "\n";
    }

    game.current_player = 0;
    game.open_cards.clear();
    game.pot = 0_ct;
    for(auto& player : game.players) player.reset();
}
}//namespace Poker

int poker_main(){
    using namespace Poker;
    GameState game;
    game.players = std::vector<Player>{5, Player(100)};
    for(auto i{0}; i < 5; i++) game.players[i].name = "Player " + std::to_string(i);
    while(true){
        do_round(game);
    }

    return 0;
}