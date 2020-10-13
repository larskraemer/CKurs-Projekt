cmake_minimum_required(VERSION 3.0)

project(Projekt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wpedantic -Werror -fsanitize=address")

file(GLOB POKER_FILES src/poker/*.cpp)
file(GLOB BLACKJACK_FILES src/blackjack/*.cpp)
file(GLOB YAHTZEE_FILES src/yahtzee/*.cpp)
file(GLOB COMMON_FILES src/common/*.cpp src/main.cpp)

add_library(poker ${POKER_FILES})
target_include_directories(poker PRIVATE include include/poker)

add_library(blackjack ${BLACKJACK_FILES})
target_include_directories(blackjack PRIVATE include include/blackjack)

add_library(yahtzee ${YAHTZEE_FILES})
target_include_directories(yahtzee PRIVATE include include/yahtzee)

add_executable(projekt ${COMMON_FILES})
target_include_directories(projekt PRIVATE include/common)
target_link_libraries(projekt blackjack poker yahtzee)