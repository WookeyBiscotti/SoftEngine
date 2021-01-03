#pragma once

#include "../types.hpp"

namespace soften {

struct ConstrainId {
	explicit ConstrainId(Id id): id(id) {}
	Id id;
};

} // namespace soften