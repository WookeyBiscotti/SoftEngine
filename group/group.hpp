#pragma once

#include "../constrain/constrain.hpp"
#include "../point/point.hpp"
#include "../types.hpp"

namespace soften {

struct Group {
	DynArray<Constrain> constrains;
	DynArray<Point> points;
};

} // namespace soften