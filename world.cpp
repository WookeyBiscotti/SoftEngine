#include "world.hpp"

#include <cmath>
#include <wykobi.hpp>

using namespace verlet;

//TODO: optimise
static bool isIntersecting(const Vector2 &p1, const Vector2 &p2, const Vector2 &q1, const Vector2 &q2, Vector2 &inter) {
    wykobi::point2d<float> p;
    if (wykobi::intersect((wykobi::point2d<float>) p1, (wykobi::point2d<float>) p2, (wykobi::point2d<float>) q1, (wykobi::point2d<float>) q1, p)) {
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
        auto p2 = p.p2 + (p.p2 - p.p1) * stepRatio + (_gravity * step * step);
        p.p1 = p.p2;
        p.p2 = p2;

        for (const auto &[a, b] : _borders) {
            Vector2 inter;
            if (isIntersecting(a, b, p.p1, p.p2, inter)) {
                std::swap(p.p1, p.p2);
            }
        }

        for (const auto &c : _constrains) {
            const auto d = _points[c.i].p2.distance(_points[c.j].p2);

            
        }
    }


    _lastStep = step;
}
