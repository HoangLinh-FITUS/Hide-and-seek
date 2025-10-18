#pragma once
#include "figure.h"
#include <SFML/graphics.hpp>
#include "sightrange.h"
#include "minimax_algorithm.h"

extern const sf::Vector2f TILE_SIZE;
extern const int INF;
extern std::mt19937 rng;

class Hider : public Figure {
private:
	sf::Sprite hiderSprite;
	std::vector <std::pair<int, int>> hiderSight;
	sf::Sprite announce;
	int cntstep = 0;
	const int UNIT = 5;
public:

	Hider() {
		hiderSprite.setPosition(0, 0);
	}

	Hider(float x, float y) {
		hiderSprite.setPosition(y * TILE_SIZE.x, x * TILE_SIZE.y);
	}

	void update(sf::Sprite& other) {
		this->hiderSprite = other;
	}
	void loadTexture(sf::Texture& load) {
		hiderSprite.setTexture(load);
	}
	void setPosition(float x, float y) {
		hiderSprite.setPosition(y * TILE_SIZE.x, x * TILE_SIZE.y);
	}

	void move(float x, float y) {
		hiderSprite.move(y, x);
	}

	sf::Vector2f getPosition() {
		sf::Vector2f pos = hiderSprite.getPosition();
		return sf::Vector2f(pos.y / TILE_SIZE.x, pos.x / TILE_SIZE.y);
	}
	void updateAnnounce(sf::Sprite& other) {
		this->announce = other;
	}

	void setAnnouncePosition(float x, float y) {
		announce.setPosition(y * TILE_SIZE.x, x * TILE_SIZE.y);
	}

	sf::Vector2f getAnnouncePosition() {
		sf::Vector2f pos = announce.getPosition();
		return sf::Vector2f(pos.y / TILE_SIZE.x, pos.x / TILE_SIZE.y);
	}

	void update_sight(std::vector<std::pair<int, int>>& hiderSight) {
		this->hiderSight = hiderSight;
	}
	std::vector<std::pair<int, int>> getSight() {
		return hiderSight;
	}

	void isStepping() {
		cntstep++;
	}
	bool checkstep_announce() {
		return cntstep % UNIT == 0;
	}

	void draw(sf::RenderWindow& window) {
		for (auto i : hiderSight) {
			sf::RectangleShape rectangle(sf::Vector2f(TILE_SIZE.x, TILE_SIZE.y));
			rectangle.setPosition(i.second * TILE_SIZE.x, i.first * TILE_SIZE.y);
			rectangle.setFillColor(sf::Color(156, 255, 172, 100));
			window.draw(rectangle);
		}
		if (announce.getTexture()!=nullptr) { 
			window.draw(announce); 
		}
		window.draw(hiderSprite);
	}
};

