#pragma once

#include "../types.hpp"

namespace soften {

struct PointFlags {
	enum : FlagsStorage {
		NONE = 0,
		STATIC = 1 << 0,
		SLEEP = 1 << 1,
	};
};

} // namespace soften