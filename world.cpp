#include "world.hpp"

#include "utils/geometry_utils.hpp"

#include <cmath>
#include <wykobi.hpp>

using namespace soften;

namespace biss {

template<>
float get<Vec2, float>(uint i, const Vec2& v) {
	if (i == 0) {
		return v.x;
	} else {
		return v.y;
	}
}

template<>
float get_lb<Rect2, float>(uint i, const Rect2& aabb) {
	return get<Vec2, float>(i, aabb.lb);
}

template<>
float get_ub<Rect2, float>(uint i, const Rect2& aabb) {
	return get<Vec2, float>(i, aabb.ub);
}

} // namespace biss

static bool isIntersecting(const Vec2& p1, const Vec2& p2, const Vec2& q1, const Vec2& q2) {
	if (wykobi::intersect((wykobi::point2d<float>)p1, (wykobi::point2d<float>)p2, (wykobi::point2d<float>)q1,
	        (wykobi::point2d<float>)q2)) {
		return true;
	}

	return false;
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

		if (isIntersecting(l1, l2, p, inf)) {
			++count;
		}
	}

	return count % 2 == 1;
}

// TODO: optimise
static bool isIntersecting(const decltype(Shell::edges)& edges, const DynArray<Point>& points, const Vec2& p,
    const Vec2& n, biss::index_t& edgeIdx) {

	Vec2 inf = p + (n * 1000);

	for (size_t i = 0; i != edges.size(); ++i) {
		const auto& e = edges[i];
		const Vec2& l1 = points[e.i].p2;
		const Vec2& l2 = points[e.j].p2;

		if (isIntersecting(l1, l2, p, inf)) {
			edgeIdx = i;
			return true;
		}
	}

	return false;
}

GroupId World::create(const GroupDef& def) {
	const auto id = _groups.emplace({}, Group{});
	_groups[id].id.id = id;

	return GroupId{id};
}

GroupProxy World::get(GroupId id) {
	return GroupProxy(_groups, id.id);
}

void World::update(float step, int iterations) {
	if (std::isnan(_lastStep)) {
		_lastStep = step / iterations;
	}

	for (int rr = 0; rr < iterations; ++rr) {
		for (auto& g : _groups) {
			updatePosition(g, step / iterations);
			updateConstrain(g, step / iterations);
			updateAABB(g);
			updateShells(g);
		}
	}

	for (auto& g : _groups) {
		updateAABB(g);
		const auto oldCenter = g.center;
		updateCenter(g);

		_groups.update(g.id.id, g.aabb, g.center - oldCenter);
	}

	_lastStep = step / iterations;
}

void World::updateAABB(Group& g) {
	// TODO: optimize
	if (g.points.empty()) {
		return;
	}

	Vec2 lb{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
	    ub{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
	bool initAABB = false;
	for (auto& p : g.points) {
		if (p.flags & PointFlags::DISABLE) {
			continue;
		}
		initAABB = true;
		if (p.p2.x < lb.x) {
			lb.x = p.p2.x;
		}
		if (p.p2.y < lb.y) {
			lb.y = p.p2.y;
		}
		if (p.p2.x > ub.x) {
			ub.x = p.p2.x;
		}
		if (p.p2.y > ub.y) {
			ub.y = p.p2.y;
		}
	}

	if (initAABB) {
		g.aabb.lb = lb;
		g.aabb.ub = ub;
	} else {
		g.aabb = {};
	}
}

inline float fast_sign(float f) {
	if (f > 0)
		return 1;
	return (f == 0) ? 0 : -1;
}

void World::updateShells(Group& group) {
	if (!group.interactBits) {
		return;
	}

	_groups.query(group.aabb, [this, &group](biss::index_t nodeIdx) {
		auto& other = _groups[nodeIdx];
		if (&other == &group || !other.interactBits || other.shell.edges.empty()) {
			return true;
		}

		for (auto& p : group.points) {
			if (!(other.interactBits & group.interactBits) || (p.flags & PointFlags::SHELL) ||
			    !(p.flags & PointFlags::INTERACTIVE) || !isIntersecting(other.aabb, p.p2) ||
			    !::isIntersecting(other.shell.edges, other.points, p.p2)) {
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
				p.p1 += (((p.p2 - p.p1) * l) * e.friction) * l;
			}
		}

		for (size_t j = 0; j != group.shell.edges.size(); ++j) {
			auto& p = group.points[group.shell.edges[j].j];
			auto& pr = group.points[group.shell.edges[j].i];
			auto& pl = group.points[group.shell.edges[(j + 1) % group.shell.edges.size()].j];

			if (!(other.interactBits & group.interactBits) || p.flags & PointFlags::STATIC ||
			    !(p.flags & PointFlags::SHELL) || !isIntersecting(other.aabb, p.p2)) {
				continue;
			}

			const auto lr = (pr.p2 - p.p2);
			const auto ll = (pl.p2 - p.p2);

			const auto pn = ((pl.p2 - p.p2).fastN() + (pr.p2 - p.p2).fastN()).fastN();

			biss::index_t edgeIdx;
			if (!::isIntersecting(other.shell.edges, other.points, p.p2, pn, edgeIdx)) {
				continue;
			}

			const auto& e = other.shell.edges[edgeIdx];
			const Vec2& l1 = other.points[e.i].p2;
			const Vec2& l2 = other.points[e.j].p2;

			auto& l1p = other.points[e.i];
			auto& l2p = other.points[e.j];

			if (other.points[e.i].is(PointFlags::STATIC) && other.points[e.j].is(PointFlags::STATIC) &&
			    p.is(PointFlags::STATIC)) {
				continue;
			}

			Vec2 l = (l2 - l1);
			const Vec2& n = Vec2(-l.y, l.x).fastN();
			const auto penetration = (l2 - p.p2) * n;

			// TODO: Change to more physics formula
			const auto dPenetration = penetration * n;

			l = l.fastN();
			const float a1 = l * (p.p2 - l1);
			const float a2 = l * (l2 - p.p2);

			const float staticM = 1000 * std::max(std::max(l1p.m, l2p.m), p.m);
			float l1m = l1p.is(PointFlags::STATIC) ? staticM : l1p.m;
			float l2m = l2p.is(PointFlags::STATIC) ? staticM : l2p.m;
			float pm = p.is(PointFlags::STATIC) ? staticM : p.m;

//			auto blockV = [step = _lastStep](Point& p) {
//				const auto l = p.p2 - p.p1;
//				const auto ml = l.length();
//				const float maxDp = 5.0f;
//				if (ml / step > maxDp) {
//					p.p1 = p.p2 - maxDp * step * (l / ml);
//				}
//			};

			const float devider = (l1m * l2m + (l1m + l2m) * pm);
			if (!l1p.is(PointFlags::STATIC)) {
				l1p.p2 -= a2 * dPenetration * pm * l2m / devider;
			}
			if (!l2p.is(PointFlags::STATIC)) {
				l2p.p2 -= a1 * dPenetration * l2m * pm / devider;
			}
			if (!p.is(PointFlags::STATIC)) {
				p.p2 += dPenetration * l1m * l2m / devider;
				p.p1 += (((p.p2 - p.p1) * l) * e.friction) * l;
			}
		}
		return true;
	});
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

void World::updateConstrain(Group& group, float step) {
	for (auto& c : group.constrains) {
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

		if (c.is(ConstrainFlags::WORKS_IF_LOWER) && invD * c.distance < 1.0f) {
			continue;
		}

		if (c.is(ConstrainFlags::BREAK_IF_GREATER) && invD * c.breakDistance < 1.0f) {
			c.flags |= ConstrainFlags::DISABLE;
		}

		if (c.is(ConstrainFlags::BREAK_IF_LOWER) && invD * c.breakDistance < 1.0f) {
			c.flags |= ConstrainFlags::DISABLE;
		}

		float coeff = 0.5f * (c.distance * invD - 1.0f);
		if (c.is(ConstrainFlags::USE_HOOK_COEFF)) {
			float coeffWithHook = 0.5f * (c.hookCoeff * step * step * (c.distance * invD - 1.0f));
			if (std::fabs(coeffWithHook) < std::fabs(coeff)) {
				coeff = coeffWithHook;
			}
		}

		const auto dT = t * coeff;

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

void World::updateCenter(Group& group) {
	Vec2 center{0, 0};
	for (const auto& p : group.points) {
		center += p.p2;
	}
	group.center = center / group.points.size();
}

World::World(): _groups(0.1f, 4) {
}
