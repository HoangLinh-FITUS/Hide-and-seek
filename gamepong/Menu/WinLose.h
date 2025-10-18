#pragma once
#include "menulevel.h"

class WinLose : public Menu_Level {
public:
	WinLose() {
		Menu_Level::update_background("Assets/win.png");
		Menu_Level::update_listcontext(std::vector<std::string>{"Restart", "Back to menu"});
	}

	void draw(sf::RenderWindow& window) {
		window.draw(Menu_Level::getbackground());
		int pos = 0;
		for (auto& i : Menu_Level::get_listcontext()) {
			i.setPosition(870, 450 + pos);
			window.draw(i);
			pos += 70;
		}
		window.display();
	}

	bool run(sf::RenderWindow& window) {
		sf::View view = window.getDefaultView();
		window.setView(view);
		while (true) {
			window.clear();
			if (Menu_Level::checkChoosedEnter()) {
				if (Menu_Level::Choosed() == 1) {
					Menu_Level::setChoseEnter(false);
					return 0;
				}
				else if (Menu_Level::Choosed() == 2) {
					Menu_Level::setChoseEnter(false);
					return 1;
				}
			}
			else {
				Menu_Level::run(window);
				draw(window);
			}
		}
	}
};