#pragma once

#include "../vector2.hpp"
#include "point_flags.hpp"

namespace soften {

struct Point {
	Point() = default;
	explicit Point(const Vec2& p, FlagsStorage flags = PointFlags::NONE, float m = 1.0f):
	    p1(p), p2(p), m(m), flags(flags) {}
	Point(const Vec2& p1, const Vec2& p2, FlagsStorage flags = PointFlags::NONE, float m = 1.0f):
	    p1(p1), p2(p2), m(m), flags(flags) {}

	bool is(FlagsStorage flag) const { return this->flags & flag; }

	Vec2 p1;
	Vec2 p2;
	float m;
	FlagsStorage flags;
};
} // namespace soften