#pragma once

#include <SFML/Graphics.hpp>

class Menu_Level {
private:
	sf::Text create_text(const std::string& CONTEXT, int SIZE, sf::Color COLOR,
		sf::Text::Style STYLE_TEXT, const std::string& PATH_FONT = "sixty/SIXTY.TTF") {
		static sf::Font font;
		font.loadFromFile(PATH_FONT);
		sf::Text text(CONTEXT, font);
		text.setCharacterSize(SIZE);
		text.setStyle(STYLE_TEXT);
		text.setFillColor(COLOR);
		text.setOutlineColor(sf::Color(252, 252, 252));
		text.setOutlineThickness(1);
		return text;
	}

public:
	void update_listcontext(std::vector <std::string> LIST_NAME_CONTEXT) {
		for (const auto& name : LIST_NAME_CONTEXT) {
			list_choose.push_back(create_text(name, SIZE_FONT, sf::Color(94, 23, 235), sf::Text::Bold));
		}
		list_choose[0].setFillColor(sf::Color(255, 87, 87));
		list_choose[0].setOutlineColor(sf::Color(252, 252, 252));
		list_choose[0].setOutlineThickness(2);
		// phan doan khong dinh
		for (int i = 0; i < list_choose.size(); i++) {
			list_choose[i].setPosition(0, i * (SIZE_FONT + 50));
		}
	}

	void moveDown() {
		int N = list_choose.size();
		list_choose[choose].setFillColor(sf::Color(94, 23, 235));
		list_choose[choose].setOutlineThickness(1);
		choose++;
		choose %= N;
		list_choose[choose].setFillColor(sf::Color(255, 87, 87));
		list_choose[choose].setOutlineColor(sf::Color(252, 252, 252));
		list_choose[choose].setOutlineThickness(2);
	}

	void moveUp() {
		int N = list_choose.size();
		list_choose[choose].setFillColor(sf::Color(94, 23, 235));
		list_choose[choose].setOutlineThickness(1);
		choose = (choose - 1 + N) % N;
		list_choose[choose].setFillColor(sf::Color(255, 87, 87));
		list_choose[choose].setOutlineColor(sf::Color(252, 252, 252));
		list_choose[choose].setOutlineThickness(2);
	}

	int Choosed() {
		return choose + 1;
	}

	void update_background(const std::string& PATH_BACKGROUND,sf::Vector2f pos={0.0,0.0}) {
		static sf::Texture tmp;
		tmp.loadFromFile(PATH_BACKGROUND);
		background.setPosition(pos);
		background.setTexture(tmp);
	}

	std::vector <sf::Text> get_listcontext() {
		return list_choose;
	}

	sf::Sprite getbackground() {
		return background;
	}
	void setChoseEnter(bool b) {
		choose_enter = b;
	}
	bool checkChoosedEnter() {
		return choose_enter;
	}

	bool checkExit() {
		return Choosed() == (int)list_choose.size();
	}

	void run(sf::RenderWindow& window) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			sf::sleep(sf::seconds(0.15));
			moveDown();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			sf::sleep(sf::seconds(0.15));
			moveUp();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			sf::sleep(sf::seconds(0.15));
			choose_enter = true;
		}

	}

private:
	sf::Sprite background;
	std::vector<sf::Text> list_choose;
	const float SIZE_FONT = 50;
	int choose = 0;
	bool choose_enter = false;
};