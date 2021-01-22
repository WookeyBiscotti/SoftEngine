#pragma once

#include "../constrain/constrain_def.hpp"
#include "../constrain/constrain_id.hpp"
#include "../constrain/constrain_proxy.hpp"
#include "../point/point_def.hpp"
#include "../point/point_id.hpp"
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

inline PointIdx GroupProxy::createPoint(const PointDef& p) {
	_groups[_idx].points.emplace_back(p.p1, p.p2, p.flags);

	return PointIdx(_groups[_idx].points.size() - 1);
}

inline PointIdx GroupProxy::createPoint(const Vec2& v, FlagsStorage flags, float m) {
	_groups[_idx].points.emplace_back(v, v, flags, m);

	return PointIdx(_groups[_idx].points.size() - 1);
}

inline ConstrainIdx GroupProxy::createConstrain(const ConstrainDef& c) {
	float d = c.distance;
	if (isnanf(d)) {
		d = _groups[_idx].points[c.i.idx].p2.distance(_groups[_idx].points[c.j.idx].p2);
	}
	_groups[_idx].constrains.emplace_back(c.i.idx, c.j.idx, d, c.flags);

	return ConstrainIdx(_groups[_idx].constrains.size() - 1);
}

inline ConstrainIdx GroupProxy::createConstrain(PointIdx i, PointIdx j, FlagsStorage flags) {
	float d = _groups[_idx].points[i.idx].p2.distance(_groups[_idx].points[j.idx].p2);

	_groups[_idx].constrains.emplace_back(i.idx, j.idx, d, flags);

	return ConstrainIdx(_groups[_idx].constrains.size() - 1);
}

inline const PointProxy GroupProxy::p(PointIdx id) const {
	return PointProxy(&_groups[_idx].points[id.idx]);
}

inline PointProxy GroupProxy::p(PointIdx id) {
	return PointProxy(&_groups[_idx].points[id.idx]);
}

inline const ConstrainProxy GroupProxy::c(ConstrainIdx id) const {
	return ConstrainProxy(&_groups[_idx].constrains[id.id]);
}

inline ConstrainProxy GroupProxy::c(ConstrainIdx id) {
	return ConstrainProxy(&_groups[_idx].constrains[id.id]);
}

inline void GroupProxy::fitConstrain(ConstrainIdx id) {
	auto& c = _groups[_idx].constrains[id.id];

	c.distance = _groups[_idx].points[c.i].p2.distance(_groups[_idx].points[c.j].p2);
}

} // namespace soften