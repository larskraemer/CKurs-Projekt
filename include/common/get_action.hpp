#pragma once
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>

#include "util.hpp"

template<class T = int>
[[nodiscard]] auto get_action(std::vector<std::tuple<T, std::string, std::string>> options) {
    for(auto[_, a, description] : options) {
        std::cout << std::setw(2) << a << "\t";
        std::cout << description << "\n";
    }

    std::string action;
    while(true){
        std::getline(std::cin, action, '\n');
        strip(action);
        for(auto[ret, a, _] : options) {
            if(a == action){
                return ret;
            }
        }
    }
}

[[nodiscard]] bool get_confirmation(std::string_view);