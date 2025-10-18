#pragma once
#include "figure.h"
#include <SFML/graphics.hpp>
#include <iostream>
//#include "bfs_algorithm.h"
#include "AstarSearch.h"

extern const sf::Vector2f TILE_SIZE;

class Seeker : public Figure {
private:
	sf::Sprite seekerSprite;
	std::vector <std::pair<int, int>> seekerSight;
	int cntstep = 0;
	const int UNIT = 5;
public:

	Seeker() {
		seekerSprite.setPosition(0, 0);
	}
	
	Seeker(float x, float y) {
		seekerSprite.setPosition(y * TILE_SIZE.x, x * TILE_SIZE.y);
	}
	void loadTexture(sf::Texture& load) {
		seekerSprite.setTexture(load);
	}
	void update(sf::Sprite& other) {
		this->seekerSprite = other;
	}
	
	void setPosition(float x, float y)  {
		seekerSprite.setPosition(y * TILE_SIZE.x, x * TILE_SIZE.y);
	}
	
	void move(float x, float y)  {
		seekerSprite.move(y, x);
	}

	sf::Vector2f getPosition()  {
		sf::Vector2f pos = seekerSprite.getPosition();
		return sf::Vector2f(pos.y / TILE_SIZE.x, pos.x / TILE_SIZE.y);
	}

	sf::Vector2f getAnnouncePosition() {
		return getPosition();
	}

	void update_sight(std::vector<std::pair<int, int>>& seekerSight) {
		this->seekerSight = seekerSight;
	}
	std::vector<std::pair<int, int>> getSight() {
		return seekerSight;
	}

	void isStepping() {
		cntstep++;
	}
	bool checkstep_announce() {
		return cntstep % UNIT == 0;
	}

	void draw(sf::RenderWindow& window)  {
		for (auto i : seekerSight) {
			sf::RectangleShape rectangle(sf::Vector2f(TILE_SIZE.x, TILE_SIZE.y));
			rectangle.setPosition(i.second * TILE_SIZE.x, i.first * TILE_SIZE.y);
			rectangle.setFillColor(sf::Color(128, 128, 128, 100));
			window.draw(rectangle);
		}
		window.draw(seekerSprite);
	}
};
