#pragma once

#include "../point/point_id.hpp"
#include "edge.hpp"

namespace soften {

class EdgeProxy {
	friend class ShellProxy;

  public:
	PointIdx i() const { return _edge->i; }
	PointIdx j() const { return _edge->j; }

  private:
	EdgeProxy(const Edge* edge): _edge(edge) {}

  private:
	const Edge* _edge;
};

} // namespace soften