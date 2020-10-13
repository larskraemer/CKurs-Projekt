#pragma once

#include <array>
#include <tuple>

#include "playeraction.hpp"

namespace BlackJack{

constexpr auto basic_strategy = std::tuple{
    std::array{ // Hard totals
        std::array{ // 4
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::Hit, // 5
            PlayerAction::Hit, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        },
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::Hit, // 5
            PlayerAction::Hit, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 5
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::Hit, // 5
            PlayerAction::Hit, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 6
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::Hit, // 5
            PlayerAction::Hit, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 7
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::Hit, // 5
            PlayerAction::Hit, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 8
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::DoubleDown, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 9
        std::array{
            PlayerAction::DoubleDown, // 2
            PlayerAction::DoubleDown, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::DoubleDown, // 7
            PlayerAction::DoubleDown, // 8
            PlayerAction::DoubleDown, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 10
        std::array{
            PlayerAction::DoubleDown, // 2
            PlayerAction::DoubleDown, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::DoubleDown, // 7
            PlayerAction::DoubleDown, // 8
            PlayerAction::DoubleDown, // 9
            PlayerAction::DoubleDown, // T
            PlayerAction::Hit, // A
        }, // 11
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 12
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 13
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 14
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 15
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 16
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 17
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 18
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 19
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 20
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 21
    },
    std::array{ // Soft totals
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 13
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 14
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 15
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 16
        std::array{
            PlayerAction::Hit, // 2
            PlayerAction::DoubleDown, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 17
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::DoubleDown, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, // 18
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 19
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 20
        std::array{
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, // 21
    },
    std::array{ //Splits
        std::array{ // 2, 2
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Split, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        },
        std::array{ // 3, 3
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Split, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        },
        std::array{ // 4, 4
            PlayerAction::Hit, // 2
            PlayerAction::Hit, // 3
            PlayerAction::Hit, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        },
        std::array{ // 5, 5
            PlayerAction::DoubleDown, // 2
            PlayerAction::DoubleDown, // 3
            PlayerAction::DoubleDown, // 4
            PlayerAction::DoubleDown, // 5
            PlayerAction::DoubleDown, // 6
            PlayerAction::DoubleDown, // 7
            PlayerAction::DoubleDown, // 8
            PlayerAction::DoubleDown, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, 
        std::array{ // 6, 6
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Hit, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, 
        std::array{ // 7, 7
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Split, // 7
            PlayerAction::Hit, // 8
            PlayerAction::Hit, // 9
            PlayerAction::Hit, // T
            PlayerAction::Hit, // A
        }, 
        std::array{ // 8, 8
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Split, // 7
            PlayerAction::Split, // 8
            PlayerAction::Split, // 9
            PlayerAction::Split, // T
            PlayerAction::Split, // A
        }, 
        std::array{ // 9, 9
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Split, // 8
            PlayerAction::Split, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, 
        std::array{ // 10, 10
            PlayerAction::Stand, // 2
            PlayerAction::Stand, // 3
            PlayerAction::Stand, // 4
            PlayerAction::Stand, // 5
            PlayerAction::Stand, // 6
            PlayerAction::Stand, // 7
            PlayerAction::Stand, // 8
            PlayerAction::Stand, // 9
            PlayerAction::Stand, // T
            PlayerAction::Stand, // A
        }, 
        std::array{ // A, A
            PlayerAction::Split, // 2
            PlayerAction::Split, // 3
            PlayerAction::Split, // 4
            PlayerAction::Split, // 5
            PlayerAction::Split, // 6
            PlayerAction::Split, // 7
            PlayerAction::Split, // 8
            PlayerAction::Split, // 9
            PlayerAction::Split, // T
            PlayerAction::Split, // A
        }, 
    }
};

}