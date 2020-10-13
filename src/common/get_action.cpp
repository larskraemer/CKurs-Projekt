#include "get_action.hpp"

#include <string_view>

void tolower(std::string& str) {
    for(auto& c : str) c = std::tolower(c);
}


[[nodiscard]] bool get_confirmation(std::string_view message) {
    std::cout << message << " [Y/n] ";
    std::string action;
    std::getline(std::cin, action, '\n');
    strip(action);
    tolower(action);
    if(action == "y") return true;
    if(action == "") return true;
    return false;
}