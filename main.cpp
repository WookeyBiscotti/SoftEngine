#include "src/world.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace soften;

auto makeRect(soften::World& world, const soften::Rect2& rect, FlagsStorage flags) {
	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);
	body.createPoint(Vec2{rect.lb.x, rect.lb.y}, flags);
	body.createPoint(Vec2{rect.ub.x, rect.lb.y}, flags);
	body.createPoint(Vec2{rect.ub.x, rect.ub.y}, flags);
	body.createPoint(Vec2{rect.lb.x, rect.ub.y}, flags);

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

auto makeCube(soften::World& world, const soften::Vec2& pos, float size) {
	auto bodyId = world.create(soften::GroupDef());
	auto body = world.get(bodyId);

	auto eps = [] { return 0.0f; };

	body.createPoint(pos + Vec2{size + eps(), size + eps()});
	body.createPoint(pos + Vec2{-size + eps(), size + eps()});
	body.createPoint(pos + Vec2{-size + eps(), -size + eps()});
	body.createPoint(pos + Vec2{size + eps(), -size + eps()});

	float Coeff = 1.0f * (1.0 / 60.0f) * (1.0 / 60.0f);
	Coeff = 1 / Coeff;

	//	auto id = body.createConstrain(0, 1);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_LOWER).distance(0.5f);
	//	id = body.createConstrain(1, 2);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_LOWER).distance(0.5f);
	//	id = body.createConstrain(2, 3);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_LOWER).distance(0.5f);
	//	id = body.createConstrain(3, 0);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_LOWER).distance(0.5f);
	//	id = body.createConstrain(0, 2);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_LOWER).distance(1.0f);
	//	id = body.createConstrain(1, 3);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_LOWER).distance(1.0f);
	//
	//	body.createConstrain(0, 1);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_GREATER).distance(1.5f);
	//	id = body.createConstrain(1, 2);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_GREATER).distance(1.5f);
	//	id = body.createConstrain(2, 3);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_GREATER).distance(1.5f);
	//	id = body.createConstrain(3, 0);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_GREATER).distance(1.5f);
	//	id = body.createConstrain(0, 2);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_GREATER).distance(2.0f);
	//	id = body.createConstrain(1, 3);
	//	body.c(id).flags(ConstrainFlags::WORKS_IF_GREATER).distance(2.0f);

	auto id = body.createConstrain(0, 1);
	//	body.c(id).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(Coeff);
	id = body.createConstrain(1, 2);
	//	body.c(id).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(Coeff);
	id = body.createConstrain(2, 3);
	//	body.c(id).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(Coeff);
	id = body.createConstrain(3, 0);
	//	body.c(id).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(Coeff);
	id = body.createConstrain(0, 2);
	//	body.c(id).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(Coeff);
	id = body.createConstrain(1, 3);
	//	body.c(id).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(Coeff);

	ShellDef shell;
	shell.edges.push_back({0, 1, 0.3f});
	shell.edges.push_back({1, 2, 0.3f});
	shell.edges.push_back({2, 3, 0.3f});
	shell.edges.push_back({3, 0, 0.3f});

	body.addShall(shell);

	return body;
}

void makeBorders(soften::World& world) {
	makeRect(world, Rect2{Vec2(-2, -2), Vec2(22, 0)}, PointFlags::STATIC);
	makeRect(world, Rect2{Vec2(-2, 10), Vec2(22, 12)}, PointFlags::STATIC);

	makeRect(world, Rect2{Vec2(-2, -2), Vec2(0, 12)}, PointFlags::STATIC);
	makeRect(world, Rect2{Vec2(20, -2), Vec2(22, 12)}, PointFlags::STATIC);
}

auto makePlate(soften::World& world, soften::Vec2 pos) {
	auto body = world.get(world.create(soften::GroupDef()));

	constexpr auto SIZEX = 60;
	constexpr auto SIZEY = 20;
	constexpr auto STEP = 0.2f;

	std::array<std::array<int, SIZEY>, SIZEX> ids;

	for (int x = 0; x != SIZEX; ++x) {
		for (int y = 0; y != SIZEY; ++y) {
			ids[x][y] = body.createPoint(pos + Vec2{pos.x + STEP * x, pos.x + STEP * y},
			    (x == 0 || x == SIZEX - 1 || y == 0 || y == SIZEY - 1 ? PointFlags::INTERACTIVE : 0), 0.01f);
		}
	}

	for (int x = 0; x != SIZEX; ++x) {
		for (int y = 0; y != SIZEY; ++y) {
			if (x != SIZEX - 1) {
				//				body.createConstrain(ids[x][y], ids[x + 1][y]);
				auto id = body.createConstrain(
				    ids[x][y], ids[x + 1][y], ConstrainFlags::WORKS_IF_GREATER | ConstrainFlags::BREAK_IF_GREATER);
				auto c = body.c(id);
				c.breakDistance(c.distance() * 2);
			}
			if (y != SIZEY - 1) {
				//				body.createConstrain(ids[x][y], ids[x][y + 1]);
				auto id = body.createConstrain(
				    ids[x][y], ids[x][y + 1], ConstrainFlags::WORKS_IF_GREATER | ConstrainFlags::BREAK_IF_GREATER);
				auto c = body.c(id);
				c.breakDistance(c.distance() * 2);
			}
		}
	}

	for (int x = 0; x != SIZEX; ++x) {
		body.p(ids[x][0]).flags(PointFlags::STATIC);
	}

	//	body.interactBits(0);

	return body;
}

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 400), "Time Alchemy");
	auto view = window.getView();
	view.setSize(20, 10);
	view.setCenter(10, 5);
	window.setView(view);

	soften::World world;
	world.gravity({0, 10});

	auto body = makeCube(world, {10, 5}, 0.15);

	auto mouseId = body.createPoint(PointDef(Vec2(0, 0), PointFlags::STATIC | PointFlags::DISABLE));
	auto mouseCId = body.createConstrain(ConstrainDef(mouseId, mouseId));
	body.c(mouseCId).flags(ConstrainFlags::DISABLE);

	sf::CircleShape circle;
	circle.setFillColor(sf::Color::White);
	circle.setRadius(0.05);
	circle.setOrigin(0.1, 0.1);

	sf::RectangleShape rect;
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(0.05);
	rect.setFillColor({0, 0, 0, 0});

	makeBorders(world);

	int iterations = 100;

	bool running = true;
	auto clock = sf::Clock();
	while (running) {
		auto t1 = clock.getElapsedTime();
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
				auto minR = body.p(Id(0)).position().distance(mouse);
				for (int i = 1; i < body.pointsCount(); ++i) {
					auto r = body.p(Id(i)).position().distance(mouse);
					if (r < minR) {
						minR = r;
						minIdx = i;
					}
				}
				body.c(mouseCId).flags(ConstrainFlags::USE_HOOK_COEFF).hookCoeff(100).distance(0);
				body.c(mouseCId).i(mouseId);
				body.c(mouseCId).j(Id(minIdx));
				body.p(mouseId).position(mouse);
				body.p(mouseId).flags(PointFlags::STATIC);
				//				body.fitConstrain(mouseCId);
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
				static int i = 0;
				makeCube(world, mouse, 0.15);
				std::cout << ++i << std::endl;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			soften::Vec2 mouse = {rawPos.x, rawPos.y};
			static int i = 0;
			makeCube(world, mouse, 0.15);
			std::cout << ++i << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			auto rawPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			soften::Vec2 mouse = {rawPos.x, rawPos.y};
			world.query(Rect2{mouse, mouse}, [&world](biss::index_t idx) {
				if (!world.get(Id{idx}).isAll(PointFlags::STATIC)) {
					world.remove(Id{idx});
					return false;
				}
				return true;
			});
		}

		world.update(1.0 / 60, 2);

		window.clear();

		for (auto body : world) {
			//						for (int i = 0; i != body.pointsCount(); ++i) {
			//							circle.setPosition(body.p(Id(i)).position().x,
			// body.p(Id(i)).position().y); 							window.draw(circle);
			//						}
			for (int i = 0; i != body.constrainCount(); ++i) {
				auto c = body.c(i);
				if (c.flags() & ConstrainFlags::DISABLE) {
					continue;
				}
				sf::Vertex line[] = {sf::Vertex(sf::Vector2f(body.p(c.i()).position().x, body.p(c.i()).position().y)),
				    sf::Vertex(sf::Vector2f(body.p(c.j()).position().x, body.p(c.j()).position().y))};
				window.draw(line, 2, sf::Lines);
			}
			//			{
			//
			//				auto rect2 = body.aabb();
			//				rect.setPosition(body.aabb().lb.x, body.aabb().lb.y);
			//				auto size = body.aabb().ub- body.aabb().lb;
			//				rect.setSize({size.x, size.y});
			//				window.draw(rect);
			//			}
			//
			//			{
			//				circle.setPosition(body.center().x, body.center().y);
			//				window.draw(circle);
			//			}
		}

		auto t2 = clock.restart();

		const auto dt = (t2 - t1).asMicroseconds();
		const auto umPerFrame = 1'000'000 / 60.0f;

		if (dt < umPerFrame) {
			sf::sleep(sf::microseconds(umPerFrame - dt));
			iterations = std::min(iterations + 1, 20);
		} else {
			iterations = std::max(iterations - 1, 10);
		}

		//		sf::Text text;
		//		text.setScale(0.01f, 0.01f);
		//		text.setString(std::to_string(1'000'000.0f / dt));
		//		text.setPosition(window.getView().getCenter() - 0.5f * window.getView().getSize());
		//
		//		window.draw(text);
		window.display();
	}
	return 0;
}
