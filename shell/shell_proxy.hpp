#pragma once

#include "../edge/edge_id.hpp"
#include "../edge/edge_proxy.hpp"
#include "shell.hpp"

namespace soften {

class ShellProxy {
	friend class ShellIterator;

  public:
	const EdgeProxy e(EdgeIdx idx) const { return EdgeProxy(&_shell->edges[idx.idx]); }

  private:
	ShellProxy(const Shell* shell): _shell(shell) {}

  private:
	const Shell* _shell;
};

} // namespace soften