#pragma once

#include <string_view>
#include <vector>

namespace detail{
class repeater{
public:
    repeater(std::string_view str, size_t times)
        : m_str(str), m_times(times)
    {}

    friend std::ostream& operator<<(std::ostream&, const repeater&);
private:
    std::string_view m_str;
    size_t m_times;
};


}//namespace detail

auto split(std::string_view str, std::string_view delim) -> std::vector<std::string_view>;
inline auto repeat(std::string_view str, size_t times) {
    return detail::repeater(str, times);
}
void strip(std::string& str);