#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "Menu\MenuEntry.h"

const sf::Vector2f WINDOW_SIZE(1920, 1080);
const sf::Vector2f TILE_SIZE(60.0f, 60.0f);
const int INF = 1e9;
// =======================

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
#define rand(l,r) std::uniform_int_distribution <int> (l,r) (rng)


int main() {

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Game", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, 0));
	Menu menu_game;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		menu_game.run(window);
		// ====================== CHECK and EXIT =============================
		if (menu_game.checkExit() && menu_game.checkChoosedEnter()) {
			window.close();
		}
	}
}
