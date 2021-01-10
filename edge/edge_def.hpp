#pragma once

#include "../utils/types.hpp"

namespace soften {

struct EdgeDef {
	Idx i;
	Idx j;
	float friction = 0.01f;
};

} // namespace soften