#pragma once
#include "../gamepong/Level/level.h"
#include "../gamepong/AstarSearch.h"
#include "../gamepong/bfs_algorithm.h"
// ==========================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

class Level3 : private Level {
private:
	const std::string& PATH_MAP = "Level/Level3/map/";
	const int R = 2;
	const int STEP_MAX = 6; // dieu kien BAT BUOC: R <= STEP_MAX 
	std::vector <Figure*> hiders_announce;
public:
	enum {
		SEEKER = 3, HIDER = 2
	};
	Level3() {
		Level::SPEED = 0.5;
	}

	void seeker_find_hider(sf::RenderWindow& window, std::vector <Figure*> &to_be_captured, bool bld = 1) {
		for (auto& seeker : seekers) {
			if (to_be_captured.size() == 0) continue;

			std::vector<std::pair<int, int>>seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
			seeker->update_sight(seekerSight);

			int Min = INF;
			Figure* to_be_captured_deleted = nullptr;
			sf::Vector2f direction;
			for (const auto& hider : to_be_captured) {
				std::vector <sf::Vector2f> path = aStarSearch(seeker->getPosition(), hider->getPosition(), matrix, NUMROW, NUMCOL);

				//std::vector <sf::Vector2f> path = bfs(seeker, hider, matrix, NUMROW, NUMCOL);
				
				if (path.size() == 0) continue;
				if (Min > path.size()) {
					Min = path.size();
					to_be_captured_deleted = hider;
					direction = path[0];
				}
			}
			if (to_be_captured_deleted == nullptr) continue;

			// ======== animation ==========
			matrix[(int)seeker->getPosition().x][(int)seeker->getPosition().y] = 0;
			while (!animation_seeker(seeker, direction)) {
				std::vector<std::pair<int, int>>seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
				seeker->update_sight(seekerSight);
				window_Draw(window);
			}
			seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
			seeker->update_sight(seekerSight);

			matrix[(int)seeker->getPosition().x][(int)seeker->getPosition().y] = 3;

			if (seeker->getPosition() == to_be_captured_deleted->getPosition()) {
				if (bld == 1) Level::score += 20;
				erase_hider(to_be_captured, to_be_captured_deleted,bld);
			}

			window_Draw(window);
		}
	}

	void hider_find_path(sf::RenderWindow& window) {
		for (auto& hider : hiders) {
			std::vector<std::pair<int, int>> hiderSight = sightRange(matrix, NUMROW, NUMCOL, hider, R);
			hider->update_sight(hiderSight);

			sf::Vector2f posNext = nextPos_in_area(hider, SEEKER, STEP_MAX, R, matrix, NUMCOL, NUMROW);
			matrix[(int)hider->getPosition().x][(int)hider->getPosition().y] = 0;

			while (!animation_hider(hider, posNext)) {
				std::vector<std::pair<int, int>> hiderSight = sightRange(matrix, NUMROW, NUMCOL, hider, R);
				hider->update_sight(hiderSight);
				window_Draw(window);
			}
			hiderSight = sightRange(matrix, NUMROW, NUMCOL, hider, R);
			hider->update_sight(hiderSight);

			hider->isStepping();
			matrix[(int)hider->getPosition().x][(int)hider->getPosition().y] = 2;
			window_Draw(window);
		}
	}


	bool seeker_see_hider(std::vector <Figure*>& seekers, std::vector<Figure*>& hiders) {
		for (const auto& seeker : seekers) {
			for (const auto &hider: hiders) {
				int x = hider->getPosition().x;
				int y = hider->getPosition().y;
				for (const auto& sight : seeker->getSight()) {
					if (std::pair<int, int>(x, y) == sight) return true;
				}
			}
		}
		return false;
	}

	void run(sf::RenderWindow& window, const std::string& NAME_MAP) {
		Level::read_map(matrix, NUMROW, NUMCOL, PATH_MAP + NAME_MAP);
		Level::loadFromFile_Agent(window);
		window_Draw(window);
		while (!hiders.empty()) {
			for (int i = 0; i < hiders.size(); i++) if (hiders[i]->checkstep_announce()) {
				hiders_announce.clear();
				break;
			}
			for (int i = 0; i < hiders.size(); i++) if (hiders[i]->checkstep_announce()) {
				sf::Vector2f pos = Level::announcePosition(hiders[i], 3);
				hiders[i]->updateAnnounce(announceSprite);
				hiders[i]->setAnnouncePosition(pos.x, pos.y);
				hiders_announce.push_back(new Hider);
				hiders_announce.back()->setPosition(hiders[i]->getAnnouncePosition().x, hiders[i]->getAnnouncePosition().y);
			}


			// =====================SEEKER tim hider =============================== 
			if (seeker_see_hider(seekers, hiders)) {
				seeker_find_hider(window, hiders);
			}
			else {
				seeker_find_hider(window, hiders_announce,0);
			}
			// ============ hider tim duong =================================
			hider_find_path(window);
		}
	}
};
