#include "table.hpp"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <vector>

const std::string_view Table::newColumn = "\t";
const std::string_view Table::newRow = "\n";


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
    max_columns = std::max(max_columns, t.m_min_field_width);

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
    auto total_padding = (t.m_len > total_len + 2) ? (t.m_len - total_len - 2) : 0;
    auto per_column_padding = total_padding / (column_lens.size() + 1);
    auto extra_padding = total_padding - per_column_padding * (column_lens.size() + 1);
    auto begin_extra_padding = extra_padding / 2;
    auto end_extra_padding = extra_padding - begin_extra_padding;

    for (auto &row : fields)
    {
        if (row[0] == "\v")
        {
            os << repeat("#", t.m_len);
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