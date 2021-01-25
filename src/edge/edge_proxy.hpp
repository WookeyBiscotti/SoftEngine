#pragma once

#include "../point/point_id.hpp"
#include "edge.hpp"

namespace soften {

class EdgeProxy {
	friend class ShellProxy;

  public:
	Id i() const { return _edge->i; }
	Id j() const { return _edge->j; }

	float friction() const { return _edge->friction; }
	void friction(float friction) { _edge->friction = friction; }

  private:
	EdgeProxy(const Edge* edge): _edge(const_cast<Edge*>(edge)) {}

  private:
	Edge* _edge;
};

} // namespace soften