#pragma once
#include "menulevel.h"
#include "../Level/Level3/include/level3.h"
#include"../Menu/WinLose.h"
class Menu_Level3 : public Menu_Level {
public:

    Menu_Level3() {
        Menu_Level::update_background("Assets/Menu.png");
        Menu_Level::update_listcontext(std::vector<std::string>{"Map 1", "Map 2", "Map 3", "Map 4", "Map 5"});
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
                Level3 level3;
                std::string nameMap = "map" + std::to_string(Menu_Level::Choosed()) + ".txt";
                level3.run(window, nameMap);
                sf::sleep(sf::seconds(1));
                WinLose win;
                Menu_Level::setChoseEnter(false);
                if (win.run(window)) {
                    break;
                }
                Menu_Level::setChoseEnter(true);
            }
            else {
                Menu_Level::run(window);
                draw(window);
            }
        }
    }
};