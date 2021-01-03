#pragma once

#include "../vector2.hpp"
#include "point_flags.hpp"

namespace soften {

struct PointDef {
	PointDef(Vec2 p1, Vec2 p2, FlagsStorage flags = PointFlags::NONE): p1(p1), p2(p2), flags(flags) {}
	PointDef(Vec2 p, FlagsStorage flags = PointFlags::NONE): p1(p), p2(p), flags(flags) {}

	Vec2 p1;
	Vec2 p2;

	FlagsStorage flags;
};

} // namespace soften