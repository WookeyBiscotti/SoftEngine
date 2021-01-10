#pragma once

#include "../utils/types.hpp"

namespace soften {

struct PointIdx {
	PointIdx(Id id): idx(id) {}
	Idx idx;
};

} // namespace soften