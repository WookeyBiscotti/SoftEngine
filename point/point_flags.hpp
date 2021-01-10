#pragma once

#include "../utils/types.hpp"

namespace soften {

struct PointFlags {
	enum : FlagsStorage {
		NONE = 0,
		STATIC = 1 << 0,
		DISABLE = 1 << 1,
		INTERACTIVE = 1 << 2,
		SHELL = 1 << 3,
	};
};

} // namespace soften