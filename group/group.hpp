#pragma once

#include "../constrain/constrain.hpp"
#include "../point/point.hpp"
#include "../shell/shell.hpp"
#include "../utils/rect2.hpp"
#include "../utils/types.hpp"
#include "group_id.hpp"

namespace soften {

struct Group {
	DynArray<Constrain> constrains;
	DynArray<Point> points;
	Shell shell;

	Rect2 aabb;
	Vec2 center;

	GroupId id;
	uint32_t interactBits = std::numeric_limits<uint32_t>::max();
};

} // namespace soften