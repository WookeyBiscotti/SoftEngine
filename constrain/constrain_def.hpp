#pragma once

#include "../point/point_id.hpp"
#include "../utils/types.hpp"
#include "../utils/vector2.hpp"
#include "constrain_flags.hpp"

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