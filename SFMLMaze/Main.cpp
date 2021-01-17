//Maze generator made by Erik Nilsson
//finish date 10/28/19
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
//#include <time.h>
#include <thread>
//#include <Windows.h>
namespace f = std::experimental::filesystem;

using namespace std;

//classes
class Position {
public:
	int x;
	int y;

	Position() {
		x = 0;
		y = 0;
	}

	Position(int nx, int ny) {
		x = nx;
		y = ny;
	}
};

class Player {
public:
	int x;
	int y;
	int width;
	int height;
	sf::Color color;

	Player() {
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		color = sf::Color::Blue;
	}

	Player(int nx, int ny, int nw, int nh) {
		x = nx;
		y = ny;
		width = nw;
		height = nh;
		color = sf::Color::Blue;
	}

	void setSize(float scale) {
		width = scale * 0.8;
		height = scale * 0.8;
	}

	void setPosition(float nx, float ny) {
		x = nx;
		y = ny;
	}
	sf::RectangleShape getShape() {
		sf::RectangleShape rect(sf::Vector2f(width, height));
		rect.setPosition(sf::Vector2f(x, y));
		rect.setFillColor(color);
		return rect;
	}
};

//program start
const int FPS = 30;
const string FILE_EXT = ".maze";

int stage;
string filePath;
vector<vector<int>> Map;
Player player;
ifstream iFile;
ofstream oFile;

sf::RenderWindow window;
const int WIDTH = 800;
const int HEIGHT = 800;

void listDirs() {
	string path = f::current_path().u8string() + "/maps";
	for (const auto& entry : f::directory_iterator(f::path(path)))
		cout << "--" << entry.path().u8string().substr(path.length()+1, entry.path().u8string().length()-(path.length()+FILE_EXT.length()+1)) << endl;
}

void saveMap(vector<vector<int>> map, string name) {
	string fileP = f::current_path().u8string() + "/maps/" + name + FILE_EXT;
	oFile.open(fileP);
	for (int i = 0; i < map.size(); i++) {
		if (i != 0) {
			//cout << endl;
			oFile << endl;
		}
		string line = "";
		for (int g = 0; g < map[0].size(); g++) {
			line += (to_string(map[i][g]));
			if (g < map[0].size()-1) {
				line += " ";
			}
		}
		//cout << line;
		oFile << line;
	}

	oFile.close();
}

int numberoflines(){
	int number_of_lines = 0;
	string line;
	ifstream myfile(filePath);

	if (myfile.is_open()) {
		while (!myfile.eof()) {
			getline(myfile, line);
			cout << line << endl;
			number_of_lines++;
		}
		myfile.close();
	}

	return number_of_lines;
}

float getScale(vector<vector<int>> map) {
	float xScale = (WIDTH / map[0].size());
	float yScale = (HEIGHT / map.size());
	float scale;

	if (xScale > yScale) {
		scale = yScale;
	}
	else {
		scale = xScale;
	}

	return scale;
}

void draw(vector<vector<int>> map) {
	//float scale = getScale(map);

	float scale = float(WIDTH)/map.size();

	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			sf::RectangleShape square(sf::Vector2f(scale, scale));
			square.setPosition(sf::Vector2f(x * scale, y * scale));
			if (map[y][x] == 0) {
				square.setFillColor(sf::Color::White);
			} 
			else if (map[y][x] == 1) {
				square.setFillColor(sf::Color::Black);
			}
			else if (map[y][x] == 2) {
				square.setFillColor(sf::Color::Green);
			}
			else if (map[y][x] == 3) {
				square.setFillColor(sf::Color::Red);
			}

			window.draw(square);
		}
	}
}

void draw(Player p) {
	window.draw(p.getShape());
}

//the recursive map generator
vector<vector<int>> genMap(vector<vector<int>> map, int x, int y, int width, int height, int delay) {
	int randX = 1;
	int randY = 1;
	if (width >= 3) {
		srand(time(NULL));
		randX = rand() % (width - 2) + 1;
		while (randX % 2 == 0) {
			randX = rand() % (width - 2) + 1;
		}
	}
	if (height >= 3) {
		srand(time(NULL));
		randY = rand() % (height - 2) + 1;
		while (randY % 2 == 0) {
			randY = rand() % (height - 2) + 1;
		}
	}

	if (width >= 3 && height >=3) {
		for (int i = y; i < y + height; i++) {
			map[i][randX + x] = 1;
		}
		if (map[y - 1][randX + x] == 0) {
			map[y][randX + x] = 0;
		}
		if (map[y + height][randX + x] == 0) {
			map[y + height - 1][randX + x] = 0;
		}
	}
	if (height >= 3 && width >=3) {
		for (int i = x; i < x + width; i++) {
			map[randY + y][i] = 1;
		}
		if (map[randY + y][x - 1] == 0) {
			map[randY + y][x] = 0;
		}
		if (map[randY + y][x + width] == 0) {
			map[randY + y][x + width - 1] = 0;
		}
	}


	
	srand(time(NULL));
	int randSide = rand() % 4;
	srand(time(NULL));
	if (height >= 3 && width >= 3) {
		if (randSide != 0 && height >= 3) {
			map[rand() % (randY)+y][randX + x] = 0;
		}
		srand(time(NULL));
		if (randSide != 1 && width >= 3) {
			map[randY + y][rand() % ((width - randX) - 1) + x + randX + 1] = 0;
		}
		srand(time(NULL));
		if (randSide != 2 && height >= 3) {
			map[rand() % ((height - randY) - 1) + y + randY + 1][randX + x] = 0;
		}
		srand(time(NULL));
		if (randSide != 3 && width >= 3) {
			map[randY + y][rand() % (randX)+x] = 0;
		}
	}
	//delay for recursive viewing
	if (delay > 0) {
		//for (int i = 0; i < delay; i++) {
		window.clear();
		sf::Event e;
		window.pollEvent(e);
		draw(map);
		window.display();
		this_thread::sleep_for(chrono::milliseconds(delay));
		//cout << "runing" << endl;
	}

	//return map;
	if (!(width >= 3 && height >= 3)) {
		return map;
	}
	else {
		map = genMap(map, x, y, randX, randY,delay);
		map = genMap(map, x + randX + 1, y, (width-randX) - 1, randY,delay);
		map = genMap(map, x, y + randY + 1, randX, (height-randY) - 1,delay);
		map = genMap(map, x + randX + 1, y + randY + 1, (width - randX) - 1, (height - randY) - 1,delay);
		
		return map;
	}
}

vector<vector<int>> generateMap(int width, int height, int delay) {
	vector<vector<int>> newMap;

	//init map
	for (int y = 0; y < height; y++) {
		vector<int> newVec;
		newMap.push_back(newVec);
		for (int x = 0; x < width; x++) {
			if (x > 0 && x < width-1 && y > 0 && y < height-1) {
				newMap[y].push_back(0);
			}
			else {
				newMap[y].push_back(1);
			}
		}
	}
	newMap[1][1] = 2;
	newMap[width - 2][height - 2] = 3;

	//recursive path changing
	newMap = genMap(newMap,1,1,width-2,height-2,delay);

	return newMap;
}

bool collisionCheck(vector<vector<int>> map, float x1, float y1, float w, float h) {
	if (map[(x1 / getScale(Map))][(y1 / getScale(Map))] == 1) {
		return true;
	}
	/*
	if (map[floor((x1 + w) / getScale(Map))][floor((y1 + h) / getScale(Map))] == 1) {
		return true;
	}*/
	cout << map[(x1 / getScale(Map))][(y1 / getScale(Map))] << endl;

	return false;
}

float move(Player p, char axis) {
	const int SPEED = getScale(Map)/10;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (!collisionCheck(Map, p.x, p.y - SPEED, p.width, p.height)) {
			p.y -= SPEED;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (!collisionCheck(Map, p.x, p.y + SPEED, p.width, p.height)) {
			p.y += SPEED;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (!collisionCheck(Map, p.x - SPEED, p.y, p.width, p.height)) {
			p.x -= SPEED;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (!collisionCheck(Map, p.x + SPEED, p.y, p.width, p.height)) {
			p.x += SPEED;
		}
	}

	if (axis == 'x') {
		return p.x;
	}
	if (axis == 'y') {
		return p.y;
	}
}

void setup() {
	stage = 0;
	string input;
	Position start;
	Position end;
	int delay = 0;

	Map = std::vector<std::vector<int>>();

	cout << "Would you like to Generate a Maze?(yes/no)" << endl;
	cin >> input;
	if (input == "yes") {
		cout << "Enter render delay for examples (0 for fastest/any positive for delay in millisec) RECOMMENDED 10ms" << endl;
		cin >> delay;
		cout << "Enter the size of the maze you want." << endl;
		int num;
		cin >> num;
		if (delay > 0) {
			window.create(sf::VideoMode(WIDTH, HEIGHT), "Maze");
			//window.setFramerateLimit(1000 / FPS);
		}
		Map = generateMap(num,num,delay);
		start = Position(1, 1);
		end = Position(Map.size() - 2, Map.size() - 2);
	}
	else {
		cout << "Mazes:" << endl;
		listDirs();
		cout << "Enter the name of the Maze file you want to start: " << endl;
		cin >> input;

		filePath = f::current_path().u8string() + "/maps/" + input + FILE_EXT;
		iFile.open(filePath);

		int count = 0;
		cout << "Now running: " << input << endl;
		cout << filePath << endl << endl;
		cout << "Preview:" << endl;
		while (!iFile.eof()) {
			string line;
			getline(iFile, line);
			vector<int> myRow;
			Map.push_back(myRow);
			for (int i = 0; i < line.length(); i++) {
				if (!(line[i] == ' ')) {
					Map[count].push_back((int)line[i] - 48);
				}
				if (line[i] - 48 == 2) {
					//start = Position(count, i - 1);
				}
				if (line[i] - 48 == 3) {
					//end = Position(count, i - 1);
				}
				//cout << line[i];
			}
			//cout << endl;
			if (line.length() != 0) {
				count += 1;
			}
		}
	}
	start.x *= getScale(Map);
	start.y *= getScale(Map);

	if (delay == 0) {
		window.create(sf::VideoMode(WIDTH, HEIGHT), "Maze");
		window.setFramerateLimit(1000 / FPS);
	}

	/*
	player = Player();
	player.setSize(getScale(Map));
	player.setPosition(start.x + getScale(Map)*0.1, start.y + getScale(Map)*0.1);
	*/
}

int main() {
	//sf::RectangleShape shape(sf::Vector2f(100.0f,100.0f));
	//shape.setFillColor(sf::Color::Blue);

	setup();
	while (window.isOpen()) {
		//basic window runner
		sf::Event event;
		if (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				cout << "Would you like to save that map? (yes/no)" << endl;
				string input;
				cin >> input;
				if (input == "yes") {
					cout << "What would you like to name the file?" << endl;
					string input;
					cin >> input;
					saveMap(Map, input);
				}
				cout << endl << "----------------------------------" << endl;
				main();
				return 0;
			}
		}

		//player.setPosition(move(player,'x'), move(player,'y'));

		window.clear();
		draw(Map);
		//draw(player);
		window.display();

		/*
		window.clear();
		shape.setPosition(sf::Vector2f(100, 100));
		window.draw(shape);
		window.display();
		*/

	}

	return 0;
};