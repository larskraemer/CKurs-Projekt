#include <array>
#include <string>
#include <string_view>
#include <tuple>

struct RowData
{
    int current_value = -1;
    int potential_value = -1;
};

struct SheetState
{
    SheetState(std::string a_name)
        : name(a_name)
    {}

    void calculate_potential_values(const std::array<int, 5> &v);
    void write_in(size_t idx);

    std::string name;
    std::array<RowData, 13> values = {};
    size_t open_positions = values.size();
    size_t sum_upper = 0;
    size_t bonus = 0;
    size_t total_upper = 0;
    size_t total_lower = 0;
    size_t total = 0;
};


constexpr auto category_short_names = std::array{ 
    "1", "2", "3", "4", "5", "6",  "tk", "fk", "fh", "ss", "bs", "y", "c",
};

constexpr auto category_long_names = std::array{ 
    "1s", "2s", "3s", "4s", "5s", "6s", "Three of a kind", "Four of a kind", "Full House", "Small Straight", "Big Straight", "Yahtzee", "Chance",
};