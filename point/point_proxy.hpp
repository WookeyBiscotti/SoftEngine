#pragma once

#include "../types.hpp"
#include "point.hpp"

namespace soften {

class PointProxy final {
	friend struct Group;
	friend struct GroupProxy;

  public:
	Vec2 position() const;
	void position(const Vec2& position);

	Vec2 curr() const;
	void curr(const Vec2& position) const;

	Vec2 prev() const;
	void prev(const Vec2& position) const;

	FlagsStorage flags() const;
	void flags(FlagsStorage flags);

	float m() const { return _point->m; }
	void m(float m) { _point->m = m; }

  private:
	explicit PointProxy(const Point* point): _point(const_cast<Point*>(point)) {}

  private:
	Point* _point;
};

inline FlagsStorage PointProxy::flags() const {
	return _point->flags;
}

inline void PointProxy::flags(FlagsStorage flags) {
	_point->flags = flags;
}

inline Vec2 PointProxy::position() const {
	return _point->p2;
}

inline void PointProxy::position(const Vec2& position) {
	_point->p2 = position;
	_point->p1 = position;
}

inline Vec2 PointProxy::curr() const {
	return position();
}

inline void PointProxy::curr(const Vec2& position) const {
	_point->p2 = position;
}

inline Vec2 PointProxy::prev() const {
	return _point->p1;
}

inline void PointProxy::prev(const Vec2& position) const {
	_point->p1 = position;
}

} // namespace soften