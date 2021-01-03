#pragma once

#include "vector2.hpp"

namespace verlet {

    struct Point {
        enum Flags : std::size_t {
            NONE = 0,
            STATIC = 1 << 0,
            SLEEP = 1 << 1,
        };

        Point() = default;
        explicit Point(const Vector2 &p, Flags flags = NONE) : p1(p), p2(p), flags(flags) {}
        Point(const Vector2 &p1, const Vector2 &p2, Flags flags = NONE) : p1(p1), p2(p2), flags(flags) {}

        bool is(Flags flag) const {
            return this->flags & flag;
        }

        Vector2 p1;
        Vector2 p2;
        Flags flags;
    };
}// namespace verlet