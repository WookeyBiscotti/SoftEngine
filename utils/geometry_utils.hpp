#pragma once

#include "rect2.hpp"

namespace soften {

inline bool isIntersecting(const Rect2& aa, const Rect2& bb) {
	const Vec2 aaMax = aa.position + aa.size;
	const Vec2 bbMax = bb.position + bb.size;

	return aa.position.x <= bbMax.x && aaMax.x >= bb.position.x && aa.position.y <= bbMax.y && aaMax.y >= bb.position.y;
}

inline bool isIntersecting(const Rect2& aa, const Vec2& p) {
	const Vec2 aaMax = aa.position + aa.size;
	return aa.position.x <= p.x && aaMax.x >= p.x && aa.position.y <= p.y && aaMax.y >= p.y;
}

} // namespace soften
