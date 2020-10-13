#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>

#include "get_action.hpp"
#include "util.hpp"

int blackjack_main();
int poker_main();
int yahtzee_main();


int main() {
    while(true){
        std::cout << "Choose a Game\n";
        auto func = get_action<int(*)(void)>({
            {blackjack_main, "b", "Blackjack"},
            {poker_main, "p", "Poker"},
            {yahtzee_main, "y", "Yahtzee"}
        });
        func();
    }
    
}