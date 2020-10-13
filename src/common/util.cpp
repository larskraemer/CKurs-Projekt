#include "util/util.hpp"

#include <string>

namespace detail{
std::ostream& operator<<(std::ostream& os, const repeater& r){
    for(size_t i = 0; i < r.m_times; i++){
        os << r.m_str;
    }
    return os;
}
}

auto split(std::string_view str, std::string_view delim) -> std::vector<std::string_view> {
    std::vector<std::string_view> ret;
    size_t pos = 0u;
    while(true){
        auto new_pos = str.find(delim, pos);
        if(new_pos == std::string_view::npos){
            ret.emplace_back(str.begin() + pos, str.size() - pos);
            return ret;
        }
        else{
            ret.emplace_back(str.begin() + pos, new_pos - pos);
            pos = new_pos + delim.size();
        }
    }
}

void strip(std::string& str){
    auto rbeg = str.rbegin();
    while(rbeg != str.rend() && std::isspace(*rbeg)) rbeg++;
    str.resize(str.rend() - rbeg);

    auto beg = str.begin();
    while(beg != str.end() && std::isspace(*beg)) beg++;
    str = std::string(beg, str.end());
}