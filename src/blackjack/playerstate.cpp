#include "playerstate.hpp"

#include "gamestate.hpp"
#include "basic_strategy.hpp"
#include "common/get_action.hpp"
#include "common/table.hpp"

namespace BlackJack{
void PlayerState::split(Deck &deck)
{
    auto &cur_hand = current_hand();
    if (current_hand().cards.size() != 2)
    {
        throw std::runtime_error("Invalid Split!");
    }
    if (current_hand().cards[0].value_BJ() !=
        current_hand().cards[1].value_BJ())
    {
        throw std::runtime_error("Invalid Split!");
    }

    add_hand(cur_hand.current_bet);
    auto &new_hand = *m_hands.back();

    new_hand.cards.push_back(cur_hand.cards[1]);
    cur_hand.cards.pop_back();

    deck.deal(new_hand);
    deck.deal(cur_hand);

    new_hand.calc_value();
    cur_hand.calc_value();
}

auto PlayerState::double_down() -> currency_t
{
    auto additional_bet = std::min(m_money, current_hand().current_bet);
    m_money -= additional_bet;
    current_hand().current_bet += additional_bet;
    return additional_bet;
}

PlayerState::PlayerState(std::string _name, currency_t _money)
    : m_name(_name), m_money(_money) {}

void PlayerState::reset_round_state(const GameState &state)
{
    m_hands.clear();

    auto bet = get_bet(state.min_bet);

    add_hand(bet);
    m_current_hand = 0;
}


auto PlayerState::get_bet(currency_t min_bet) const -> currency_t {
    auto bet = currency_t{0};

    std::cout << name() << ", what is your bet? (Stack: " << money() << ")\n";
    while (1)
    {
        std::cin >> bet;
        while(std::cin.get() != '\n');
        if (bet > money())
        {
            printf("You don't have that much. Try again!\n");
        }
        else if (bet < min_bet)
        {
            std::cout << "The minimum Bet is " << min_bet << "; Try again!\n";
        }
        else
        {
            break;
        }
    }
    return bet;
}

auto PlayerState::get_action(const GameState &) const -> PlayerAction
{
    auto &hand = *m_hands.at(m_current_hand);
    std::vector<std::tuple<PlayerAction, std::string, std::string>> actions = {
        {PlayerAction::Hit, "1", "Hit"},
        {PlayerAction::Stand, "2", "Stand"},
        {PlayerAction::DoubleDown, "3", "Double Down"}
    };

    if (hand.cards.size() == 2)
    {
        if (hand.cards[0].value_BJ() == hand.cards[1].value_BJ())
        {
            actions.emplace_back(PlayerAction::Split, "4", "Split");
        }
    }

    return ::get_action<PlayerAction>(actions);
}

void PlayerState::display_state(const GameState& state) const {
    auto &hand = *m_hands.at(m_current_hand);
    Table table(80);
    table << "\v\n\n";
    table << name() << "\n\n\v";
    std::cout << table;
    table << "\n";
    table << "Dealer Cards:\t";
    table << state.dealer.current_hand().cards[0] << "\t??\n\n\v\n\n";
    table << "Your Cards:";
    for(auto& c : hand.cards) table << "\t" << c;
    table << "\n\nCurrent Value:\t" << hand.value;
    table << "\n\n\v";
    std::cout << table;
}

void PlayerState::add_hand(currency_t bet)
{
    hands().emplace_back(std::make_unique<PlayerHand>());
    hands().back()->current_bet = bet;
    m_money -= bet;
}

bool PlayerState::next_hand() noexcept
{
    m_current_hand++;
    if (m_current_hand == m_hands.size())
    {
        m_current_hand = 0;
        return false;
    }
    return true;
}

void DealerState::reset_round_state(const GameState &)
{
    hand.cards.clear();
    hand.value = 0;
    hand.softness = 0;
}

auto DealerState::get_action(const GameState &) const -> PlayerAction
{
    if (hand.value < 17)
        return PlayerAction::Hit;
    return PlayerAction::Stand;
}

void DealerState::display_state(const GameState&) const {
    std::cout << "Dealer Hand: " << hand << "\n";
}

auto AIPlayerState::get_action(const GameState &state) const -> PlayerAction {
    auto& hand = current_hand();

    auto dealer_open_val = state.dealer.current_hand().cards[0].value_BJ();

    if(hand.cards.size() == 2
        && hand.cards[0].value_BJ() == hand.cards[1].value_BJ()){
        //Splittable
        auto val = hand.cards[0].value_BJ();
        return std::get<2>(basic_strategy)[val - 2][dealer_open_val - 2];
    }
    if(hand.softness > 0){
        // Soft hand
        return std::get<1>(basic_strategy)[hand.value - 13][dealer_open_val - 2];
    }
    else{
        return std::get<0>(basic_strategy)[hand.value - 4][dealer_open_val - 2];
    }
}

auto AIPlayerState::get_bet(currency_t) const -> currency_t {
    return 10_ct;
}
}