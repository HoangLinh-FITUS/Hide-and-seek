#pragma once
#include <set>
#include<vector>
#include"figure.h"
#include "bfs_algorithm.h"
#include "level\level.h"

class Cell
{
public:
	std::pair<int, int>parent;
	float f, g, h;
	Cell() {
		f = FLT_MAX;
		g = FLT_MAX;
		h = FLT_MAX;
		parent = { -1,-1 };
	}
};

bool isValid(int row, int col, int NUMROW, int NUMCOL)
{
	return (row >= 0) && (row < NUMROW) && (col >= 0) && (col < NUMCOL);
}

bool isUnBlocked(int** matrix, int row, int col)
{
	return matrix[row][col]!=1 && matrix[row][col] != 4;
}

bool isDestination(int row, int col, sf::Vector2f dest)
{
	return row == dest.x && col == dest.y;
}
float calculateHValue(int row, int col, sf::Vector2f dest)
{
	float dx = abs(row - dest.x);
	float dy = abs(col - dest.y);
	return dx > dy ? dx - dy + sqrt(2) * dy : dy - dx + sqrt(2) * dx;
}

std::vector <sf::Vector2f> tracePath(std::vector<std::vector<Cell>>cellPath, sf::Vector2f S, sf::Vector2f T)
{
	std::vector <sf::Vector2f> res;
	while (T != S) {
		res.push_back(T);
		std::pair <int, int> tv = cellPath[T.x][T.y].parent;
		T.x = tv.first;
		T.y = tv.second;
	}
	reverse(res.begin(), res.end());
	return res;
}
//Figure* A, Figure* B, int** matrix, int NUMROW, int NUMCOL, int step_limited = 100
std::vector <sf::Vector2f> aStarSearch(sf::Vector2f A, sf::Vector2f B, int** matrix, int NUMROW, int NUMCOL) {
	std::vector<std::vector<bool>>visited(NUMROW, std::vector<bool>(NUMCOL, false));
	std::vector<std::vector<Cell>> cellPath(NUMROW, std::vector<Cell>(NUMCOL));
	int row, col;
	row = A.x;
	col = A.y;
	cellPath[row][col].f = 0.0;
	cellPath[row][col].g = 0.0;
	cellPath[row][col].h = 0.0;
	cellPath[row][col].parent = { row,col };
	//Store f and coordinate in non-descending order
	std::set<std::pair<float, std::pair<int, int>>>candidateCells;
	candidateCells.insert({ 0.0,{row,col} });
	while (!candidateCells.empty()) {
		std::pair<float, std::pair<int, int>>tmp = *candidateCells.begin();
		candidateCells.erase(candidateCells.begin());
		row = tmp.second.first;
		col = tmp.second.second;
		visited[row][col] = 1;
		float fNew, gNew, hNew;
		for (int i = 0; i < 8; i++) {
			if (isValid(row + h[i], col + c[i], NUMROW, NUMCOL)) {
				if (isDestination(row + h[i], col + c[i], B)) {
					cellPath[row + h[i]][col + c[i]].parent = { row,col };
					return tracePath(cellPath, A, B);
				}
				else if (!visited[row + h[i]][col + c[i]] && isUnBlocked(matrix, row + h[i], col + c[i])) {
					gNew = cellPath[row][col].g + 1.0;
					hNew = calculateHValue(row + h[i],col + c[i], B);
					fNew = gNew + hNew;
					if (cellPath[row + h[i]][col + c[i]].f == FLT_MAX || cellPath[row + h[i]][col + c[i]].f > fNew)
					{
						candidateCells.insert({ fNew, {row + h[i],col + c[i]} });
						cellPath[row + h[i]][col + c[i]].f = fNew;
						cellPath[row + h[i]][col + c[i]].g = gNew;
						cellPath[row + h[i]][col + c[i]].h = hNew;
						cellPath[row + h[i]][col + c[i]].parent = { row,col };
					}
				}
			}
		}
	} 
	std::vector <sf::Vector2f> res;
	std::vector <std::pair<int, int>> pos;
	for (int j = 0; j < 8; j++) {
		int x = A.x + h[j];
		int y = A.y + c[j];
		if (0 <= x && x < NUMROW && 0 <= y && y < NUMCOL && matrix[x][y] != 1 && matrix[x][y] != 4) {
			pos.push_back({ x, y });
		}
	}
	int x = rand() % pos.size();
	return { {1.0f * pos[x].first, 1.0f * pos[x].second } };
}