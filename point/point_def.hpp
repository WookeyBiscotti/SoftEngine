#pragma once

#include "../utils/vector2.hpp"
#include "point_flags.hpp"

namespace soften {

struct PointDef {
	PointDef(Vec2 p1, Vec2 p2, FlagsStorage flags = PointFlags::NONE, float m = 1.0f):
	    p1(p1), p2(p2), m(m), flags(flags) {}
	PointDef(Vec2 p, FlagsStorage flags = PointFlags::NONE, float m = 1.0f): p1(p), p2(p), m(m), flags(flags) {}

	Vec2 p1;
	Vec2 p2;
	float m;

	FlagsStorage flags;
};

} // namespace soften