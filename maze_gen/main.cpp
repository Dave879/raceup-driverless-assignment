#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../common/utils.h"

int main(int argc, char *argv[])
{

	srand(time(0));
	if (argc <= 3)
	{
		LOGERR("Error! A width, height and output file name must be specified: ./maze_gen 6 9 output_gen.txt      # Ex: Width = 6, Height = 9")
		return EXIT_FAILURE;
	}

	std::stringstream stream;

	int width = atoi(argv[2]), height = atoi(argv[1]);

	std::ofstream file;

	file.open(argv[3]);

	file << "";

	file.close();

	int num_graphs = 1;
	if (argc == 5)
	{
		num_graphs = atoi(argv[4]);
		std::cout << "Generating " << num_graphs << " mazes... ";
	}

	for (size_t g = 0; g < num_graphs; g++)
	{
		for (size_t i = 0; i < width; i++)
		{
			for (size_t j = 0; j < height; j++)
			{
				if ((rand() % 100) > 70)
				{
					stream << "#";
				}
				else
				{
					stream << ".";
				}
			}
			stream << "\n";
		}
		stream << std::endl;
		std::string s = stream.str();

		char arr[2] = {'S', 'E'};
		for (size_t i = 0; i < 2; i++)
		{
			int placement = rand() % s.size();
			if (s.at(placement) == '\n' || s.at(placement) == 'S')
			{
				i--;
				continue;
			}
			s[placement] = arr[i];
		}

		stream.str(std::string());
		std::ofstream file;

		file.open(argv[3], std::ios_base::app);

		file << s;

		file.close();

	}
	std:: cout << "Done!" << std::endl;

	return 0;
}