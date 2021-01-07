#pragma once

#include "../types.hpp"

namespace soften {

struct ConstrainFlags {
	enum : FlagsStorage {
		NONE = 0,
		DISABLE = 1 << 0,
		WORKS_IF_GREATER = 1 << 1,
	};
};

} // namespace soften