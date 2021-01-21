#pragma once

#include "constrain/constrain.hpp"
#include "group/group.hpp"
#include "group/group_def.hpp"
#include "group/group_id.hpp"
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

		auto operator->() { return GroupProxy(&_it->second); }
		auto operator*() { return GroupProxy(&_it->second); }

		bool operator==(const GroupIterator& other) { return _it == other._it; }
		bool operator!=(const GroupIterator& other) { return _it != other._it; }

	  private:
		explicit GroupIterator(HashMap<Id, Group>::iterator it): _it(it) {}

		HashMap<Id, Group>::iterator _it;
	};

	GroupIterator begin() { return GroupIterator(_groups.begin()); }
	GroupIterator end() { return GroupIterator(_groups.end()); }

	void gravity(const Vec2& gravity) { _gravity = gravity; }
	const Vec2& gravity() const { return _gravity; }

	void update(float step, int iterations = 10);

	GroupId create(const GroupDef& def);
	GroupProxy get(GroupId id);

//	template<class Fn>
//	void queryShell(const Rect2& rect) {
//		//TODO: Optimize
//		for(const auto& g: _groups) {
//			if(isIntersecting(rect, g.))
//		}
//	}

	private:
	void updateAABB(Group& group);
	void updateCenter(Group& group);
	void updateShells(Group& group);
	void updatePosition(Group& group, float step);
	void updateConstrain(Group& group, float step);

  private:
	float _lastStep = std::numeric_limits<float>::signaling_NaN();
	Vec2 _gravity{0, 0};

	Id _currentGroupId = 0;
	HashMap<Id, Group> _groups;
};

} // namespace soften
