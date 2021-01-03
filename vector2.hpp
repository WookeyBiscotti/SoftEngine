#pragma once

#include "utils.hpp"
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

        wykobi::point2d<float> wykobi() const {
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

        float length() const {
            return std::sqrt(x * x + y * y);
        }

        float fastInvLength() const {
            return fastInvSqrt(x * x + y * y);
        }

        Vector2 n() const {
            auto result = Vector2(*this);
            result /= length();

            return result;
        }

        Vector2 fastN() const {
            auto result = Vector2(*this);
            result *= result.fastInvLength();

            return result;
        }

        float x, y;
    };

    inline Vector2 operator-(const Vector2 &a, const Vector2 &b) {
        return Vector2(a) -= b;
    }

    inline Vector2 operator+(const Vector2 &a, const Vector2 &b) {
        return Vector2(a) += b;
    }

    inline float operator*(const Vector2 &a, const Vector2 &b) {
        return a.x * b.x + a.y * b.y;
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