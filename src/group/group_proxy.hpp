#pragma once

#include "../constrain/constrain_def.hpp"
#include "../constrain/constrain_proxy.hpp"
#include "../point/point_def.hpp"
#include "../point/point_proxy.hpp"
#include "../shell/shell.hpp"
#include "../shell/shell_def.hpp"
#include "../utils/types.hpp"
#include "../utils/vector2.hpp"
#include "group.hpp"

namespace soften {

class GroupProxy {
	friend class World;
	friend class GroupIterator;

  public:
	Idx pointsCount() const { return _groups[_idx].points.size(); }
	Idx constrainCount() const { return _groups[_idx].constrains.size(); }

	const PointProxy p(Id id) const;
	PointProxy p(Id id);

	const ConstrainProxy c(Id id) const;
	ConstrainProxy c(Id id);
	void fitConstrain(Id id);

	Id createPoint(const PointDef& p);
	Id createPoint(const Vec2& v, FlagsStorage flags = PointFlags::NONE, float m = 1.0f);

	Id createConstrain(const ConstrainDef& c);
	Id createConstrain(Id i, Id j, FlagsStorage flags = ConstrainFlags::NONE);

	void addShall(const ShellDef& def);

	bool isAll(FlagsStorage flags) const;

	const Rect2& aabb() const { return _groups[_idx].aabb; }

	const Vec2& center() const { return _groups[_idx].center; }

	uint32_t interactBits() { return _groups[_idx].interactBits; }
	void interactBits(uint32_t interactBits) { _groups[_idx].interactBits = interactBits; }

  private:
	GroupProxy(AABB2DTree<Group>& groups, uint idx): _groups(const_cast<AABB2DTree<Group>&>(groups)), _idx(idx) {}

  private:
	AABB2DTree<Group>& _groups;
	biss::index_t _idx;
};

inline Id GroupProxy::createPoint(const PointDef& p) {
	_groups[_idx].points.emplace_back(p.p1, p.p2, p.flags);

	return Id(_groups[_idx].points.size() - 1);
}

inline Id GroupProxy::createPoint(const Vec2& v, FlagsStorage flags, float m) {
	_groups[_idx].points.emplace_back(v, v, flags, m);

	return Id(_groups[_idx].points.size() - 1);
}

inline Id GroupProxy::createConstrain(const ConstrainDef& c) {
	float d = c.distance;
	if (isnanf(d)) {
		d = _groups[_idx].points[c.i].p2.distance(_groups[_idx].points[c.j].p2);
	}
	_groups[_idx].constrains.emplace_back(c.i, c.j, d, c.flags);

	return Id(_groups[_idx].constrains.size() - 1);
}

inline Id GroupProxy::createConstrain(Id i, Id j, FlagsStorage flags) {
	float d = _groups[_idx].points[i].p2.distance(_groups[_idx].points[j].p2);

	_groups[_idx].constrains.emplace_back(i, j, d, flags);

	return Id(_groups[_idx].constrains.size() - 1);
}

inline const PointProxy GroupProxy::p(Id id) const {
	return PointProxy(&_groups[_idx].points[id]);
}

inline PointProxy GroupProxy::p(Id id) {
	return PointProxy(&_groups[_idx].points[id]);
}

inline const ConstrainProxy GroupProxy::c(Id id) const {
	return ConstrainProxy(&_groups[_idx].constrains[id]);
}

inline ConstrainProxy GroupProxy::c(Id id) {
	return ConstrainProxy(&_groups[_idx].constrains[id]);
}

inline void GroupProxy::fitConstrain(Id id) {
	auto& c = _groups[_idx].constrains[id];

	c.distance = _groups[_idx].points[c.i].p2.distance(_groups[_idx].points[c.j].p2);
}

inline bool GroupProxy::isAll(FlagsStorage flags) const {
	for (const auto& p : _groups[_idx].points) {
		if (!p.is(flags)) {
			return false;
		}
	}
	return true;
}

} // namespace soften