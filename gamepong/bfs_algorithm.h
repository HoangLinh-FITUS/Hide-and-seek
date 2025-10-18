#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

std::vector <sf::Vector2f> bfs(sf::Vector2f S, sf::Vector2f T, int** matrix, int NUMROW, int NUMCOL, int step_limited = 100) {

	std::vector <std::vector<int>> dist(NUMROW, std::vector<int>(NUMCOL, 0));
	std::vector <std::vector<std::pair<int, int>>> trace(NUMROW, std::vector<std::pair<int, int>>(NUMCOL, { 0,0 }));

	std::queue <sf::Vector2f> q;
	q.push(S);
	dist[S.x][S.y] = 1;

	while (!q.empty()) {
		sf::Vector2f u = q.front();
		q.pop();
		if (dist[u.x][u.y] >= step_limited) continue;
		for (int j = 0; j < sizeof(h) / sizeof(h[0]); j++) {
			sf::Vector2f v = u + sf::Vector2f(h[j], c[j]);
			if (0 <= v.x && v.x < NUMROW && 0 <= v.y && v.y < NUMCOL && dist[v.x][v.y] == 0) {
				if (matrix[(int)v.x][(int)v.y] == 1) continue;
				dist[v.x][v.y] = dist[u.x][u.y] + 1;
				trace[v.x][v.y] = { u.x, u.y };
				q.push(v);
			}
		}
	}
	std::vector <sf::Vector2f> res;
	if (dist[(int)T.x][(int)T.y] == 0) {
		std::vector <std::pair<int, int>> pos;
		for (int j = 0; j < 8; j++) {
			int x = S.x + h[j];
			int y = S.y + c[j];
			if (0 <= x && x < NUMROW && 0 <= y && y < NUMCOL && matrix[x][y] != 1 && matrix[x][y] != 4) {
				pos.push_back({ x, y });
			}
		}
		srand(time(nullptr));
		int x = rand() % pos.size();
		res.push_back({ 1.0f * pos[x].first, 1.0f * pos[x].second });
		return res;
	}

	while (T != S) {
		res.push_back(T);
		std::pair <int, int> tv = trace[T.x][T.y];
		T.x = tv.first;
		T.y = tv.second;
	}
	reverse(res.begin(), res.end());
	return res;
}
