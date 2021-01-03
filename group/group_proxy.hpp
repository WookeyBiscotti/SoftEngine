#pragma once

#include "../constrain/constrain_id.hpp"
#include "../constrain/constrain_def.hpp"
#include "../constrain/constrain_proxy.hpp"
#include "../point/point_def.hpp"
#include "../point/point_id.hpp"
#include "../point/point_proxy.hpp"
#include "../types.hpp"
#include "group.hpp"

namespace soften {

class GroupProxy {
	friend class World;
	friend class GroupIterator;

  public:
	Idx pointsCount() const { return _group.points.size(); }
	Idx constrainCount() const { return _group.constrains.size(); }

	const PointProxy p(PointIdx id) const;
	PointProxy p(PointIdx id);

	const ConstrainProxy c(ConstrainIdx id) const;
	ConstrainProxy c(ConstrainIdx id);
	void fitConstrain(ConstrainIdx id);

	PointIdx createPoint(const PointDef& p);

	ConstrainIdx createConstrain(const ConstrainDef& c);

  private:
	explicit GroupProxy(const Group& group): _group(group) {}

  private:
	const Group& _group;
};

inline PointIdx GroupProxy::createPoint(const PointDef& p) {
	const_cast<Group&>(_group).points.emplace_back(p.p1, p.p2, p.flags);

	return PointIdx(_group.points.size() - 1);
}

inline ConstrainIdx GroupProxy::createConstrain(const ConstrainDef& c) {
	float d = c.distance;
	if (isnanf(d)) {
		d = _group.points[c.i.idx].p2.distance(_group.points[c.j.idx].p2);
	}
	const_cast<Group&>(_group).constrains.emplace_back(c.i.idx, c.j.idx, d, c.flags);

	return ConstrainIdx(_group.constrains.size() - 1);
}

inline const PointProxy GroupProxy::p(PointIdx id) const {
	return PointProxy(_group.points[id.idx]);
}

inline PointProxy GroupProxy::p(PointIdx id) {
	return PointProxy(_group.points[id.idx]);
}

inline const ConstrainProxy GroupProxy::c(ConstrainIdx id) const {
	return ConstrainProxy(_group.constrains[id.id]);
}

inline ConstrainProxy GroupProxy::c(ConstrainIdx id) {
	return ConstrainProxy(_group.constrains[id.id]);
}

inline void GroupProxy::fitConstrain(ConstrainIdx id) {
	auto& c = const_cast<Group&>(_group).constrains[id.id];

	c.distance = _group.points[c.i].p2.distance(_group.points[c.j].p2);
}

} // namespace soften