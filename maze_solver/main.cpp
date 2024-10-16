#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <sstream>

#include "../common/utils.h"

#define WALL_CHAR '#'
#define VISITED_CHAR 'V'
#define PASSAGE_CHAR '.'
#define START_CHAR 'S'
#define END_CHAR 'E'

enum direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct xy
{
	int32_t x;
	int32_t y;
};

bool operator==(const xy &a, const xy &b)
{
	return a.x == b.x && a.y == b.y;
}

struct node
{
	uint8_t dir_that_led_here = 0;
	xy coords;
};

bool IsAWallThere(const xy &coord, const direction &d, const std::vector<std::string> &maze)
{
	xy new_coords = coord;

	switch (d)
	{
	case direction::UP:
		new_coords.y--;
		break;
	case direction::RIGHT:
		new_coords.x++;
		break;
	case direction::DOWN:
		new_coords.y++;
		break;
	case direction::LEFT:
		new_coords.x--;
		break;
	}

	// Check bounds
	if (new_coords.y < 0 || new_coords.x < 0 || new_coords.y >= (int32_t)maze.size() || new_coords.x >= (int32_t)maze.at(new_coords.y).size())
	{
		return true;
	}

	if (maze.at(new_coords.y).at(new_coords.x) == WALL_CHAR || maze.at(new_coords.y).at(new_coords.x) == VISITED_CHAR)
	{
		return true;
	}

	return false;
}

uint8_t GetFirstFreeDir(const uint8_t &dir)
{
	for (size_t i = 0; i < 4; i++)
	{
		if ((dir & 1 << i) == 0)
		{
			return 1 << i;
		}
	}
	return 0;
}

uint8_t CheckAvailableDirs(const xy &coord, const std::vector<std::string> &maze)
{
	// Bits are set to 1 if that direction is visited or blocked
	// 0000 0000
	//	     ^^^^
	//      URDL
	// U -> Up
	// R -> Right
	// D -> Down
	// L -> Left
	uint8_t dirs = 0;
	dirs |= IsAWallThere(coord, direction::UP, maze) ? 0b1000 : 0;
	dirs |= IsAWallThere(coord, direction::RIGHT, maze) ? 0b100 : 0;
	dirs |= IsAWallThere(coord, direction::DOWN, maze) ? 0b10 : 0;
	dirs |= IsAWallThere(coord, direction::LEFT, maze) ? 0b1 : 0;
	return dirs;
}

uint8_t HowManyDirectionsFree(const uint8_t &n)
{
	uint8_t count = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if (n & 1 << i)
		{
			count++;
		}
	}
	return 4 - count;
}

uint8_t GetNextDir(const uint8_t &dir, const xy &curr, const xy &end, bool dir_preference = true)
{
	if (dir == 0b1111)
	{
		return dir;
	}

	if (HowManyDirectionsFree(dir) == 1)
	{
		return GetFirstFreeDir(dir);
	}

	// Choose preferred direction

	int32_t x_diff = end.x - curr.x;
	int32_t y_diff = end.y - curr.y;

	// Bits are set to 1 if that direction is visited or blocked
	// 0000 0000
	//	     ^^^^
	//      URDL
	// U -> Up
	// R -> Right
	// D -> Down
	// L -> Left
	if (dir_preference ? abs(x_diff) < abs(y_diff) : abs(x_diff) > abs(y_diff))
	{
		if (x_diff > 0)
		{
			if ((dir & 0b100) == 0) // Right is free
			{
				return 0b100; // Go right
			}
		}
		else
		{
			if ((dir & 0b1) == 0) // Left is free
			{
				return 0b1; // Go left
			}
		}
	}

	if (y_diff > 0)
	{
		if ((dir & 0b10) == 0) // Down is free
		{
			return 0b10; // Go down
		}
	}
	else
	{
		if ((dir & 0b1000) == 0) // Up is free
		{
			return 0b1000; // Go up
		}
	}

	return GetFirstFreeDir(dir);
}

void PrintMaze(const std::vector<std::string> &m)
{
	for (size_t i = 0; i < m.size(); i++)
	{
		// std::cout << i << "\t";
		for (size_t j = 0; j < m.at(i).size(); j++)
		{
			switch (m.at(i).at(j))
			{
			case WALL_CHAR:
				std::cout << "█";
				break;
			case PASSAGE_CHAR:
				std::cout << "░";
				break;
			case 'X':
				std::cout << red << "█" << reset;
				break;
			default:
				std::cout << red << m.at(i).at(j) << reset;
				break;
			}
		}
		std::cout << std::endl;
	}
	LOG("\n");
}

void PrintMazes(const std::vector<std::vector<std::string>> &m)
{
	for (size_t maze_num = 0; maze_num < m.size(); maze_num++)
	{
		LOG("Maze number " << maze_num << "\n");
		PrintMaze(m.at(maze_num));
	}
}

// Bits are set to 1 if that direction was visited
// 0000 0000
//	     ^^^^
//      URDL
// U -> Up
// R -> Right
// D -> Down
// L -> Left
std::string DirToString(const uint8_t &dir)
{
	switch (dir)
	{
	case 0b1:
		return "Left";
		break;
	case 0b10:
		return "Down";
		break;
	case 0b100:
		return "Right";
		break;
	case 0b1000:
		return "Up";
		break;
	case 0b1111:
		return "Going back";
		break;
	default:
		return "ERROR";
		break;
	}
}

xy FindChar(const std::vector<std::string> &m, const char c)
{
	for (size_t i = 0; i < m.size(); i++)
	{
		for (size_t j = 0; j < m.at(i).size(); j++)
		{
			if (m.at(i).at(j) == c)
			{
				return {(int32_t)j, (int32_t)i};
			}
		}
	}
	return {-1, -1};
}

std::vector<node> SolveMaze(std::vector<std::string> m, xy start, xy end, bool preferred_dir)
{
	std::vector<node> nodes;
	nodes.push_back({0, start});
	m.at(start.y).at(start.x) = VISITED_CHAR; // Set start as visited

	while (!(nodes.at(nodes.size() - 1).coords == end) && !(nodes.at(nodes.size() - 1).coords == start && CheckAvailableDirs(nodes.at(nodes.size() - 1).coords, m) == 0b1111))
	{
		// PrintMaze(m);

		uint8_t dirs_visited = CheckAvailableDirs(nodes.at(nodes.size() - 1).coords, m);
		// LOG("Current coords x: " << nodes.at(nodes.size() - 1).coords.x << " y:" << nodes.at(nodes.size() - 1).coords.y);
		// LOG("dirs_visited: " << std::bitset<4>(dirs_visited).to_string());

		uint8_t next_dir = GetNextDir(dirs_visited, nodes.at(nodes.size() - 1).coords, end, preferred_dir);

		m.at(nodes.at(nodes.size() - 1).coords.y).at(nodes.at(nodes.size() - 1).coords.x) = VISITED_CHAR; // Set current tile as visited

		if (next_dir == 0b1111)
		{
			// LOG("Going back a step")
			//  Go back a step
			nodes.pop_back();
			continue;
		}

		xy next_coords = nodes.at(nodes.size() - 1).coords;
		// Bits are set to 1 if that direction was visited
		// 0000 0000
		//	     ^^^^
		//      URDL
		// U -> Up
		// R -> Right
		// D -> Down
		// L -> Left
		switch (next_dir)
		{
		case 0b1: // Move Left
			next_coords.x--;
			break;
		case 0b10: // Move Down
			next_coords.y++;
			break;
		case 0b100: // Move Right
			next_coords.x++;
			break;
		case 0b1000: // Move Up
			next_coords.y--;
			break;
		}

		nodes.push_back({next_dir, next_coords});

		// LOG("Next dir: " << DirToString(next_dir))
		// LOG("Next coords: x: " << next_coords.x << " y: " << next_coords.y)
		// char aa;
		// std::cin >> aa;
	}
	return nodes;
}

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		LOGERR("Error! An input filename must be specified: ./maze_solver input.txt")
		return EXIT_FAILURE;
	}

	std::ifstream file(argv[1]);

	if (!file.is_open())
	{
		LOGERR("Error reading the file \"" << argv[1] << "\"!")
	}
	else
	{
		LOG_g("Parsing file " << argv[1])
	}

	std::string str;
	bool different_maze = false;
	std::vector<std::vector<std::string>> mazes;

	while (std::getline(file, str))
	{
		if (str.size() == 0)
		{
			different_maze = true;
			continue;
		}
		if (different_maze || mazes.size() == 0)
		{
			mazes.push_back(std::vector<std::string>());
			different_maze = false;
		}

		mazes.at(mazes.size() - 1).push_back(str);
	}

	if (file.bad())
	{
		LOGERR("Error while reading file");
	}

	for (size_t i = 0; i < mazes.size(); i++)
	{
		std::vector<std::string> m = mazes.at(i);


		LOG("Trying to solve maze " << i << ":");

		xy start, end;

		START_TIMER
		// Find start and end, store coordinates
		start = FindChar(m, START_CHAR);
		end = FindChar(m, END_CHAR);

		std::vector<node> sol1 = SolveMaze(m, start, end, true);
		std::vector<node> sol2 = SolveMaze(m, start, end, false);

		std::vector<node> nodes = sol1.size() <= sol2.size() ? sol1 : sol2;

		END_TIMER

		if (nodes.at(nodes.size() - 1).coords == end)
		{
			LOG_g("Solution found!");
			for (size_t i = 1; i < nodes.size(); i++)
			{
				std::cout << DirToString(nodes.at(i).dir_that_led_here);
				if (i == nodes.size() - 1)
				{
					std::cout << std::endl;
				}
				else
				{
					std::cout << ", ";
				}
			}
			m = mazes.at(i);
			xy next_coords = start;
			for (size_t i = 1; i < nodes.size() - 1; i++)
			{
				switch (nodes.at(i).dir_that_led_here)
				{
				case 0b1: // Move Left
					next_coords.x--;
					break;
				case 0b10: // Move Down
					next_coords.y++;
					break;
				case 0b100: // Move Right
					next_coords.x++;
					break;
				case 0b1000: // Move Up
					next_coords.y--;
					break;
				}
				m.at(next_coords.y).at(next_coords.x) = 'X';
			}
			PrintMaze(m);
		} else {
			LOG_r("No solution found!");
			PrintMaze(m);
		}

		nodes.clear();
	}

	return 0;
}