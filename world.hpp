#pragma once

#include "constrain/constrain.hpp"
#include "point/point.hpp"

#include <vector>

namespace soften {

class World {
  public:
	void gravity(const Vec2& gravity) { _gravity = gravity; }

	const Vec2& gravity() const { return _gravity; }

	void update(float step);

	const std::vector<Point>& points() const { return _points; }

	const auto& constrains() const { return _constrains; }

	auto& c(std::size_t i) { return _constrains[i]; }

	auto& p(std::size_t i) { return _points[i]; }

	void resetConstrain(std::size_t i) {
		auto& c = _constrains[i];

		c.distance = _points[c.i].p2.distance(_points[c.j].p2);
	}

	std::size_t addPoint(const Point& point) {
		_points.push_back(point);
		return _points.size() - 1;
	}

	std::size_t addPoint(float x, float y, FlagsStorage flags = PointFlags::NONE) {
		_points.emplace_back(Vec2(x, y), flags);
		return _points.size() - 1;
	}

	void addBorder(const Vec2& a, const Vec2& b) { _borders.emplace_back(a, b); }

	std::size_t addConstrain(std::size_t i, std::size_t j) {
		addConstrain(i, j, _points[i].p2.distance(_points[j].p2));
		return _constrains.size() - 1;
	}

	void addConstrain(std::size_t i, std::size_t j, float distance) {
		_constrains.emplace_back(Constrain{i, j, distance});
	}

  private:
	float _lastStep = std::numeric_limits<float>::signaling_NaN();
	Vec2 _gravity{0, 0};

	std::vector<Point> _points;

	std::vector<std::pair<Vec2, Vec2>> _borders;
	std::vector<Constrain> _constrains;
};


} // namespace soften
