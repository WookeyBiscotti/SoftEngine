#pragma once

#include "../types.hpp"

namespace soften {

struct ConstrainFlags {
	enum : FlagsStorage {
		NONE = 0,
		DISABLE = 1 << 0,
	};
};

} // namespace soften