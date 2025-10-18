#pragma once
//#include "figure.h"
//#include "hider.h"
//#include "seeker.h"
//#include "sightrange.h"
//=====================================
#include <SFML/graphics.hpp>
#include <fstream>
#include "../figure.h"
#include "../hider.h"
#include "../seeker.h"
#include "../sightrange.h"


class Level {
private:
	bool CALLED_FUNCTION_LOADFROMFILE_AGENT = false;
	bool CALLED_FUNCTION_READ_MAP = false;

protected:
	std::vector<sf::Sprite> tilemap;
	std::vector <Figure*> seekers;
	std::vector <Figure*> hiders;
	sf::Sprite floorSprite;
	sf::Texture wallTexture;
	sf::Texture floorTexture;
	sf::Texture announceTexture;
	sf::Sprite announceSprite;
	std::vector<sf::Texture>movement;
	sf::Texture bloodspot;
	std::vector<sf::Sprite>blood;
	int** matrix = nullptr;
	int NUMROW, NUMCOL;
	int score = 0;
	float SPEED;
public:
	~Level() {
		for (int i = 0; i < NUMROW; i++) delete[] matrix[i];
		delete[] matrix;
	}
	void read_map(int**& matrix, int& n, int& m, const std::string& namemap) {
		if (CALLED_FUNCTION_READ_MAP == true) return;
		CALLED_FUNCTION_READ_MAP = true;
		std::ifstream Fin(namemap);
		Fin >> n >> m;
		matrix = new int* [n];
		for (int i = 0; i < n; i++) matrix[i] = new int[m];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				Fin >> matrix[i][j];
			}
		}
		Fin.close();
	}

	std::vector<sf::Sprite> make_table(int** matrix, int ROW, int COL, sf::Sprite floor, sf::Sprite wall) {
		std::vector <sf::Sprite> tilemap;
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				floor.setPosition(j * TILE_SIZE.x, i * TILE_SIZE.y);
				wall.setPosition(j * TILE_SIZE.x, i * TILE_SIZE.y);
				if (matrix[i][j] == 1) tilemap.push_back(wall);
				else tilemap.push_back(floor);
			}
		}
		return tilemap;
	}
	void change_dir_texture(int index, Figure* A, sf::Sprite dirSprite) {
		dirSprite.setTexture(movement[index]);
		dirSprite.setPosition(A->getPosition().y * TILE_SIZE.x, A->getPosition().x * TILE_SIZE.y);
		A->update(dirSprite);
	}
	bool animation_hider(Figure* A, sf::Vector2f to) {
		static sf::Texture dirTexture;
		sf::Sprite dirSprite;
		if (to == A->getPosition() + sf::Vector2f(-1, 0)) {
			change_dir_texture(8, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(-1, 1)) {
			change_dir_texture(9, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(0, 1)) {
			change_dir_texture(10, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(1, 1)) {
			change_dir_texture(11, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(1, 0)) {
			change_dir_texture(12, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(1, -1)) {
			change_dir_texture(13, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(0, -1)) {
			change_dir_texture(14, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(-1, -1)) {
			change_dir_texture(15, A, dirSprite);
		}

		sf::Vector2f AB = to - A->getPosition();
		float len = sqrt(AB.x * AB.x + AB.y * AB.y);
		A->move(AB.x / len * SPEED, AB.y / len * SPEED);
		if ((int)len == 0 && len <= SPEED) {
			A->setPosition(to.x, to.y);
			return true;
		}
		return false;
	}
	bool animation_seeker(Figure* A, sf::Vector2f to) {
		static sf::Texture dirTexture;
		sf::Sprite dirSprite;
		if (to == A->getPosition() + sf::Vector2f(-1, 0)) {
			change_dir_texture(0, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(-1, 1)) {
			change_dir_texture(1, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(0, 1)) {
			change_dir_texture(2, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(1, 1)) {
			change_dir_texture(3, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(1, 0)) {
			change_dir_texture(4, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(1, -1)) {
			change_dir_texture(5, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(0, -1)) {
			change_dir_texture(6, A, dirSprite);
		}
		else if (to == A->getPosition() + sf::Vector2f(-1, -1)) {
			change_dir_texture(7, A, dirSprite);
		}
		sf::Vector2f AB = to - A->getPosition();
		float len = sqrt(AB.x * AB.x + AB.y * AB.y);
		A->move(AB.x / len * SPEED, AB.y / len * SPEED);
		if ((int)len == 0 && len <= SPEED) {
			A->setPosition(to.x, to.y);
			return true;
		}
		return false;
	}

	void loadFromFile_Agent(sf::RenderWindow& window) {
		if (CALLED_FUNCTION_LOADFROMFILE_AGENT == true) return;
		CALLED_FUNCTION_LOADFROMFILE_AGENT = true;
		std::vector<std::string>grass = { "grass(1).jpg","grass(2).jpg","grass(3).jpg","grass(4).jpg" };
		std::shuffle(grass.begin(), grass.end(), rng);
		srand(time(0));
		int z = rand() % grass.size();
		floorTexture.loadFromFile("Assets/"+grass[z]);
		floorSprite.setTexture(floorTexture);
		//floorSprite.setTextureRect(sf::IntRect(0,40, 80, 80));
		//=============================================
		std::vector<std::string>bloods = { "blood(1).png","blood(2).png"};
		std::shuffle(bloods.begin(), bloods.end(), rng);
		z = rand() % bloods.size();
		bloodspot.loadFromFile("Assets/" + bloods[z]);
		//=============================================
		std::vector<std::string>obstacles = { "obstacle(1).png","obstacle(2).png","obstacle(3).png" ,"obstacle(4).png"
		,"obstacle(5).png"};
		std::shuffle(obstacles.begin(), obstacles.end(), rng);
		z = rand() % obstacles.size();
		sf::Sprite wallSprite;
		wallTexture.loadFromFile("Assets/"+obstacles[z]);
		wallSprite.setTexture(wallTexture);
		//wallSprite.setTextureRect(sf::IntRect(432, 69, 78, 78));
		//=============================================
		static sf::Texture seekerTexture;
		sf::Sprite seekerSprite;
		seekerTexture.loadFromFile("Assets/S_seeker.png");
		seekerSprite.setTexture(seekerTexture);
		//=============================================
		static sf::Texture hiderTexture;
		sf::Sprite hiderSprite;
		hiderTexture.loadFromFile("Assets/S_hider.png");
		hiderSprite.setTexture(hiderTexture);
		//=============================================
		announceTexture.loadFromFile("Assets/announce.png");
		announceSprite.setTexture(announceTexture);
		announceSprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
		seekers.clear();
		//=============================================
		std::vector<std::string>direct={ "N_seeker.png","NE_seeker.png","E_seeker.png","SE_seeker.png","S_seeker.png",
		"SW_seeker.png","W_seeker.png","NW_seeker.png","N_hider.png","NE_hider.png","E_hider.png","SE_hider.png","S_hider.png",
		"SW_hider.png","W_hider.png","NW_hider.png"};
		for (auto dir : direct) {
			sf::Texture tmp;
			tmp.loadFromFile("Assets/" + dir);
			movement.push_back(tmp);
		}
		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) if (matrix[i][j] == 3) {
				Figure* seeker = new Seeker;
				seeker->update(seekerSprite);
				seeker->setPosition(i, j);
				seekers.push_back(seeker);
			}
		}

		hiders.clear();
		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) if (matrix[i][j] == 2) {
				Figure* hider = new Hider;
				hider->update(hiderSprite);
				hider->setPosition(i, j);
				hiders.push_back(hider);
			}
		}
		tilemap = make_table(matrix, NUMROW, NUMCOL, floorSprite, wallSprite);
	}
	sf::Vector2f announcePosition(Figure* hider, int r) {
		sf::Vector2f pos = hider->getPosition();
		std::vector<sf::Vector2f>announcePos;
		for (int i = std::max(0, (int)pos.x - r); i <= std::min(NUMROW - 1, (int)pos.x + r); i++) {
			for (int j = std::max(0, (int)pos.y - r); j <= std::min(NUMCOL - 1, (int)pos.y + r); j++) {
				if (matrix[i][j] != 1 && matrix[i][j] != 2 && matrix[i][j] != 4) {
					announcePos.push_back(sf::Vector2f(i * 1.0, j * 1.0));
				}
			}
		}
		std::shuffle(announcePos.begin(), announcePos.end(), rng);
		srand(time(0));
		int z = rand() % announcePos.size();
		return announcePos[z];
	}
	sf::Vector2f forecast_in_blind(Figure* seeker, int r, std::vector<std::vector<int>>& visited) {

		sf::Vector2f pos = seeker->getPosition();
		std::vector<std::pair<int, int>>sight = seeker->getSight();
		std::vector<sf::Vector2f>forecast;
		for (int i = std::max(0, (int)pos.x - r); i <= std::min(NUMROW - 1, (int)pos.x + r); i++) {
			for (int j = std::max(0, (int)pos.y - r); j <= std::min(NUMCOL - 1, (int)pos.y + r); j++) {
				if ((matrix[i][j] != 1 && matrix[i][j] != 4) && visited[i][j] == 0) {
					forecast.push_back(sf::Vector2f(i * 1.0, j * 1.0));
				}
			}
		}
		//if (forecast.size() == 0) {
		//	std::cout << "er\n";
		//	for (auto x : sight) {
		//		visited[x.first][x.second] = 0;
		//		//forecast.push_back(sf::Vector2f(x.first * 1.0, x.second * 1.0));
		//	}
		//	return forecast_in_blind(seeker, r, visited);
		//}

		std::shuffle(forecast.begin(), forecast.end(), rng);
		srand(time(0));
		int z = rand() % forecast.size();
		return forecast[z];
	}
	void window_Draw(sf::RenderWindow& window) {
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

	void seeker_move_to_pos(sf::RenderWindow& window, Figure* seeker, sf::Vector2f pos, int R) {
		matrix[(int)seeker->getPosition().x][(int)seeker->getPosition().y] = 0;
		while (!animation_seeker(seeker, pos)) {
			std::vector<std::pair<int, int>>seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
			seeker->update_sight(seekerSight);
			window_Draw(window);
		}
		std::vector<std::pair<int, int>>seekerSight = sightRange(matrix, NUMROW, NUMCOL, seeker, R);
		seeker->update_sight(seekerSight);
		matrix[(int)seekers[0]->getPosition().x][(int)seekers[0]->getPosition().y] = 3;
	}

	void erase_hider(std::vector<Figure*>& hiders, Figure* hider_erase, bool bld = 1) {
		for (int i = 0; i < hiders.size(); i++) {
			if (hiders[i] == hider_erase) {
				if (bld) {
					sf::Sprite tmp;
					tmp.setTexture(bloodspot);
					tmp.setPosition(hiders[i]->getPosition().y * TILE_SIZE.x, hiders[i]->getPosition().x * TILE_SIZE.y);
					blood.push_back(tmp);
				}
				hiders.erase(hiders.begin() + i);
				break;
			}
		}
	}
};

