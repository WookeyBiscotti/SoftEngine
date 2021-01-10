#pragma once

#include "../utils/types.hpp"

namespace soften {

struct Edge {
	Idx i;
	Idx j;
	float friction;
};

} // namespace soften