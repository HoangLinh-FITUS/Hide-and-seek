#pragma once
#include "menulevel.h"
#include "menulevel1.h"
#include "menulevel2.h"
#include "menulevel3.h"
#include "menulevel4.h"


class Menu : public Menu_Level { // menu level 0
public:
	Menu() {
		Menu_Level::update_background("Assets/Menu.png");
		Menu_Level::update_listcontext(std::vector<std::string>{"Level 1", "Level 2", "Level 3", "Level 4", "Exit"});
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

	void run(sf::RenderWindow& window) {
		while (true) {
			window.clear();
			if (Menu_Level::checkChoosedEnter()) {
				if (Menu_Level::Choosed() == 1) {
					Menu_Level1 menulevel1;
					menulevel1.run(window);
					Menu_Level::setChoseEnter(false);
					break;
				}
				else if (Menu_Level::Choosed() == 2) {
					Menu_Level2 menulevel2;
					menulevel2.run(window);
					Menu_Level::setChoseEnter(false);
					break;
				}
				else if (Menu_Level::Choosed() == 3) {
					Menu_Level3 menulevel3;
					menulevel3.run(window);
					Menu_Level::setChoseEnter(false);
					break;
				}
				else if (Menu_Level::Choosed() == 4) {
					Menu_Level4 menulevel4;
					menulevel4.run(window);
					Menu_Level::setChoseEnter(false);
					break;
				}
				else {
					break;
				}
			}
			else {
				Menu_Level::run(window);
				draw(window);
			}
		}
	}
};