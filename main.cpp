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

    for (size_t i = 0; i != 10; ++i) {
        world.addPoint(rand() % 20, rand() % 10);
    }

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
            }
        }

        world.update(1.0 / 60);

        window.clear();

        sf::CircleShape circle;
        circle.setFillColor(sf::Color::White);
        circle.setRadius(0.1);
        circle.setOrigin(0.05, 0.05);

        for (const auto &p : world.points()) {
            circle.setPosition(p.p2.x, p.p2.y);
            window.draw(circle);
        }

        sf::sleep(sf::milliseconds(1'000 / 60));

        window.display();
    }
    return 0;
}
