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

  private:
	explicit PointProxy(const Point& point): _point(point) {}
	PointProxy() = delete;
	PointProxy(const PointProxy&) = delete;

  private:
	const Point& _point;
};

inline FlagsStorage PointProxy::flags() const {
	return _point.flags;
}

inline void PointProxy::flags(FlagsStorage flags) {
	const_cast<Point&>(_point).flags = flags;
}

inline Vec2 PointProxy::position() const {
	return _point.p2;
}

inline void PointProxy::position(const Vec2& position) {
	const_cast<Point&>(_point).p2 = position;
	const_cast<Point&>(_point).p1 = position;
}

inline Vec2 PointProxy::curr() const {
	return position();
}

inline void PointProxy::curr(const Vec2& position) const {
	const_cast<Point&>(_point).p2 = position;
}

inline Vec2 PointProxy::prev() const {
	return _point.p1;
}

inline void PointProxy::prev(const Vec2& position) const {
	const_cast<Point&>(_point).p1 = position;
}

} // namespace soften