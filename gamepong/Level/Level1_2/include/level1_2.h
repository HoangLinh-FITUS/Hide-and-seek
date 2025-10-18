#pragma once
#include "../../level.h"
#include"../gamepong/AstarSearch.h"
#include"../gamepong/bfs_algorithm.h"
// ==========================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <intrin.h>

class Level1_2 : private Level {
private:
	const std::string& PATH_MAP = "Level/Level1_2/";
	const int R = 3;
	const int STEP_MAX = 10; // dieu kien BAT BUOC: R <= STEP_MAX 
	std::vector <std::vector<int>> dist_hider;
public:
	Level1_2() {
		Level::SPEED = 0.5;
	}
	void calc_distance_allAgent(const std::vector<Figure*>& hiders, std::vector<std::vector<int>>& dist_hider, bool pos = 0) {
		dist_hider.assign(hiders.size(), std::vector<int>(hiders.size(), 0));

		for (int i = 0; i < (int)hiders.size() - 1; i++) {
			for (int j = i + 1; j < (int)hiders.size(); j++) {
				int d;
				if (pos) d = bfs(hiders[i]->getPosition(), hiders[j]->getPosition(), matrix, NUMROW, NUMCOL).size();
				else d = bfs(hiders[i]->getAnnouncePosition(), hiders[j]->getAnnouncePosition(), matrix, NUMROW, NUMCOL).size();
				dist_hider[i][j] = d;
				dist_hider[j][i] = d;
			}
		}
	}

	std::vector <Figure*> tours_seeker(Figure* seeker, std::vector<Figure*> hiders, bool b = 0) {
		hiders.push_back(seeker);
		calc_distance_allAgent(hiders, dist_hider, b);

		int n = hiders.size();
		std::vector <std::vector<int>> dp(n, std::vector<int>(1 << n, INF));
		std::vector <std::vector<int>> trace(n, std::vector<int>(1 << n, -1));
		for (int i = 0; i < n; i++) dp[i][0] = 0;

		for (int mask = 0; mask < (1 << n); mask++) {
			for (int i = 0; i < n; i++) {
				for (int tmp = mask, j; tmp; tmp ^= (1 << j)) {
					j = 31 - __lzcnt(tmp);
					int x = dp[j][mask ^ (1 << j)] + dist_hider[i][j];
					if (dp[i][mask] > x) {
						dp[i][mask] = x;
						trace[i][mask] = j;
					}
				}
			}
		}
		int pos = n - 1;
		int mask = (1 << n) - 1;
		std::vector <Figure*> res;
		while (pos != -1) {
			if (pos != n - 1) res.push_back(hiders[pos]);
			mask ^= (1 << pos);
			pos = trace[pos][mask];
		}
		return res;
	}
	void seeker_find(sf::RenderWindow& window, std::vector <sf::Vector2f> path, Figure* seeker, std::vector<Figure*>& announces, bool& check, std::vector<std::vector<int>>& visited, Figure* tmp) {
		for (auto& x : path) {
			visited[x.x][x.y] = 1;
			Level::seeker_move_to_pos(window, seeker, x, R);
			std::vector<Figure*>hider_in_sight;
			for (auto& y : seeker->getSight()) {
				visited[y.first][y.second] = 1;
				if (matrix[y.first][y.second] == 2) {
					for (auto hider : hiders) {
						if (hider->getPosition() == sf::Vector2f(y.first * 1.0, y.second * 1.0)) hider_in_sight.push_back(hider);
					}
					matrix[y.first][y.second] == 0;
				}
			}
			std::vector<Figure*>hider_order = tours_seeker(seeker, hider_in_sight, 1);
			for (auto order : hider_order) {
				std::vector <sf::Vector2f> tmpPath = aStarSearch(seeker->getPosition(), order->getPosition(), matrix, NUMROW, NUMCOL);
				for (auto& z : tmpPath) {
					visited[z.x][z.y] = 1;
					Level::seeker_move_to_pos(window, seeker, z, R);
				}
				for (auto hider : hiders) {
					if (hider->getPosition() == order->getPosition()) {
						Level::score += 20;
						erase_hider(hiders, hider);
						erase_hider(announces, hider,1);
						break;
					}
				}
			}
			if (hiders.empty() || tmp->getPosition() != announces[0]->getPosition()) break;
		}
	}

	void seeker_find_hider(sf::RenderWindow& window, int r) {
		for (auto& seeker : seekers) {
			std::vector<std::vector<int>>visited(NUMROW, std::vector<int>(NUMCOL, 0));
			std::vector<std::pair<int, int>>seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
			seeker->update_sight(seekerSight);
			std::vector <Figure*> announces = tours_seeker(seeker, hiders);
			Figure* tmp = nullptr;
			while (!announces.empty()) {
				bool check = 0;
				tmp = announces[0];
				std::vector <sf::Vector2f> path = aStarSearch(seeker->getPosition(), announces[0]->getAnnouncePosition(), matrix, NUMROW, NUMCOL);
				seeker_find(window, path, seeker, announces, check, visited, tmp);
				while (std::find(announces.begin(), announces.end(), tmp) != announces.end()) {
					sf::Vector2f forecast = Level::forecast_in_blind(tmp, r, visited);
					path = aStarSearch(seeker->getPosition(), forecast, matrix, NUMROW, NUMCOL);
					seeker_find(window, path, seeker, announces, check, visited, tmp);

				}
			}
		}
	}
	void run(sf::RenderWindow& window, const std::string& NAME_MAP) {
		window.clear();
		Level::read_map(matrix, NUMROW, NUMCOL, PATH_MAP + NAME_MAP);
		Level::loadFromFile_Agent(window);
		int r = 3;

		for (auto hider : hiders) {
			sf::Vector2f pos = Level::announcePosition(hider, r);
			hider->updateAnnounce(announceSprite);
			hider->setAnnouncePosition(pos.x, pos.y);
		}
		seeker_find_hider(window, r);
		window_Draw(window);
	}
};
