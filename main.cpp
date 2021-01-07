#include "group/group.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace soften;

auto makeCube(soften::World& world, soften::Vec2 pos) {
	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);

	auto eps = [] { return rand() % 10'000 / 10'000'0.0f; };

	body.createPoint(pos + Vec2{0.5f + eps(), 0.5f + eps()});
	body.createPoint(pos + Vec2{-0.5f + eps(), 0.5f + eps()});
	body.createPoint(pos + Vec2{-0.5f + eps(), -0.5f + eps()});
	body.createPoint(pos + Vec2{0.5f + eps(), -0.5f + eps()});

	body.createConstrain(0, 1);
	body.createConstrain(1, 2);
	body.createConstrain(2, 3);
	body.createConstrain(3, 0);
	body.createConstrain(0, 2);
	body.createConstrain(1, 3);

	ShellDef shell;
	shell.edges.push_back({0, 1});
	shell.edges.push_back({1, 2});
	shell.edges.push_back({2, 3});
	shell.edges.push_back({3, 0});

	body.addShall(shell);

	return body;
}

auto makeRect(soften::World& world, soften::Rect2 rect, FlagsStorage flags) {
	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);
	body.createPoint(Vec2{rect.position.x, rect.position.y}, flags);
	body.createPoint(Vec2{rect.position.x + rect.size.x, rect.position.y}, flags);
	body.createPoint(Vec2{rect.position.x + rect.size.x, rect.position.y + rect.size.y}, flags);
	body.createPoint(Vec2{rect.position.x, rect.position.y + rect.size.y}, flags);

	body.createConstrain(0, 1);
	body.createConstrain(1, 2);
	body.createConstrain(2, 3);
	body.createConstrain(3, 0);
	body.createConstrain(0, 2);
	body.createConstrain(1, 3);

	ShellDef shell;
	shell.edges.push_back({0, 1});
	shell.edges.push_back({1, 2});
	shell.edges.push_back({2, 3});
	shell.edges.push_back({3, 0});

	body.addShall(shell);

	return body;
}

void makeBorders(soften::World& world) {
	makeRect(world, Rect2{Vec2(-2, -2), Vec2(24, 2)}, PointFlags::STATIC);
	makeRect(world, Rect2{Vec2(-2, 12), Vec2(24, 2)}, PointFlags::STATIC);

	makeRect(world, Rect2{Vec2(-2, -2), Vec2(2, 14)}, PointFlags::STATIC);
	makeRect(world, Rect2{Vec2(20, -2), Vec2(2, 14)}, PointFlags::STATIC);
}

auto makeCube2(soften::World& world, soften::Vec2 pos) {
	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);
	body.createPoint(pos + Vec2{0.5f, -0.5f});
	body.createPoint(pos + Vec2{-0.5f, -0.5f});
	body.createPoint(pos + Vec2{-0.5f, 0.5f});
	body.createPoint(pos + Vec2{0.5f, 0.5f});

	body.createConstrain(0, 1);
	body.createConstrain(1, 2);
	body.createConstrain(2, 3);
	body.createConstrain(3, 0);
	body.createConstrain(0, 2);
	body.createConstrain(1, 3);

	ShellDef shell;
	shell.edges.push_back({0, 1});
	shell.edges.push_back({1, 2});
	shell.edges.push_back({2, 3});
	shell.edges.push_back({3, 0});

	body.addShall(shell);

	return body;
}

auto makePlate(soften::World& world, soften::Vec2 pos) {
	auto body = world.get(world.create(soften::GroupDef()));

	constexpr auto SIZEX = 60;
	constexpr auto SIZEY = 20;
	constexpr auto STEP = 0.2f;

	std::array<std::array<int, SIZEY>, SIZEX> ids;

	for (int x = 0; x != SIZEX; ++x) {
		for (int y = 0; y != SIZEY; ++y) {
			ids[x][y] = body.createPoint(pos + Vec2{pos.x + STEP * x, pos.x + STEP * y}).idx;
		}
	}

	for (int x = 0; x != SIZEX; ++x) {
		for (int y = 0; y != SIZEY; ++y) {
			if (x != SIZEX - 1) {
				body.createConstrain(ids[x][y], ids[x + 1][y], ConstrainFlags::WORKS_IF_GREATER);
			}
			if (y != SIZEY - 1) {
				body.createConstrain(ids[x][y], ids[x][y + 1], ConstrainFlags::WORKS_IF_GREATER);
			}
		}
	}

	for (int x = 0; x != SIZEX; ++x) {
		body.p(ids[x][0]).flags(PointFlags::STATIC);
	}

	return body;
}

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 400), "Time Alchemy");
	auto view = window.getView();
	view.setSize(20, -10);
	view.setCenter(10, 5);
	window.setView(view);

	soften::World world;
	world.gravity({0, -10});

	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);

	//	int SIZE = 5;
	//	for (int a = 0; a < SIZE; ++a) {
	//		body.createPoint(PointDef(Vec2(5 + std::cos(2 * M_PI * a / SIZE), 5 + std::sin(2 * M_PI * a / SIZE))));
	//	}
	//	body.createPoint(PointDef(Vec2(5, 5)));
	//	for (int a = 0; a < SIZE; ++a) {
	//		body.createConstrain(ConstrainDef(PointIdx(a), PointIdx((a + 1) % SIZE)));
	//		body.createConstrain(ConstrainDef(PointIdx(a), PointIdx(SIZE)));
	//	}

	//	body = makePlate(world, {1, 1});
	body = makeCube(world, {1, 1});

	//	{
	//		auto bodyId1 = world.create(soften::GroupDef());
	//		auto body1 = world.get(bodyId1);
	//
	//		int SIZE = 10;
	//		for (int a = 0; a < SIZE; ++a) {
	//			body1.createPoint(PointDef(Vec2(5 + std::cos(2 * M_PI * a / SIZE), 5 + std::sin(2 * M_PI * a / SIZE))));
	//		}
	//		body1.createPoint(PointDef(Vec2(5, 5)));
	//		for (int a = 0; a < SIZE; ++a) {
	//			body1.createConstrain(ConstrainDef(PointIdx(a), PointIdx((a + 1) % SIZE)));
	//			body1.createConstrain(ConstrainDef(PointIdx(a), PointIdx(SIZE)));
	//		}
	//	}

	auto mouseId = body.createPoint(PointDef(Vec2(0, 0), PointFlags::STATIC | PointFlags::DISABLE));
	auto mouseCId = body.createConstrain(ConstrainDef(mouseId, mouseId));
	body.c(mouseCId).flags(ConstrainFlags::DISABLE);

	//	makeCube(world, {3, 3});
	//	makeCube(world, {5, 3});
	//	makeCube(world, {4, 1});
	//	makeCube(world, {4.3, 2.9});
	//	makeCube(world, {7, 2});

	//	world.addBorder({0, 0}, {20, 0});
	//	world.addBorder({20, 0}, {20, 10});
	//	world.addBorder({20, 10}, {0, 10});
	//	world.addBorder({0, 10}, {0, 0});

	makeBorders(world);

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
				for (int i = 1; i < body.pointsCount(); ++i) {
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
				body.p(mouseId).flags(PointFlags::STATIC);
				body.fitConstrain(mouseCId);
			} else if (event.type == sf::Event::EventType::MouseButtonReleased) {
				body.c(mouseCId).flags(ConstrainFlags::DISABLE);
				body.p(mouseId).flags(PointFlags::STATIC | PointFlags::DISABLE);
				body.p(mouseId).position({0, 0});
			} else if (event.type == sf::Event::EventType::MouseMoved) {
				if (!(body.c(mouseCId).flags() & soften::ConstrainFlags::DISABLE)) {
					auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					soften::Vec2 mouse = {rawPos.x, rawPos.y};
					body.p(mouseId).position(mouse);
				}
			} else if (event.type == sf::Event::EventType::KeyReleased) {
				auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				soften::Vec2 mouse = {rawPos.x, rawPos.y};
				makeCube(world, mouse);
			}
		}

		world.update(1.0 / 60);

		window.clear();

		sf::CircleShape circle;
		circle.setFillColor(sf::Color::White);
		circle.setRadius(0.1);
		circle.setOrigin(0.1, 0.1);

		sf::RectangleShape rect;
		rect.setOutlineColor(sf::Color::Green);
		rect.setOutlineThickness(0.05);
		rect.setFillColor({0, 0, 0, 0});
		for (auto body : world) {
			//			for (int i = 0; i != body.pointsCount(); ++i) {
			//				circle.setPosition(body.p(PointIdx(i)).position().x, body.p(PointIdx(i)).position().y);
			//				window.draw(circle);
			//			}

			for (int i = 0; i != body.constrainCount(); ++i) {
				auto c = body.c(i);
				if (c.flags() & ConstrainFlags::DISABLE) {
					continue;
				}
				sf::Vertex line[] = {sf::Vertex(sf::Vector2f(body.p(c.i()).position().x, body.p(c.i()).position().y)),
				    sf::Vertex(sf::Vector2f(body.p(c.j()).position().x, body.p(c.j()).position().y))};
				window.draw(line, 2, sf::Lines);
			}

			//			rect.setPosition(body.aabb().position.x, body.aabb().position.y);
			//			rect.setSize({body.aabb().size.x, body.aabb().size.y});
			//
			//			window.draw(rect);
		}

		sf::sleep(sf::milliseconds(1'000 / 60));

		window.display();
	}
	return 0;
}
