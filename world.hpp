#pragma once

#include <vector>

#include "constrain.hpp"
#include "point.hpp"

namespace verlet {

    class World {
    public:
        void gravity(const Vector2 &gravity) {
            _gravity = gravity;
        }

        const Vector2 &gravity() const {
            return _gravity;
        }

        void update(float step);

        const std::vector<Point> &points() const {
            return _points;
        }

        std::size_t addPoint(const Point &point) {
            _points.push_back(point);
            return _points.size() - 1;
        }

        std::size_t addPoint(float x, float y) {
            _points.emplace_back(Vector2(x, y));
            return _points.size() - 1;
        }

        void addBorder(const Vector2 &a, const Vector2 &b) {
            _borders.emplace_back(a, b);
        }

    private:
        float _lastStep = std::numeric_limits<float>::signaling_NaN();
        Vector2 _gravity{0, 0};

        std::vector<Point> _points;

        std::vector<std::pair<Vector2, Vector2>> _borders;
        std::vector<Constrain> _constrains;
    };

}// namespace verlet
