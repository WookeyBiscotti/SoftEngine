#pragma once

#include "../types.hpp"

namespace soften {

class Group;
struct GroupId {
	explicit GroupId(Id id): id(id) {}
	Id id;
};

} // namespace soften
