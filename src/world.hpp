#pragma once

#include "constrain/constrain.hpp"
#include "group/group.hpp"
#include "group/group_def.hpp"
#include "group/group_proxy.hpp"
#include "point/point.hpp"
#include "utils/types.hpp"

namespace soften {

class World {
  public:
	class GroupIterator {
		friend class World;

	  public:
		auto operator++() {
			_it++;
			return *this;
		}

		auto operator->() { return GroupProxy(_groups, _it.idx()); }
		auto operator*() { return GroupProxy(_groups, _it.idx()); }

		bool operator==(const GroupIterator& other) { return _it == other._it; }
		bool operator!=(const GroupIterator& other) { return _it != other._it; }

	  private:
		explicit GroupIterator(AABB2DTree<Group>& groups, AABB2DTree<Group>::Iterator it): _it(it), _groups(groups) {}

		AABB2DTree<Group>::Iterator _it;
		AABB2DTree<Group>& _groups;
	};

	World();

	auto begin() { return GroupIterator(_groups, _groups.begin()); }
	auto end() { return GroupIterator(_groups, _groups.end()); }

	void gravity(const Vec2& gravity) { _gravity = gravity; }
	const Vec2& gravity() const { return _gravity; }

	void update(float step, int iterations = 10);

	Id create(const GroupDef& def);
	GroupProxy get(Id id);

	void remove(Id id);

	Id create(const PointDef& point);
	Id create(const ConstrainDef& constrain);

	template<class Fn>
	void query(const Rect2& rect, Fn fn) {
		_groups.template query(rect, fn);
	}

  private:
	void updateAABB(Group& group);
	void updateCenter(Group& group);
	void updateShells(Group& group);
	void updatePosition(Group& group, float step);
	void updateConstrain(Group& group, float step);
	void updateGlobalPoints(float step);

	void interactPoint(Point& p);

  private:
	float _lastStep = std::numeric_limits<float>::signaling_NaN();
	Vec2 _gravity{0, 0};

	AABB2DTree<Group> _groups;
	AABB2DTree<Point> _globalPoints;
	Index<Constrain> _constrains;
};

} // namespace soften
