#include <iostream>
#include <iomanip>
#include <vector>
#include <string_view>
#include <random>
#include <tuple>
#include <sstream>
#include <functional>

#include "common/get_action.hpp"
#include "common/table.hpp"
#include "common/util/util.hpp"
#include "common/util/iota.hpp"
#include "common/util/filter.hpp"
#include "common/util/zip.hpp"

#include "sheet.hpp"


template <class Container>
void push_dice_rolls(Container& container, size_t count)
{
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution dist(0, 5);

    for(auto i = 0u; i < count; i++){
        container[dist(mt)]++;
    }
}

void do_reroll(std::array<size_t, 6> &dice)
{
    std::cout << "What do you want to keep?\n";
    std::string input;
    std::getline(std::cin, input, '\n');
    size_t kept_dice = 0;

    std::array<size_t, 6> keep_dice = {};
    for(auto c : input){
        if(isdigit(c)){
            int dice_val = c-'0';
            if(dice_val < 1 || dice_val > 6) continue;
            if(dice[dice_val - 1] > 0){
                dice[dice_val - 1]--;
                keep_dice[dice_val - 1]++;
                kept_dice++;
            }
        }
    }
    push_dice_rolls(keep_dice, 5 - kept_dice);
    dice = keep_dice;
}

enum class PlayerAction {
    ReRoll,
    WriteIn
};

void do_round(SheetState &sheet, const std::vector<SheetState>& sheets)
{
    std::array<size_t, 6> dice_counts = {};

    push_dice_rolls(dice_counts, 5);
    size_t remaining_rolls = 2;

    while (true)
    {
        sheet.calculate_potential_values(dice_counts);
        show_sheets(sheets);

        for(int i = 5; i >= 0; i--){
            for(size_t j = 0; j < dice_counts[i]; j++){
                std::cout << (i+1) << "\t";
            }
        }
        std::cout << "\n";

        auto action = [&](){
            if(remaining_rolls == 0) return PlayerAction::WriteIn;
            return get_action<PlayerAction>({
                {PlayerAction::ReRoll,  "r", "Reroll [" + std::to_string(remaining_rolls) + " remain]"},
                {PlayerAction::WriteIn, "w", "Write in"}
            });
        }();

        switch (action)
        {
        case PlayerAction::ReRoll:{
            do_reroll(dice_counts);
            remaining_rolls--;
        } break;
        case PlayerAction::WriteIn: {
            std::vector<std::tuple<int, std::string, std::string>> choices;
            auto nums = iota(0);

            for(auto [i, category, short_name, long_name] : 
                filter(
                    zip(
                        nums, 
                        sheet.values, 
                        category_short_names, 
                        category_long_names
                    ), 
                    [](auto args){
                        return std::get<1>(args).current_value == -1;
                    }
                )){
                choices.emplace_back(i, short_name, long_name);
            }
            sheet.write_in(get_action(choices));
            return;
        } break;
        }
    }
}


int yahtzee_main()
{
    std::cout << "How many Players? ";
    size_t player_count;
    std::cin >> player_count;
    while (std::cin.get() != '\n');

    std::cout << "Please enter their names:\n";
    std::vector<SheetState> sheets;
    for (size_t i = 0; i < player_count; i++)
    {
        std::string name;
        std::cin >> name;
        sheets.emplace_back(name);
    }
    while (std::cin.get() != '\n');

    while(true){
        for(auto i = 0u; i < 13; i++){
            for(auto& sheet : sheets){
                do_round(sheet, sheets);
            }
        }

        show_sheets(sheets);

        if(!get_confirmation("Play again? ")){
            break;
        }
        for(auto& sheet: sheets) sheet.reset();
    }

    return 0;
}