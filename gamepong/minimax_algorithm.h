#pragma once
#include <SFML/Graphics.hpp>

extern const int INF;
extern std::mt19937 rng;

bool inside_matrix(int x, int y, int NUMROW, int NUMCOL) {
	return 0 <= x && x < NUMROW && 0 <= y && y < NUMCOL;
}

std::vector <std::pair<sf::Vector2f, int>> T, shw;
int mininummax(sf::Vector2f hider, sf::Vector2f seeker, bool turn/* true: hider*/, int deep, int** matrix, int NUMROW, int NUMCOL,
					int alpha = -INF, int beta = INF) {

	//
	// Chung minh: Vi hider gap seeker truc tiep lam giam di so buoc (nen TH hider di vao o cua seeker khong bao gio say ra)
	//

	if (deep == 0) return INF;

	if (seeker == hider) return 0;

	if (turn == true) {
		int Max = 0;
		for (int j = 0; j < 8; j++) {
			int x = hider.x + h[j];
			int y = hider.y + c[j];
			if (inside_matrix(x, y, NUMROW, NUMCOL) && matrix[x][y] != 1 && matrix[x][y] != 2 && matrix[x][y] != 4) {
				Max = std::max(Max, mininummax({ 1.0f * x, 1.0f * y }, seeker, false, deep - 1, matrix, NUMROW, NUMCOL, alpha, beta) + 1);
				alpha = std::max(alpha, Max);
				if (beta <= alpha) break;
			}
		}
		return Max;
	}

	int Min = INF;
	for (int j = 0; j < 8; j++) {
		int x = seeker.x + h[j];
		int y = seeker.y + c[j];
		if (inside_matrix(x, y, NUMROW, NUMCOL) && matrix[x][y] != 1 && matrix[x][y] != 4) {
			Min = std::min(Min, mininummax(hider, { 1.0f * x, 1.0f * y }, true, deep, matrix, NUMROW, NUMCOL, alpha, beta));
			beta = std::min(beta, Min);
			if (beta <= alpha) break;
		}
	}
	return Min;
}


sf::Vector2f nextPos_in_area(Figure* hider, int is_seeker, int step_max, int R, int** matrix, int NUMCOL, int NUMROW) {
	sf::Vector2f res_dic = { -1, -1 };
	int Max = 0;
	for (const auto& pos: hider->getSight()) {
		if (pos.first == hider->getPosition().x && pos.second == hider->getPosition().y) continue;
		int seeker_x = pos.first;
		int seeker_y = pos.second;
		if (inside_matrix(seeker_x, seeker_y, NUMROW, NUMCOL) && matrix[seeker_x][seeker_y] == is_seeker) {
			for (int dic = 0; dic < sizeof(h) / sizeof(h[0]); dic++) {
				int new_x = hider->getPosition().x + h[dic];
				int new_y = hider->getPosition().y + c[dic];
				if (inside_matrix(new_x, new_y, NUMROW, NUMCOL) && matrix[new_x][new_y] == 0) {
					int val = mininummax({ 1.0f * new_x, 1.0f * new_y }, { 1.0f * seeker_x, 1.0f * seeker_y }, false, step_max, matrix, NUMROW, NUMCOL);
					
					//
					// step = step_max, buoc di toi da ma nhan vat tinh duoc  
					// 

					if (Max < val) {
						Max = val;
						res_dic = { 1.0f * new_x, 1.0f * new_y };
					}
				}
			}
		}
	}

	if (res_dic.x != -1) return res_dic;
	// =======
	std::vector <int> pos;
	for (int i = 0; i < sizeof(h) / sizeof(h[0]); i++) {
		int x = hider->getPosition().x + h[i];
		int y = hider->getPosition().y + c[i];
		if (inside_matrix(x, y, NUMROW, NUMCOL) && matrix[x][y] == 0) {
			for (const auto& k : hider->getSight()) if (std::pair<int,int>(x, y) == k) {
				pos.push_back(i);
				break;
			}
		}
	}

	if (pos.size() == 0) return hider->getPosition();

	int z = rand() % pos.size();
	res_dic.x = hider->getPosition().x + h[pos[z]];
	res_dic.y = hider->getPosition().y + c[pos[z]];

	return res_dic;
}