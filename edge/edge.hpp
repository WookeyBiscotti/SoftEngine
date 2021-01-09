#pragma once

#include "../types.hpp"

namespace soften {

struct Edge {
	Idx i;
	Idx j;
	float friction;
};

} // namespace soften