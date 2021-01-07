#pragma once

#include "../edge/edge.hpp"

namespace soften {

struct Shell {
	DynArray<Edge> edges;
	Vec2 center;
};

} // namespace soften