#pragma once

#include <cinttypes>

namespace verlet {
    struct Constrain {
        std::size_t i;
        std::size_t j;

        float distance;
    };
}// namespace verlet