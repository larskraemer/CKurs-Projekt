#include "sheet.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>

#include "common/slice.hpp"
#include "common/table.hpp"
#include "common/zip.hpp"


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
}

void SheetState::calculate_potential_values(const std::array<int, 5> &v)
{
    const auto vc = value_counts(v);
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


void SheetState::show(bool show_potential_value) const {
    Table table(80);
    table.set_min_width(4);

    auto z = zip(values, category_long_names);

    table << "\v\n";
    for(auto [row, name] : slice(z, 0, 6)){
        table << name << "\t";
        if(row.current_value >= 0)  table << row.current_value;
        else if(show_potential_value) table << "(" << row.potential_value << ")";
        table << "\n";
    }

    table << "\v\n";

    table << "Total\t" << sum_upper << "\n";
    table << "Bonus\t" << bonus << "\n";
    table << "Total upper\t" << total_upper << "\n";

    table << "\v\n";

    for(const auto& [row, name] : slice(z, 6, 13)){
        table << name << "\t";
        if(row.current_value >= 0)  table << row.current_value;
        else if(show_potential_value) table << "(" << row.potential_value << ")";
        table << "\n";
    }

    table << "\v\n";
    table << "Total lower\t" << total_lower << "\n";
    table << "Total upper\t" << total_upper << "\n";
    table << "\v\n";
    table << "Total\t" << total << "\n";
    table << "\v";

    std::cout << table;
}