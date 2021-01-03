#include "group/group.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
	using namespace soften;

	sf::RenderWindow window(sf::VideoMode(800, 400), "Time Alchemy");
	auto view = window.getView();
	view.setSize(20, 10);
	view.setCenter(10, 5);
	window.setView(view);

	soften::World world;
	world.gravity({0, 10});

	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);

	int SIZE = 5;
	for (int a = 0; a < SIZE; ++a) {
		body.createPoint(PointDef(Vec2(5 + std::cos(2 * M_PI * a / SIZE), 5 + std::sin(2 * M_PI * a / SIZE))));
	}
	body.createPoint(PointDef(Vec2(5, 5)));
	for (int a = 0; a < SIZE; ++a) {
		body.createConstrain(ConstrainDef(PointIdx(a), PointIdx((a + 1) % SIZE)));
		body.createConstrain(ConstrainDef(PointIdx(a), PointIdx(SIZE)));
	}

	{
		auto bodyId1 = world.create(soften::GroupDef());
		auto body1 = world.get(bodyId1);

		int SIZE = 10;
		for (int a = 0; a < SIZE; ++a) {
			body1.createPoint(PointDef(Vec2(5 + std::cos(2 * M_PI * a / SIZE), 5 + std::sin(2 * M_PI * a / SIZE))));
		}
		body1.createPoint(PointDef(Vec2(5, 5)));
		for (int a = 0; a < SIZE; ++a) {
			body1.createConstrain(ConstrainDef(PointIdx(a), PointIdx((a + 1) % SIZE)));
			body1.createConstrain(ConstrainDef(PointIdx(a), PointIdx(SIZE)));
		}
	}

	auto mouseId = body.createPoint(PointDef(Vec2(0, 0), soften::PointFlags::STATIC));
	auto mouseCId = body.createConstrain(ConstrainDef(mouseId, mouseId));
	body.c(mouseCId).flags(ConstrainFlags::DISABLE);

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
				if (!body.pointsCount()) {
					continue;
				}

				auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				Vec2 mouse = {rawPos.x, rawPos.y};
				auto minIdx = 0;
				auto minR = body.p(PointIdx(0)).position().distance(mouse);
				for (int i = 1; i < SIZE + 1; ++i) {
					auto r = body.p(PointIdx(i)).position().distance(mouse);
					if (r < minR) {
						minR = r;
						minIdx = i;
					}
				}

				body.c(mouseCId).flags(ConstrainFlags::NONE);
				body.c(mouseCId).i(mouseId);
				body.c(mouseCId).j(PointIdx(minIdx));
				body.p(mouseId).position(mouse);
				body.fitConstrain(mouseCId);
			} else if (event.type == sf::Event::EventType::MouseButtonReleased) {
				body.c(mouseCId).flags(ConstrainFlags::DISABLE);
				body.p(mouseId).position({0, 0});
			} else if (event.type == sf::Event::EventType::MouseMoved) {
				if (!(body.c(mouseCId).flags() & soften::ConstrainFlags::DISABLE)) {
					auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					soften::Vec2 mouse = {rawPos.x, rawPos.y};
					body.p(mouseId).position(mouse);
				}
			}
		}

		world.update(1.0 / 60);

		window.clear();

		sf::CircleShape circle;
		circle.setFillColor(sf::Color::White);
		circle.setRadius(0.1);
		circle.setOrigin(0.1, 0.1);

		for (auto body : world) {

			for (int i = 0; i != body.pointsCount(); ++i) {
				circle.setPosition(body.p(PointIdx(i)).position().x, body.p(PointIdx(i)).position().y);
				window.draw(circle);
			}

			for (int i = 0; i != body.constrainCount(); ++i) {
				auto c = body.c(i);
				if (c.flags() & ConstrainFlags::DISABLE) {
					continue;
				}
				sf::Vertex line[] = {sf::Vertex(sf::Vector2f(body.p(c.i()).position().x, body.p(c.i()).position().y)),
				    sf::Vertex(sf::Vector2f(body.p(c.j()).position().x, body.p(c.j()).position().y))};
				window.draw(line, 2, sf::Lines);
			}
		}
		sf::sleep(sf::milliseconds(1'000 / 60));

		window.display();
	}
	return 0;
}
