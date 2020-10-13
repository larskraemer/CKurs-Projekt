#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "gamestate.hpp"
#include "common/get_action.hpp"

int blackjack_main()
{
    using namespace BlackJack;

    std::cout << "Choose a name: ";
    std::string name;
    std::cin >> name;
    while(std::cin.get() != '\n');

    std::cout << "How many CPU players do you want? ";
    size_t cpu_count;
    std::cin >> cpu_count;
    while(std::cin.get() != '\n');

    auto state = GameState();
    state.add_player(name, 100_ct);
    for(size_t i = 0; i < cpu_count; i++){
        state.add_CPU("CPU " + std::to_string(i+1), 100_ct);
    }

    while (not state.players.empty())
    {
        state.start_round();

        for (auto &player : state.players)
        {
            if (player->money() < state.min_bet)
            {
                std::cout << player->name() << " is unable to continue playing.\n";
            }
        }
        state.players.erase(
            std::remove_if(state.players.begin(), state.players.end(),
                           [&state](const auto &p) {
                               return p->money() < state.min_bet;
                           }),
            state.players.end());
        if(!get_confirmation("Do you want to continue playing?")){
            break;
        }
    }
    return 0;
}