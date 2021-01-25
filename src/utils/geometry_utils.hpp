#pragma once

#include "rect2.hpp"

namespace soften {

inline bool isIntersecting(const Rect2& aa, const Rect2& bb) {
	return aa.lb.x <= bb.ub.x && aa.ub.x >= bb.lb.x && aa.lb.y <= bb.ub.y && aa.ub.y >= bb.lb.y;
}

inline bool isIntersecting(const Rect2& aa, const Vec2& p) {
	return aa.lb.x <= p.x && aa.ub.x >= p.x && aa.lb.y <= p.y && aa.ub.y >= p.y;
}

} // namespace soften
