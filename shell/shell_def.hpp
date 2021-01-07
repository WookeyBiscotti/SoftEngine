#pragma once

#include "../edge/edge_def.hpp"

namespace soften {

struct ShellDef {
	DynArray<EdgeDef> edges;
};

} // namespace soften