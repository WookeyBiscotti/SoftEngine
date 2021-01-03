#pragma once

#include "constrain/constrain.hpp"
#include "group/group.hpp"
#include "group/group_def.hpp"
#include "group/group_id.hpp"
#include "group/group_proxy.hpp"
#include "point/point.hpp"
#include "types.hpp"

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

		auto operator->() { return GroupProxy(_it->second); }
		auto operator*() { return GroupProxy(_it->second); }

		bool operator==(const GroupIterator& other) { return _it == other._it; }
		bool operator!=(const GroupIterator& other) { return _it != other._it; }

	  private:
		GroupIterator(HashMap<Id, Group>::iterator it): _it(it) {}

		HashMap<Id, Group>::iterator _it;
	};

	GroupIterator begin() { return _groups.begin(); }
	GroupIterator end() { return _groups.end(); }

	void gravity(const Vec2& gravity) { _gravity = gravity; }

	const Vec2& gravity() const { return _gravity; }

	void update(float step);

	void addBorder(const Vec2& a, const Vec2& b) { _borders.emplace_back(a, b); }

	GroupId create(const GroupDef& def);
	GroupProxy get(GroupId id);

  private:
	float _lastStep = std::numeric_limits<float>::signaling_NaN();
	Vec2 _gravity{0, 0};

	std::vector<std::pair<Vec2, Vec2>> _borders;

	Id _currentGroupId = 0;
	HashMap<Id, Group> _groups;
};

} // namespace soften
