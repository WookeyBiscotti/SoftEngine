#pragma once

#include <wykobi.hpp>

namespace verlet {

    struct Vector2 {
        Vector2() = default;
        Vector2(float x, float y) : x(x), y(y) {}

        auto &operator+=(const Vector2 &o) {
            x += o.x;
            y += o.y;

            return *this;
        }

        auto &operator*=(float o) {
            x *= o;
            y *= o;

            return *this;
        }

        auto &operator/=(float o) {
            x /= o;
            y /= o;

            return *this;
        }

        auto &operator-=(const Vector2 &o) {
            x -= o.x;
            y -= o.y;

            return *this;
        }

        bool approxEqual(const Vector2 &other, float epsilon = std::numeric_limits<float>::epsilon()) const {
            return std::abs(x - other.x) <= epsilon && std::abs(y - other.y) <= epsilon;
        }

        operator wykobi::point2d<float>() const {
            wykobi::point2d<float> p;
            p.x = x;
            p.y = y;

            return p;
        }

        float distance(const Vector2 &other) const {
            auto a = (x - other.x);
            auto b = (y - other.y);

            return std::sqrt(a * a + b * b);
        }

        float x, y;
    };

    inline Vector2 operator-(const Vector2 &a, const Vector2 &b) {
        return Vector2(a) -= b;
    }

    inline Vector2 operator+(const Vector2 &a, const Vector2 &b) {
        return Vector2(a) += b;
    }

    inline Vector2 operator*(const Vector2 &a, float b) {
        return Vector2(a) *= b;
    }

    inline Vector2 operator*(float b, const Vector2 &a) {
        return Vector2(a) *= b;
    }

    inline Vector2 operator/(const Vector2 &a, float b) {
        return Vector2(a) /= b;
    }


}// namespace verlet