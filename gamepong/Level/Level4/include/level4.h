#pragma once
#include "Level/level.h"
#include "obstacle.h"
#include "Level\Level3\include\Level3.h"
// ==========================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

class Level4 : private Level {
private:
	bool CALLED_FUNCTION_READ_MAP_AND_OBSTACLE = false;

	const std::string& PATH_MAP = "Level/Level4/map/";
	const int R = 3;
	const int STEP_MAX = 6; // dieu kien BAT BUOC: R <= STEP_MAX 
	std::vector <Obstacle> input_obstacle;
	std::vector <Figure*> hiders_announce;
	bool ALLOW_PUSH_OBSTACLE = true;
	int score = 0;
	const int STEP_LIMITED = 100;
	const int STEP_BEFORE_START = 20;
public:
	Level4() {
		Level::SPEED = 1.0;
	}
	std::vector <Obstacle> read_map_and_obstacle(int**& matrix, int& NUMROW, int& NUMCOL, const std::string& NAME_FILE) {
		if (CALLED_FUNCTION_READ_MAP_AND_OBSTACLE == true) return input_obstacle;
		CALLED_FUNCTION_READ_MAP_AND_OBSTACLE = true;
		std::ifstream Fin(NAME_FILE);
		Fin >> NUMROW >> NUMCOL;
		matrix = new int* [NUMROW];
		for (int i = 0; i < NUMROW; i++) {
			matrix[i] = new int[NUMCOL];
			for (int j = 0; j < NUMCOL; j++) {
				Fin >> matrix[i][j];
			}
		}
		std::vector <Obstacle> input_obstacles;
		int a, b, c, d;
		while (Fin >> a >> b >> c >> d) {
			input_obstacles.push_back({ a - 1, b - 1 , c - 1 , d - 1 });
		}
		Fin.close();
		return input_obstacles;
	}
	bool check_intersection_obstacle(const Obstacle& obs) {
		for (auto& i : input_obstacle) if (&i != &obs && i.intersection(obs)) return true;
		return false;
	}
	void figure_push_obstacle(std::pair <int, int> pos_figure, Obstacle& obs, int** matrix, int NUMROW, int NUMCOL) {
		if (pos_figure.first > obs.c) {
			obs.move_up(matrix, NUMROW, NUMCOL);
			if (check_intersection_obstacle(obs)) obs.move_down(matrix, NUMROW, NUMCOL);
		}
		else if (pos_figure.first < obs.a) {
			obs.move_down(matrix, NUMROW, NUMCOL);
			if (check_intersection_obstacle(obs)) obs.move_up(matrix, NUMROW, NUMCOL);
		}
		else if (pos_figure.second < obs.b) {
			obs.move_right(matrix, NUMROW, NUMCOL);
			if (check_intersection_obstacle(obs)) obs.move_left(matrix, NUMROW, NUMCOL);
		}
		else if (pos_figure.second > obs.d) {
			obs.move_left(matrix, NUMROW, NUMCOL);
			if (check_intersection_obstacle(obs)) obs.move_right(matrix, NUMROW, NUMCOL);
		}
	}

	void update_input_obstacle(int **matrix, int NUMROW, int NUMCOL) {
		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) if (matrix[i][j] == 4) matrix[i][j] = 0;
		}
		for (auto& obstacle : input_obstacle) {
			for (int i = obstacle.a; i <= obstacle.c; i++) {
				for (int j = obstacle.b; j <= obstacle.d; j++) matrix[i][j] = 4;
			}
		}
	}
	void window_Draw(sf::RenderWindow& window) {
		// ============
		window.clear();
		for (auto& i : tilemap) {
			if (i.getTexture() == &wallTexture) {
				floorSprite.setPosition(i.getPosition().x, i.getPosition().y);
				window.draw(floorSprite);
			}
			window.draw(i);
		}
		for (auto i : blood) window.draw(i);
		for (auto i : hiders) i->draw(window);
		for (auto i : seekers) i->draw(window);

		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) if (matrix[i][j] == 4) {
				sf::RectangleShape rec(TILE_SIZE);
				rec.setPosition({ j * TILE_SIZE.x, i * TILE_SIZE.y });
				rec.setFillColor(sf::Color::Red);
				window.draw(rec);
			}
		}
		// ================
		sf::View view = window.getDefaultView();
		view.setCenter((NUMCOL + 1) * TILE_SIZE.x / 2, (NUMROW + 1) * TILE_SIZE.y / 2);
		window.setView(view);

		sf::Font font;
		font.loadFromFile("sixty/SIXTY.TTF");
		sf::Text text("Score: " + std::to_string(score), font);
		text.setPosition(0, 0);
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::Yellow);
		window.draw(text);

		window.display();
	}
	
	// ===============
	void will_push_obstacle(std::pair<int, int> pos_figure, int **matrix, int NUMROW, int NUMCOL) {
		for (int j = 0; j < 4; j++) {
			int x = pos_figure.first + h[j];
			int y = pos_figure.second + c[j];
			if (0 <= x && x < NUMROW && 0 <= y && y < NUMCOL && matrix[x][y] == 4) {
				for (auto& obstacle : input_obstacle) {
					Obstacle tmp(x, y, x, y);
					if (obstacle.intersection(tmp)) {
						figure_push_obstacle(pos_figure, obstacle, matrix, NUMROW, NUMCOL);
						break;
					}
				}
				break;
			}
		}
	}

	void decision_push(std::pair <int, int> pos, int** matrix, int NUMROW, int NUMCOL) {
		//srand(time(nullptr));
		if (rand() % 2 == 1) {
			will_push_obstacle(pos, matrix, NUMROW, NUMCOL);
			update_input_obstacle(matrix, NUMROW, NUMCOL);
		}

	}
	void seeker_find_hider(sf::RenderWindow& window, std::vector <Figure*>& to_be_captured, bool bld = 1) {
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
				if (bld == 1) score += 20;
				erase_hider(to_be_captured, to_be_captured_deleted, bld);
			}
			window_Draw(window);

			std::pair <int, int> tmp_pos = { seeker->getPosition().x, seeker->getPosition().y };
			decision_push(tmp_pos, matrix, NUMROW, NUMCOL);
			seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
			seeker->update_sight(seekerSight);
		}
	}

	void hider_find_path(sf::RenderWindow& window, bool ALLOW_PUSH_OBSTACLE) {
		for (auto& hider : hiders) {
			std::vector<std::pair<int, int>> hiderSight = sightRange(matrix, NUMROW, NUMCOL, hider, R);
			hider->update_sight(hiderSight);

			sf::Vector2f posNext = nextPos_in_area(hider, 3, STEP_MAX, R, matrix, NUMCOL, NUMROW);
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
			std::pair <int, int> tmp_pos = { hider->getPosition().x, hider->getPosition().y };
			window_Draw(window);
			//===========================
			std::pair <int, int> pos = { hider->getPosition().x, hider->getPosition().y };
			if (ALLOW_PUSH_OBSTACLE == false) continue; //========================== 
			decision_push(pos, matrix, NUMROW, NUMCOL);
			hiderSight = sightRange(matrix, NUMROW, NUMCOL, hider, R);
			hider->update_sight(hiderSight);
		}
	}


	bool seeker_see_hider(std::vector <Figure*>& seekers, std::vector<Figure*>& hiders) {
		for (const auto& seeker : seekers) {
			for (const auto& hider : hiders) {
				int x = hider->getPosition().x;
				int y = hider->getPosition().y;
				for (const auto& sight : seeker->getSight()) {
					if (std::pair<int, int>(x, y) == sight) return true;
				}
			}
		}
		return false;
	}

	void run_map(sf::RenderWindow& window, const std::string& NAME_MAP) {
		input_obstacle = read_map_and_obstacle(matrix, NUMROW, NUMCOL, PATH_MAP + NAME_MAP);
		Level::loadFromFile_Agent(window);
		update_input_obstacle(matrix, NUMROW, NUMCOL);
		window_Draw(window);

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

		/*std::vector<int>horizontal;
		for (int i = 0; i < NUMCOL; i++) {
			for (int j = 0; j < NUMROW; j++) {
				if (matrix[j][i] == 1 || (matrix[j][i] == 0 && j == NUMROW - 1)) {
					int flag;
					if (j > 0)
						flag = j - 1;
					else flag = j;
					while (matrix[flag][i] != 1 && flag > 0) {
						flag--;
					}
					if (j - flag + 1 - matrix[flag][i] - matrix[j][i] > 0) horizontal.push_back(j - flag + 1 - matrix[flag][i] - matrix[j][i]);
				}
			}
		} */
		// ======================
		std::pair <int, int> pos_seeker = { seekers[0]->getPosition().x, seekers[0]->getPosition().y };
		
		static int TMP = 0;
		TMP++;
		if (TMP == STEP_BEFORE_START) ALLOW_PUSH_OBSTACLE = false;

		if (ALLOW_PUSH_OBSTACLE == false) {
			if (seeker_see_hider(seekers, hiders)) {
				seeker_find_hider(window, hiders);
			}
			else {
				seeker_find_hider(window, hiders_announce, 0);
			}
		}
		hider_find_path(window, ALLOW_PUSH_OBSTACLE);
	}

	void run(sf::RenderWindow& window, const std::string& NAME_MAP) {
		int cnt = 0;
		do {
;			run_map(window, NAME_MAP);
			cnt += (1 - ALLOW_PUSH_OBSTACLE);
		} while (!hiders.empty() && cnt <= STEP_LIMITED);
	}
};
