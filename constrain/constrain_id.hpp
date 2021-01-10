#pragma once

#include "../utils/types.hpp"

namespace soften {

struct ConstrainIdx {
	ConstrainIdx(Id id): id(id) {}
	Idx id;
};

} // namespace soften