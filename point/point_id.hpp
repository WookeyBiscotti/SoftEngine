#pragma once

#include "../types.hpp"

namespace soften {

struct PointId {
	explicit PointId(Id id): id(id) {}
	Id id;
};

} // namespace soften