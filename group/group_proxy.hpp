#pragma once

#include "../constrain/constrain_def.hpp"
#include "../constrain/constrain_id.hpp"
#include "../constrain/constrain_proxy.hpp"
#include "../point/point_def.hpp"
#include "../point/point_id.hpp"
#include "../point/point_proxy.hpp"
#include "../shell/shell.hpp"
#include "../shell/shell_def.hpp"
#include "../types.hpp"
#include "../vector2.hpp"
#include "group.hpp"

namespace soften {

class GroupProxy {
	friend class World;
	friend class GroupIterator;

  public:
	Idx pointsCount() const { return _group->points.size(); }
	Idx constrainCount() const { return _group->constrains.size(); }

	const PointProxy p(PointIdx id) const;
	PointProxy p(PointIdx id);

	const ConstrainProxy c(ConstrainIdx id) const;
	ConstrainProxy c(ConstrainIdx id);
	void fitConstrain(ConstrainIdx id);

	PointIdx createPoint(const PointDef& p);
	PointIdx createPoint(const Vec2& v, FlagsStorage flags = PointFlags::NONE, float m = 1.0f);

	ConstrainIdx createConstrain(const ConstrainDef& c);
	ConstrainIdx createConstrain(PointIdx i, PointIdx j, FlagsStorage flags = ConstrainFlags::NONE);

	void addShall(const ShellDef& def);

	const Rect2& aabb() const { return _group->aabb; }

  private:
	explicit GroupProxy(const Group* group): _group(const_cast<Group*>(group)) {}

  private:
	Group* _group;
};

inline PointIdx GroupProxy::createPoint(const PointDef& p) {
	_group->points.emplace_back(p.p1, p.p2, p.flags);

	return PointIdx(_group->points.size() - 1);
}

inline PointIdx GroupProxy::createPoint(const Vec2& v, FlagsStorage flags, float m) {
	_group->points.emplace_back(v, v, flags, m);

	return PointIdx(_group->points.size() - 1);
}

inline ConstrainIdx GroupProxy::createConstrain(const ConstrainDef& c) {
	float d = c.distance;
	if (isnanf(d)) {
		d = _group->points[c.i.idx].p2.distance(_group->points[c.j.idx].p2);
	}
	_group->constrains.emplace_back(c.i.idx, c.j.idx, d, c.flags);

	return ConstrainIdx(_group->constrains.size() - 1);
}

inline ConstrainIdx GroupProxy::createConstrain(PointIdx i, PointIdx j, FlagsStorage flags) {
	float d = _group->points[i.idx].p2.distance(_group->points[j.idx].p2);

	_group->constrains.emplace_back(i.idx, j.idx, d, flags);

	return ConstrainIdx(_group->constrains.size() - 1);
}

inline const PointProxy GroupProxy::p(PointIdx id) const {
	return PointProxy(&_group->points[id.idx]);
}

inline PointProxy GroupProxy::p(PointIdx id) {
	return PointProxy(&_group->points[id.idx]);
}

inline const ConstrainProxy GroupProxy::c(ConstrainIdx id) const {
	return ConstrainProxy(_group->constrains[id.id]);
}

inline ConstrainProxy GroupProxy::c(ConstrainIdx id) {
	return ConstrainProxy(_group->constrains[id.id]);
}

inline void GroupProxy::fitConstrain(ConstrainIdx id) {
	auto& c = _group->constrains[id.id];

	c.distance = _group->points[c.i].p2.distance(_group->points[c.j].p2);
}

} // namespace soften