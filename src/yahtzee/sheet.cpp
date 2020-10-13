#include "sheet.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>

#include "common/table.hpp"
#include "common/util/enumerate.hpp"
#include "common/util/slice.hpp"
#include "common/util/zip.hpp"


constexpr std::array<int, 6> value_counts(const std::array<int, 5> &v)
{
    std::array<int, 6> value_counts = {};
    for (auto x : v)
        value_counts[x - 1]++;
    return value_counts;
}

void SheetState::write_in(size_t idx) {
    auto new_value = (values[idx].current_value = values[idx].potential_value);

    if(idx < 6){
        // Upper half
        sum_upper += new_value;
        bonus = (sum_upper >= 63) ? 35 : 0;
        total_upper = sum_upper + bonus;
    }
    else{
        total_lower += new_value;
    }
    total = total_lower + total_upper;
    open_positions--;
    for(auto& category : values){
        category.potential_value = -1;
    }
}

void SheetState::calculate_potential_values(const std::array<size_t, 6> &vc)
{
    const auto total_value = [&]() {
        auto ret = 0;
        for (size_t i = 0; i < vc.size(); i++)
            ret += vc[i] * (i + 1);
        return ret;
    }();
    const auto is_toak = std::any_of(vc.begin(), vc.end(), [](const auto &x) { return x >= 3; });
    const auto is_foak = std::any_of(vc.begin(), vc.end(), [](const auto &x) { return x >= 4; });
    const auto is_yahtzee = std::any_of(vc.begin(), vc.end(), [](const auto &x) { return x >= 5; });
    const auto is_fullhouse = std::any_of(vc.begin(), vc.end(), [](const auto &x) { return x == 3; }) &&
                              std::any_of(vc.begin(), vc.end(), [](const auto &x) { return x == 2; });
    const auto player_has_yahtzee = values[11].current_value != -1;
    int i = 0;

    values[i++].potential_value = [&]() { return vc[0] * 1; }();
    values[i++].potential_value = [&]() { return vc[1] * 2; }();
    values[i++].potential_value = [&]() { return vc[2] * 3; }();
    values[i++].potential_value = [&]() { return vc[3] * 4; }();
    values[i++].potential_value = [&]() { return vc[4] * 5; }();
    values[i++].potential_value = [&]() { return vc[5] * 6; }();
    values[i++].potential_value = [&]() {
        if (is_yahtzee && player_has_yahtzee)
            return 50;
        if (is_toak)
            return total_value;
        return 0;
    }();
    values[i++].potential_value = [&]() {
        if (is_yahtzee && player_has_yahtzee)
            return 50;
        if (is_foak)
            return total_value;
        return 0;
    }();
    values[i++].potential_value = [&]() {
        if (is_yahtzee && player_has_yahtzee)
            return 50;
        if (is_fullhouse)
            return 25;
        return 0;
    }();
    values[i++].potential_value = [&]() {
        if (is_yahtzee && player_has_yahtzee)
            return 50;
        if (vc[3 - 1] == 0 || vc[4 - 1] == 0)
            return 0;
        if (vc[6 - 1] >= 1 && vc[5 - 1] >= 1)
            return 30;
        if (vc[5 - 1] >= 1 && vc[2 - 1] >= 1)
            return 30;
        if (vc[2 - 1] >= 1 && vc[1 - 1] >= 1)
            return 30;
        return 0;
    }();
    values[i++].potential_value = [&]() {
        if (is_yahtzee && player_has_yahtzee)
            return 50;
        if (vc[2 - 1] == 0 || vc[3 - 1] == 0 || vc[4 - 1] == 0 || vc[5 - 1] == 0)
            return 0;
        if (vc[6 - 1] >= 1 || vc[1 - 1] >= 1)
            return 40;
        return 0;
    }();
    values[i++].potential_value = [&]() { return is_yahtzee ? 50 : 0; }();
    values[i++].potential_value = [&]() {
        if (is_yahtzee && player_has_yahtzee)
            return 50;
        return total_value;
    }();
}

void SheetState::reset() {
    *this = SheetState(name);
}


void show_sheets(const std::vector<SheetState>& sheets) {
    Table table;
    table.set_min_width(4);

    table << "\v\n";
    table << "Name";
    for(auto& sheet : sheets) table << "\t" << sheet.name;
    table << "\n\v\n";
    for(auto [i, name] : slice(enumerate(category_long_names), 0, 6)){
        table << name;
        for(auto& sheet : sheets){
            auto& value = sheet.values[i];
            table << "\t";
            if(value.current_value >= 0) table << value.current_value;
            else if(value.potential_value >= 0) table << "(" << value.potential_value << ")";
        }
        table << "\n";
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
            else if(value.potential_value >= 0) table << "(" << value.potential_value << ")";
        }
        table << "\n";
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
