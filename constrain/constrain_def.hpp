#pragma once

#include "../point/point_id.hpp"
#include "../types.hpp"
#include "../vector2.hpp"

namespace soften {

struct ConstrainDef {
	ConstrainDef(PointId i, PointId j, float distance = FLOAT_NAN, FlagsStorage flags = ConstrainFlags::NONE):
	    i(i), j(j), distance(distance), flags(flags) {}

	PointId i;
	PointId j;
	float distance;
	FlagsStorage flags;
};

} // namespace soften