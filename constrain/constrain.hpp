#pragma once

#include "constrain_flags.hpp"

namespace soften {
struct Constrain {
	Constrain(Id i, Id j, float distance, FlagsStorage flags = ConstrainFlags::NONE):
	    i(i), j(j), distance(distance), flags(flags) {}
	Constrain() = default;

	bool is(FlagsStorage flag) const { return this->flags & flag; }

	Id i;
	Id j;
	float distance;
	FlagsStorage flags;
};
} // namespace soften