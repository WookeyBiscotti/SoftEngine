#pragma once

#include "../constrain/constrain.hpp"
#include "../point/point.hpp"
#include "../rect2.hpp"
#include "../shell/shell.hpp"
#include "../types.hpp"
#include "group_id.hpp"

namespace soften {

struct Group {
	DynArray<Constrain> constrains;
	DynArray<Point> points;
	Shell shell;

	Rect2 aabb{};

	GroupId id;
};

} // namespace soften