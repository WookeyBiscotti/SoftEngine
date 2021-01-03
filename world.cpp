#include "world.hpp"

#include <cmath>
#include <wykobi.hpp>

using namespace verlet;

//TODO: optimise
static bool isIntersecting(const Vector2 &p1, const Vector2 &p2, const Vector2 &q1, const Vector2 &q2, Vector2 &inter) {
    wykobi::point2d<float> p;
    if (wykobi::intersect((wykobi::point2d<float>) p1, (wykobi::point2d<float>) p2, (wykobi::point2d<float>) q1, (wykobi::point2d<float>) q2, p)) {
        inter.x = p.x;
        inter.y = p.y;

        return true;
    }

    return false;
}

void World::update(float step) {
    if (std::isnan(_lastStep)) {
        _lastStep = step;
    }

    const auto stepRatio = (step / _lastStep);
    for (auto &p : _points) {
        if (p.is(Point::STATIC)) {
            continue;
        }
        auto p2 = p.p2 + (p.p2 - p.p1) * stepRatio + (_gravity * step * step);
        p.p1 = p.p2;
        p.p2 = p2;
    }

    for (int rr = 0; rr < 100; ++rr) {

        for (const auto &c : _constrains) {
            if (c.is(Constrain::DISABLE)) {
                continue;
            }

            auto &pa = _points[c.i];
            auto &pb = _points[c.j];

            const auto t = pa.p2 - pb.p2;
            auto invD = t.fastInvLength();
            const auto dT = 0.5f * t * (c.distance * invD - 1.0f);

            if (pa.is(Point::STATIC)) {
                if (pb.is(Point::STATIC)) {
                    continue;
                } else {
                    pb.p2 -= 2 * dT;
                }
            } else {
                if (pb.is(Point::STATIC)) {
                    pa.p2 += 2 * dT;
                } else {
                    pa.p2 += dT;
                    pb.p2 -= dT;
                }
            }
        }
    }

    for (auto &p : _points) {
        if (p.is(Point::STATIC)) {
            continue;
        }
        for (const auto &[a, b] : _borders) {
            Vector2 o;
            if (isIntersecting(a, b, p.p1, p.p2, o)) {
                const auto l = Vector2(a - b).fastN();
                auto p1o = o - p.p1;
                auto p2o = o - p.p2;
                p.p1 = o + (p1o - 2 * l * (p1o * l));
                p.p2 = o + (p2o - 2 * l * (p2o * l));
            }
        }
    }

    _lastStep = step;
}
