#pragma once

#include "../point/point_id.hpp"
#include "edge.hpp"

namespace soften {

class EdgeProxy {
	friend class ShellProxy;

  public:
	PointIdx i() const { return _edge->i; }
	PointIdx j() const { return _edge->j; }

	float friction() const { return _edge->friction; }
	void friction(float friction) { _edge->friction = friction; }

  private:
	EdgeProxy(const Edge* edge): _edge(const_cast<Edge*>(edge)) {}

  private:
	Edge* _edge;
};

} // namespace soften