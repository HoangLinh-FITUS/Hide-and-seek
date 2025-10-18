#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

extern const sf::Vector2f TILE_SIZE;

double distancePointToLine(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C)
{
	std::pair<int, int>AB = { -1 * (B.second - A.second),B.first - A.first };
	int c = -1 * A.first * AB.first + (-1 * A.second * AB.second);
	double distance = (double)abs(C.first * AB.first + C.second * AB.second + c) / sqrt(AB.first * AB.first + AB.second * AB.second);
	return distance;
}
bool hasObstacleBetweenTwoPoints(int** matrix, sf::Sprite seeker, std::pair<int, int> B)
{
	int Sx = seeker.getPosition().x / TILE_SIZE.x;
	int Sy = seeker.getPosition().y / TILE_SIZE.y;
	std::pair<int, int> A = { Sy,Sx };
	for (int i = std::min(A.first, B.first); i <= std::max(A.first, B.first); i++)
	{
		for (int j = std::min(A.second, B.second); j <= std::max(A.second, B.second); j++)
		{
			double distance = distancePointToLine(A, B, { i, j });
			// cout << i << " " << j << " " << distance << endl;
			if ((matrix[i][j] == 1 || matrix[i][j] == 4) && distance >= 0 && distance < ((sqrt(2) - 0.0001) / 2))
			{
				return 1;
			}
		}
	}
	return 0;
}
//std::vector<std::pair<int, int>> sightRange(int** matrix, sf::Sprite& seeker, int r)
std::vector<std::pair<int, int>> sightRange(int** matrix, int NUMROW, int NUMCOL, sf::Sprite& seeker, int r)
{
	std::vector<std::pair<int, int>> visibleCells;
	int Sx = seeker.getPosition().x / TILE_SIZE.x;
	int Sy = seeker.getPosition().y / TILE_SIZE.y;
	for (int i = std::max(0, Sy - r); i <= std::min(NUMROW - 1, Sy + r); ++i)
	{
		for (int j = std::max(0, Sx - r); j <= std::min(NUMCOL - 1, Sx + r); ++j)
		{

			if ((matrix[i][j] != 1 && matrix[i][j] != 4) && !hasObstacleBetweenTwoPoints(matrix, seeker, {i, j}))
			{
				visibleCells.push_back({ i, j });
			}

		}
	}
	return visibleCells;
}


// ===== ham ghep =======
std::vector <std::pair<int, int>> sightRange(int** matrix, int NUMROW, int NUMCOL, Figure* seeker, int r) {
    sf::Sprite seekerSprite;
    seekerSprite.setPosition(seeker->getPosition().y * TILE_SIZE.x, seeker->getPosition().x * TILE_SIZE.y);
    return sightRange(matrix, NUMROW, NUMCOL, seekerSprite, r);
}
