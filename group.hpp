#pragma once

#include "constrain/constrain.hpp"
#include "constrain/constrain_def.hpp"
#include "constrain/constrain_id.hpp"
#include "constrain/constrain_proxy.hpp"
#include "point/point.hpp"
#include "point/point_def.hpp"
#include "point/point_id.hpp"
#include "point/point_proxy.hpp"
#include "types.hpp"

namespace soften {

class World;

class Group final {
	friend class World;

  public:
	World& world();

	const PointProxy p(PointId id) const;
	PointProxy p(PointId id);

	const ConstrainProxy c(ConstrainId id) const;
	ConstrainProxy c(ConstrainId id);

	PointId createPoint(const PointDef& p);

	ConstrainId createConstrain(const ConstrainDef& c);

  private:
	Group(World& world): _world(world) {}

  private:
	DynArray<Constrain> _constrains;
	DynArray<Point> _points;

	World& _world;
};

World& Group::world() {
	return _world;
}

PointId Group::createPoint(const PointDef& p) {
	_points.emplace_back(p.p1, p.p2, p.flags);

	return PointId(_points.size() - 1);
}

ConstrainId Group::createConstrain(const ConstrainDef& c) {
	float d = c.distance;
	if (isnanf(d)) {
		d = _points[c.i.id].p2.distance(_points[c.j.id].p2);
	}
	_constrains.emplace_back(c.i.id, c.j.id, d, c.flags);

	return ConstrainId(_constrains.size() - 1);
}

const PointProxy Group::p(PointId id) const {
	return PointProxy(_points[id.id]);
}

PointProxy Group::p(PointId id) {
	return PointProxy(_points[id.id]);
}

const ConstrainProxy Group::c(ConstrainId id) const {
	return ConstrainProxy(_constrains[id.id]);
}

ConstrainProxy Group::c(ConstrainId id) {
	return ConstrainProxy(_constrains[id.id]);
}

} // namespace soften