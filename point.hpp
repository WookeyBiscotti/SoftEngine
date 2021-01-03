#pragma once

#include "vector2.hpp"

namespace verlet {

    struct Point {
        enum Flags {
            STATIC = 1 << 0,
            SLEEP = 1 << 1,
        };

        Point() = default;
        explicit Point(const Vector2 &p) : p1(p), p2(p) {}
        Point(const Vector2 &p1, const Vector2 &p2) : p1(p1), p2(p2) {}

        Vector2 p1;
        Vector2 p2;
    };
}// namespace verlet