#pragma once
#include <SFML/graphics.hpp>
#include <queue>
#include <map>
#include <vector>

const int h[] = { 0 , 0, 1, -1, 1, 1, -1, -1 };
const int c[] = { 1, -1, 0, 0, -1, 1, 1, -1 };

class Figure {
public:
	virtual sf::Vector2f getPosition() = 0;
	virtual std::vector<std::pair<int, int>> getSight() = 0;
	virtual void update(sf::Sprite&) = 0;
	virtual void loadTexture(sf::Texture& load) = 0;
	virtual void setPosition(float x, float y) = 0;
	virtual void move(float x, float y) = 0;
	virtual void update_sight(std::vector<std::pair<int, int>>&) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void updateAnnounce(sf::Sprite& other){};
	virtual void setAnnouncePosition(float x, float y){};
	virtual sf::Vector2f getAnnouncePosition() { return sf::Vector2f(-1, -1); }
	virtual void isStepping() = 0;
	virtual bool checkstep_announce() = 0;
};
