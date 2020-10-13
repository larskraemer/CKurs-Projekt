#include <iostream>
#include <iomanip>
#include <vector>
#include <string_view>
#include <random>
#include <tuple>
#include <sstream>
#include <functional>

#include "common/util.hpp"
#include "common/get_action.hpp"
#include "common/iota.hpp"
#include "common/slice.hpp"
#include "common/table.hpp"
#include "common/filter.hpp"
#include "common/enumerate.hpp"
#include "common/zip.hpp"

#include "sheet.hpp"

using namespace std::string_view_literals;

template <class Iter>
void push_dice_rolls(Iter begin, Iter end)
{
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution dist(1, 6);

    for (auto it = begin; it != end; it++)
    {
        *it = dist(mt);
    }
}

enum class ValueType : int
{
    Ones,
    Twos,
    Threes,
    Fours,
    Fives,
    Sixes,
    ThreeOfAKind,
    FourOfAKind,
    FullHouse,

};

void do_reroll(std::array<int, 5> &dice)
{
    bool success = false;
    auto target_pos = dice.begin();
    while (not success)
    {
        std::cout << "What do you want to keep?\n";
        std::string input;
        std::getline(std::cin, input, '\n');
        std::istringstream ss(input);

        std::vector<int> keep_values;
        int val;
        while (ss >> val)
            keep_values.push_back(val);

        success = true;
        for (auto keep_val : keep_values)
        {
            if (target_pos == dice.end())
            {
                success = false;
                break;
            }
            auto pos = std::find_if(target_pos, dice.end(), [&](auto &v) {
                return v == keep_val;
            });
            if (pos == dice.end())
            {
                success = false;
                break;
            }
            else
            {
                std::swap(*pos, *target_pos);
            }
            target_pos++;
        }
    }
    std::cout << "Rolling " << dice.end() - target_pos << " Dice\n";
    push_dice_rolls(target_pos, dice.end());
    std::sort(dice.begin(), dice.end(), std::greater<>());
}

void do_round(SheetState &sheet)
{
    std::array<int, 5> dice;
    push_dice_rolls(dice.begin(), dice.end());
    std::sort(dice.begin(), dice.end(), std::greater<>());
    size_t remaining_rolls = 2;

    while (true)
    {
        for (auto d : dice)
        {
            std::cout << d << "\t";
        }
        std::cout << "\n";

        sheet.calculate_potential_values(dice);
        sheet.show(true);

        int action = 2;
        if (remaining_rolls > 0)
        {
            std::cout << "What do you want to do?\n";
            action = get_action({{1, "r", "Reroll [" + std::to_string(remaining_rolls) + " remain]"},
                                 {2, "w", "Write in"}});
        }

        switch (action)
        {
        case 1:
        { // Reroll
            do_reroll(dice);
            remaining_rolls--;
        }
        break;
        case 2:
        { //Write in
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
            auto idx = get_action(choices);
            sheet.write_in(idx);
            return;
        }
        break;
        }
    }
}

void show_sheets(const std::vector<SheetState>& sheets) {
    Table table(80);
    table.set_min_width(4);

    table << "\v\n";
    for(auto [i, name] : slice(enumerate(category_long_names), 0, 6)){
        table << name;
        for(auto& sheet : sheets){
            auto& value = sheet.values[i];
            table << "\t";
            if(value.current_value >= 0) table << value.current_value;
            else if(value.potential_value >= 0) table << value.potential_value;
        }
    }

    table << "\v\n";

    table << "Total";
    for(auto& sheet : sheets) table << "\t" << sheet.sum_upper;
    table << "\n";
    table << "Bonus";
    for(auto& sheet : sheets) table << "\t" << sheet.bonus;
    table << "\n";
    table << "Total upper";
    for(auto& sheet : sheets) table << "\t" << sheet.total_upper;
    table << "\n";

    table << "\v\n";

    for(auto [i, name] : slice(enumerate(category_long_names), 6, 13)){
        table << name;
        for(auto& sheet : sheets){
            auto& value = sheet.values[i];
            table << "\t";
            if(value.current_value >= 0) table << value.current_value;
            else if(value.potential_value >= 0) table << value.potential_value;
        }
    }

    table << "\v\n";
    table << "Total lower";
    for(auto& sheet : sheets) table << "\t" << sheet.total_lower;
    table << "\n";
    table << "Total upper";
    for(auto& sheet : sheets) table << "\t" << sheet.total_upper;
    table << "\n";
    table << "\v\n";
    table << "Total";
    for(auto& sheet : sheets) table << "\t" << sheet.total;
    table << "\n";
    table << "\v";

    std::cout << table;
}

int yahtzee_main()
{
    std::cout << "How many Players? ";
    size_t player_count;
    std::cin >> player_count;
    while (std::cin.get() != '\n');

    std::cout << "Please enter their names:\n";
    std::vector<std::string> names;
    for (size_t i = 0; i < player_count; i++)
    {
        names.emplace_back();
        std::cin >> names.back();
    }
    while (std::cin.get() != '\n');

    for (auto &name : names)
        std::cout << "\"" << name << "\"\n";

    SheetState sheet;

    do_round(sheet);

    sheet.show();

    return 0;
}