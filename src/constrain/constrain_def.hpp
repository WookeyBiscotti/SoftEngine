#pragma once

#include "../utils/types.hpp"
#include "../utils/vector2.hpp"
#include "constrain_flags.hpp"

namespace soften {

struct ConstrainDef {
	ConstrainDef(Id i, Id j, float distance = FLOAT_NAN, FlagsStorage flags = ConstrainFlags::NONE):
	    i(i), j(j), distance(distance), flags(flags) {}

	Id i;
	Id j;
	float distance;
	FlagsStorage flags;
};

} // namespace soften