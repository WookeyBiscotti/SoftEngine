#pragma once

#include "../point/point_id.hpp"
#include "../types.hpp"
#include "../vector2.hpp"

namespace soften {

struct ConstrainDef {
	ConstrainDef(PointIdx i, PointIdx j, float distance = FLOAT_NAN, FlagsStorage flags = ConstrainFlags::NONE):
	    i(i), j(j), distance(distance), flags(flags) {}

	PointIdx i;
	PointIdx j;
	float distance;
	FlagsStorage flags;
};

} // namespace soften