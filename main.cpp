#include <iostream>

#include "world.hpp"

#include <SFML/Graphics.hpp>


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 400), "Time Alchemy");
    auto view = window.getView();
    view.setSize(20, 10);
    view.setCenter(10, 5);
    window.setView(view);


    verlet::World world;
    world.gravity({0, 10});

    int SIZE = 5;

    for (int a = 0; a < SIZE; ++a) {
        world.addPoint(5 + std::cos(2 * M_PI * a / SIZE), 5 + std::sin(2 * M_PI * a / SIZE));
    }

    world.addPoint(5, 5);

    for (int a = 0; a < SIZE; ++a) {
        world.addConstrain(a, (a + 1) % SIZE);
        world.addConstrain(a, SIZE);
    }

    auto mouseId = world.addPoint(0, 0, verlet::Point::STATIC);
    auto mouseCId = world.addConstrain(mouseId, mouseId);
    world.c(mouseCId).flags = verlet::Constrain::DISABLE;

    world.addBorder({0, 0}, {20, 0});
    world.addBorder({20, 0}, {20, 10});
    world.addBorder({20, 10}, {0, 10});
    world.addBorder({0, 10}, {0, 0});

    bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                running = false;
                break;
            } else if (event.type == sf::Event::EventType::MouseButtonPressed) {
                if (world.points().empty()) {
                    continue;
                }

                auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                verlet::Vector2 mouse = {rawPos.x, rawPos.y};
                auto minIdx = 0;
                auto minR = world.p(0).p2.distance(mouse);
                for (int i = 1; i < SIZE + 1; ++i) {
                    auto r = world.p(i).p2.distance(mouse);
                    if (r < minR) {
                        minR = r;
                        minIdx = i;
                    }
                }

                world.c(mouseCId).flags = verlet::Constrain::NONE;
                world.c(mouseCId).i = mouseId;
                world.c(mouseCId).j = minIdx;
                world.p(mouseId).p2 = mouse;
                world.resetConstrain(mouseCId);
            } else if (event.type == sf::Event::EventType::MouseButtonReleased) {
                world.c(mouseCId).flags = verlet::Constrain::DISABLE;
                world.p(mouseId).p2 = {0, 0};
            } else if (event.type == sf::Event::EventType::MouseMoved) {
                if (!world.c(mouseCId).is(verlet::Constrain::DISABLE)) {
                    auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    verlet::Vector2 mouse = {rawPos.x, rawPos.y};
                    world.p(mouseId).p2 = mouse;
                }
            }
        }

        world.update(1.0 / 600);

        window.clear();

        sf::CircleShape circle;
        circle.setFillColor(sf::Color::White);
        circle.setRadius(0.1);
        circle.setOrigin(0.1, 0.1);

        for (const auto &p : world.points()) {
            circle.setPosition(p.p2.x, p.p2.y);
            window.draw(circle);
        }

        for (const auto &c : world.constrains()) {
            if (c.is(verlet::Constrain::DISABLE)) {
                continue;
            }
            sf::Vertex line[] =
                    {
                            sf::Vertex(sf::Vector2f(world.points()[c.i].p2.x, world.points()[c.i].p2.y)),
                            sf::Vertex(sf::Vector2f(world.points()[c.j].p2.x, world.points()[c.j].p2.y))};
            window.draw(line, 2, sf::Lines);
        }

        sf::sleep(sf::milliseconds(1'00 / 60));

        window.display();
    }
    return 0;
}
