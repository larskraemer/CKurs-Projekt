#include "table.hpp"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <vector>

std::ostream &operator<<(std::ostream &os, Table &t)
{
    auto str = t.fields.str();
    auto fields = [&]() {
        std::vector<std::vector<std::string_view>> ret;
        auto rows = split(str, "\n");
        std::transform(rows.begin(), rows.end(), std::back_inserter(ret), [](auto sv) {
            auto ret = split(sv, "\t");
            while (ret.size() > 0 && ret.back().empty())
                ret.pop_back();
            return ret;
        });
        return ret;
    }();

    auto max_columns = std::max_element(fields.begin(), fields.end(), [](const auto &lhs, const auto &rhs) {
                           return lhs.size() < rhs.size();
                       })->size();

    for (auto &row : fields)
        row.resize(max_columns);

    std::vector<size_t> column_lens;
    column_lens.resize(max_columns);

    for (auto &row : fields)
    {
        for (auto [col, col_len] : zip(row, column_lens))
        {
            col_len = std::max({col.size(), col_len, t.m_min_field_width});
        }
    }

    auto total_len = std::accumulate(column_lens.begin(), column_lens.end(), size_t(0));
    auto total_padding = [&](){
        if(t.m_len > total_len + 4 + 2*column_lens.size()){
            return t.m_len - total_len - 2;
        }
        else{
            return 2*column_lens.size() + 2;
        }
    }();
    total_len += total_padding;
    auto per_column_padding = total_padding / (column_lens.size() + 1);
    auto extra_padding = total_padding - per_column_padding * (column_lens.size() + 1);
    auto begin_extra_padding = extra_padding / 2;
    auto end_extra_padding = extra_padding - begin_extra_padding;

    for (auto &row : fields)
    {
        if (row[0] == "\v")
        {
            os << repeat("#", total_len + 2);
            os << "\n";
            continue;
        }
        os << "#";
        os << repeat(" ", begin_extra_padding);
        for (auto [len, col] : zip(column_lens, row))
        {
            os << repeat(" ", per_column_padding);
            os << std::setw(len) << col;
        }
        os << repeat(" ", per_column_padding + end_extra_padding);
        os << "#\n";
    }

    t.fields = {};
    return os;
}