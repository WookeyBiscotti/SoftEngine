#include "world.hpp"

#include <cmath>
#include <wykobi.hpp>

using namespace soften;

// TODO: optimise
static bool isIntersecting(const Vec2& p1, const Vec2& p2, const Vec2& q1, const Vec2& q2, Vec2& inter) {
	wykobi::point2d<float> p;
	if (wykobi::intersect((wykobi::point2d<float>)p1, (wykobi::point2d<float>)p2, (wykobi::point2d<float>)q1,
	        (wykobi::point2d<float>)q2, p)) {
		inter.x = p.x;
		inter.y = p.y;

		return true;
	}

	return false;
}

GroupId World::create(const GroupDef& def) {
	_groups[_currentGroupId];
	return GroupId(_currentGroupId++);
}

GroupProxy World::get(GroupId id) {
	return GroupProxy(_groups[id.id]);
}

void World::update(float step) {
	if (std::isnan(_lastStep)) {
		_lastStep = step;
	}

	const auto stepRatio = (step / _lastStep);

	for (auto& [_, g] : _groups) {
		for (auto& p : g.points) {
			if (p.is(PointFlags::STATIC)) {
				continue;
			}
			auto p2 = p.p2 + (p.p2 - p.p1) * stepRatio + (_gravity * step * step);
			p.p1 = p.p2;
			p.p2 = p2;
		}

		for (int rr = 0; rr < 10; ++rr) {

			for (const auto& c : g.constrains) {
				if (c.is(ConstrainFlags::DISABLE)) {
					continue;
				}

				auto& pa = g.points[c.i];
				auto& pb = g.points[c.j];

				const auto t = pa.p2 - pb.p2;
				auto invD = t.fastInvLength();
				const auto dT = 0.5f * t * (c.distance * invD - 1.0f);

				if (pa.is(PointFlags::STATIC)) {
					if (pb.is(PointFlags::STATIC)) {
						continue;
					} else {
						pb.p2 -= 2 * dT;
					}
				} else {
					if (pb.is(PointFlags::STATIC)) {
						pa.p2 += 2 * dT;
					} else {
						pa.p2 += dT;
						pb.p2 -= dT;
					}
				}
			}
		}

		for (auto& p : g.points) {
			if (p.is(PointFlags::STATIC)) {
				continue;
			}
			for (const auto& [a, b] : _borders) {
				Vec2 o;
				if (isIntersecting(a, b, p.p1, p.p2, o)) {
					const auto l = Vec2(a - b).fastN();
					auto p1o = o - p.p1;
					auto p2o = o - p.p2;
					p.p1 = o + (p1o - 2 * l * (p1o * l));
					p.p2 = o + (p2o - 2 * l * (p2o * l));
				}
			}
		}
	}
}
