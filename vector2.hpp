#pragma once

#include "utils.hpp"

#include <wykobi.hpp>

namespace soften {

struct Vec2 {
	Vec2() = default;
	Vec2(float x, float y): x(x), y(y) {}

	auto& operator+=(const Vec2& o) {
		x += o.x;
		y += o.y;

		return *this;
	}

	auto& operator*=(float o) {
		x *= o;
		y *= o;

		return *this;
	}

	auto& operator/=(float o) {
		x /= o;
		y /= o;

		return *this;
	}

	auto& operator-=(const Vec2& o) {
		x -= o.x;
		y -= o.y;

		return *this;
	}

	bool approxEqual(const Vec2& other, float epsilon = std::numeric_limits<float>::epsilon()) const {
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

	float distance(const Vec2& other) const {
		auto a = (x - other.x);
		auto b = (y - other.y);

		return std::sqrt(a * a + b * b);
	}

	float length() const { return std::sqrt(x * x + y * y); }

	float fastInvLength() const { return fastInvSqrt(x * x + y * y); }

	Vec2 n() const {
		auto result = Vec2(*this);
		result /= length();
		return result;
	}

	Vec2 fastN() const {
		auto result = Vec2(*this);
		result *= result.fastInvLength();

		return result;
	}

	float cross(const Vec2& other) const { return x * other.y - y * other.x; }

	float x, y;
};

inline Vec2 operator-(const Vec2& a, const Vec2& b) {
	return Vec2(a) -= b;
}

inline Vec2 operator+(const Vec2& a, const Vec2& b) {
	return Vec2(a) += b;
}

inline float operator*(const Vec2& a, const Vec2& b) {
	return a.x * b.x + a.y * b.y;
}

inline Vec2 operator*(const Vec2& a, float b) {
	return Vec2(a) *= b;
}

inline Vec2 operator*(float b, const Vec2& a) {
	return Vec2(a) *= b;
}

inline Vec2 operator/(const Vec2& a, float b) {
	return Vec2(a) /= b;
}

} // namespace soften