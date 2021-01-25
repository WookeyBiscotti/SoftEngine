#pragma once

#include "constrain_flags.hpp"

namespace soften {
struct Constrain {
	Constrain(Id i, Id j, float distance, FlagsStorage flags = ConstrainFlags::NONE, float hookCoeff = 0.0f,
	    float breakDistance = 0.0f):
	    i(i),
	    j(j), distance(distance), flags(flags), hookCoeff(hookCoeff) {}
	Constrain() = default;

	bool is(FlagsStorage flag) const { return this->flags & flag; }

	Id i;
	Id j;
	float distance;
	float breakDistance;
	float hookCoeff;
	FlagsStorage flags;
};
} // namespace soften