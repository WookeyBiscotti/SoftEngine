#pragma once

#include "point_flags.hpp"
#include "../vector2.hpp"

namespace soften {

struct Point {
	Point() = default;
	explicit Point(const Vec2& p, FlagsStorage flags = PointFlags::NONE): p1(p), p2(p), flags(flags) {}
	Point(const Vec2& p1, const Vec2& p2, FlagsStorage flags = PointFlags::NONE): p1(p1), p2(p2), flags(flags) {}

	bool is(FlagsStorage flag) const { return this->flags & flag; }

	Vec2 p1;
	Vec2 p2;
	FlagsStorage flags;
};
} // namespace soften