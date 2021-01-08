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

static bool isIntersecting(const Rect2& aa, const Rect2& bb) {
	const Vec2 aaMax = aa.position + aa.size;
	const Vec2 bbMax = bb.position + bb.size;

	return aa.position.x <= bbMax.x && aaMax.x >= bb.position.x && aa.position.y <= bbMax.y && aaMax.y >= bb.position.y;
}

static bool isIntersecting(const Rect2& aa, const Vec2& p) {
	const Vec2 aaMax = aa.position + aa.size;
	return aa.position.x <= p.x && aaMax.x >= p.x && aa.position.y <= p.y && aaMax.y >= p.y;
}

// TODO: optimise
static bool isIntersecting(const decltype(Shell::edges)& edges, const DynArray<Point>& points, const Vec2& p) {
	if (edges.empty()) {
		return false;
	}

	int count = 0;

	Vec2 inf(std::numeric_limits<float>::max(), 0);

	for (size_t i = 0; i != edges.size(); ++i) {
		const auto& e = edges[i];
		const Vec2& l1 = points[e.i].p2;
		const Vec2& l2 = points[e.j].p2;

		Vec2 _;
		if (isIntersecting(l1, l2, p, inf, _)) {
			++count;
		}
	}

	return count % 2 == 1;
}

GroupId World::create(const GroupDef& def) {
	_groups[_currentGroupId].id.id = _currentGroupId;
	return GroupId{_currentGroupId++};
}

GroupProxy World::get(GroupId id) {
	return GroupProxy(&_groups[id.id]);
}

void World::update(float step) {
	if (std::isnan(_lastStep)) {
		_lastStep = step;
	}

	for (auto& [_, g] : _groups) {
		updatePosition(g, step);
		//
		//		for (auto& p : g.points) {
		//			if (p.is(PointFlags::STATIC)) {
		//				continue;
		//			}
		//			for (const auto& [a, b] : _borders) {
		//				Vec2 o;
		//				if (isIntersecting(a, b, p.p1, p.p2, o)) {
		//					const auto l = Vec2(a - b).fastN();
		//					auto p1o = o - p.p1;
		//					auto p2o = o - p.p2;
		//					p.p1 = o + (p1o - 2 * l * (p1o * l));
		//					p.p2 = o + (p2o - 2 * l * (p2o * l));
		//				}
		//			}
		//		}

		//		updateAABB(g);
	}
	for (int rr = 0; rr < 3; ++rr) {
		for (auto& [_, g] : _groups) {
			updateConstrain(g);
		}
	}
	for (auto& [_, g] : _groups) {
		updateAABB(g);
	}
	for (int rr = 0; rr < 1; ++rr) {
		for (auto& [_, g] : _groups) {
			updateShells(g);
		}
	}
	for (int rr = 0; rr < 1; ++rr) {
		for (auto& [_, g] : _groups) {
			updateConstrain(g);
		}
	}

	for (int rr = 0; rr < 1; ++rr) {
		for (auto& [_, g] : _groups) {
			updateAABB(g);
			updateShells(g);
		}
	}

	for (auto& [_, g] : _groups) {
		updateAABB(g);
	}
	//
	//	for (auto& [_, g] : _groups) {
	//		updateShells(g);
	//	}
	//
	//	for (auto& [_, g] : _groups) {
	//		for (int rr = 0; rr < 10; ++rr) {
	//			updateConstrain(g);
	//		}
	//	}
}

void World::updateAABB(Group& g) {
	// TODO: optimize
	if (g.points.empty()) {
		return;
	}

	Vec2 aa{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
	    bb{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
	bool initAABB = false;
	for (auto& p : g.points) {
		if (p.flags & PointFlags::DISABLE) {
			continue;
		}
		initAABB = true;
		if (p.p2.x < aa.x) {
			aa.x = p.p2.x;
		}
		if (p.p2.y < aa.y) {
			aa.y = p.p2.y;
		}
		if (p.p2.x > bb.x) {
			bb.x = p.p2.x;
		}
		if (p.p2.y > bb.y) {
			bb.y = p.p2.y;
		}
	}

	if (initAABB) {
		g.aabb.position = aa;
		g.aabb.size = bb - aa;
	} else {
		g.aabb = {};
	}
}

void World::updateShells(Group& group) {
	// TODO: optimize!!!
	//	for (auto& [_, g] : _groups) {
	//		if (g.shell.edges.empty()) {
	//			continue;
	//		}
	//		Vec2 center{0, 0};
	//		for (const auto& e : g.shell.edges) {
	//			center += g.points[e.i].p2;
	//		}
	//		g.shell.center = center / g.shell.edges.size();
	//	}

	for (auto& [_, other] : _groups) {
		if (&other == &group || other.shell.edges.empty() || !isIntersecting(other.aabb, group.aabb)) {
			continue;
		}

		for (auto& p : group.points) {
			if (!(p.flags & PointFlags::INTERACTIVE) || !isIntersecting(other.aabb, p.p2) ||
			    !isIntersecting(other.shell.edges, other.points, p.p2)) {
				continue;
			}

			auto& edges = other.shell.edges;
			auto& points = other.points;

			float minPenetration = std::numeric_limits<float>::max();
			size_t minIdx = -1;
			Vec2 minN;

			for (size_t i = 0; i != edges.size(); ++i) {
				const auto& e = edges[i];
				const Vec2& l1 = points[e.i].p2;
				const Vec2& l2 = points[e.j].p2;

				if (points[e.i].is(PointFlags::STATIC) && points[e.j].is(PointFlags::STATIC) &&
				    p.is(PointFlags::STATIC)) {
					continue;
				}

				const Vec2& l = (l2 - l1);
				const Vec2& n = Vec2(l.y, -l.x).fastN();
				const auto penetration = (l2 - p.p2) * n;
				if (penetration < minPenetration) {
					minPenetration = penetration;
					minN = n;
					minIdx = i;
				}
			}
			if (minIdx == -1) {
				continue;
			}

			// TODO: Change to more physics formula
			const auto dp = minPenetration * minN;
			const auto& e = edges[minIdx];
			auto& l1 = points[e.i];
			auto& l2 = points[e.j];

			const auto& l = (l2.p2 - l1.p2).fastN();
			const float& lInvLen = l.fastInvLength();
			const float a1 = l * (p.p2 - l1.p2).fastN();
			const float a2 = l * (l2.p2 - p.p2).fastN();

			const float staticM = 1000 * std::max(std::max(l1.m, l2.m), p.m);
			float l1m = l1.is(PointFlags::STATIC) ? staticM : (a2 / (a1 + a2)) * l1.m;
			float l2m = l2.is(PointFlags::STATIC) ? staticM : (a1 / (a1 + a2)) * l2.m;
			float pm = p.is(PointFlags::STATIC) ? staticM : p.m;

			const float devider = (l1m * l2m + (l1m + l2m) * pm);
			if (!l1.is(PointFlags::STATIC)) {
				l1.p2 -= dp * pm * l2m / devider;
			}
			if (!l2.is(PointFlags::STATIC)) {
				l2.p2 -= dp * l2m * pm / devider;
			}
			if (!p.is(PointFlags::STATIC)) {
				p.p2 += dp * l1m * l2m / devider;
			}
		}

		for (auto& edge : group.shell.edges) {
			const auto ratio = 0.5f;
			//			const auto ratio = rand() % 1000 / 1000.0f;
			auto p = ratio * group.points[edge.i].p2 + (1 - ratio) * group.points[edge.j].p2;

			if (!isIntersecting(other.aabb, p) || !isIntersecting(other.shell.edges, other.points, p)) {
				continue;
			}

			auto& edges = other.shell.edges;
			auto& points = other.points;
			auto& p1 = group.points[edge.i];
			auto& p2 = group.points[edge.j];

			float minPenetration = std::numeric_limits<float>::max();
			size_t minIdx = -1;
			Vec2 minN;

			for (size_t i = 0; i != edges.size(); ++i) {
				const auto& e = edges[i];
				const Vec2& l1 = points[e.i].p2;
				const Vec2& l2 = points[e.j].p2;

				if (points[e.i].is(PointFlags::STATIC) && points[e.j].is(PointFlags::STATIC) &&
				    p1.is(PointFlags::STATIC) && p2.is(PointFlags::STATIC)) {
					continue;
				}

				const Vec2& l = (l2 - l1);
				const Vec2& n = Vec2(l.y, -l.x).fastN();

				const auto penetration = (l2 - p) * n;

				if (penetration < minPenetration) {
					minPenetration = penetration;
					minN = n;
					minIdx = i;
				}
			}

			if (minIdx == -1) {
				continue;
			}

			// TODO: Change to more physics formula
			const auto dp = minPenetration * minN;

			const auto& e = edges[minIdx];
			auto& l1 = points[e.i];
			auto& l2 = points[e.j];

			const float staticM = 1000 * std::max(std::max(std::max(l1.m, l2.m), p1.m), p2.m);
			float l1m = l1.is(PointFlags::STATIC) ? staticM : l1.m;
			float l2m = l2.is(PointFlags::STATIC) ? staticM : l2.m;
			float p1m = p1.is(PointFlags::STATIC) ? staticM : p1.m;
			float p2m = p2.is(PointFlags::STATIC) ? staticM : p2.m;

			const float devider = (l1m * l2m * p1m + l1m * l2m * p2m + l1m * p1m * p2m + l2m * p1m * p2m);
			if (!l1.is(PointFlags::STATIC)) {
				l1.p2 -= dp * l2m * p1m * p2m / devider;
			}
			if (!l2.is(PointFlags::STATIC)) {
				l2.p2 -= dp * l1m * p1m * p2m / devider;
			}
			if (!p1.is(PointFlags::STATIC)) {
				p1.p2 += dp * l1m * l2m * p2m / devider;
			}
			if (!p2.is(PointFlags::STATIC)) {
				p2.p2 += dp * l1m * l2m * p1m / devider;
			}
		}
	}
}

void World::updatePosition(Group& group, float step) {
	const auto stepRatio = (step / _lastStep);

	for (auto& p : group.points) {
		if (p.is(PointFlags::STATIC)) {
			continue;
		}
		auto p2 = p.p2 + (p.p2 - p.p1) * stepRatio + (_gravity * step * step);
		p.p1 = p.p2;
		p.p2 = p2;
	}
}

void World::updateConstrain(Group& group) {
	for (const auto& c : group.constrains) {
		if (c.is(ConstrainFlags::DISABLE)) {
			continue;
		}

		auto& pa = group.points[c.i];
		auto& pb = group.points[c.j];

		const auto t = pa.p2 - pb.p2;
		auto invD = t.fastInvLength();

		if (c.is(ConstrainFlags::WORKS_IF_GREATER) && invD * c.distance > 1.0f) {
			continue;
		}

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
				pa.p2 += dT * pb.m / (pa.m + pb.m);
				pb.p2 -= dT * pa.m / (pa.m + pb.m);
			}
		}
	}
}
