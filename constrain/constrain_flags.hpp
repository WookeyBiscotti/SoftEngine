#pragma once

#include "../utils/types.hpp"

namespace soften {

struct ConstrainFlags {
	enum : FlagsStorage {
		NONE = 0,
		DISABLE = 1 << 0,

		WORKS_IF_GREATER = 1 << 1,
		WORKS_IF_LOWER = 1 << 2,

		USE_HOOK_COEFF = 1 << 3,

		BREAK_IF_GREATER = 1 << 4,
		BREAK_IF_LOWER = 1 << 5,
	};
};

} // namespace soften