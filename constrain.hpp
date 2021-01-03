#pragma once

#include <cinttypes>

namespace verlet {
    struct Constrain {
        enum Flags : std::size_t {
            NONE = 0,
            DISABLE = 1 << 0,
        };

        bool is(Flags flag) const {
            return this->flags & flag;
        }

        std::size_t i;
        std::size_t j;

        float distance;

        Flags flags;
    };
}// namespace verlet