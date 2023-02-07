#include "Filemanager.h"


void save_to_file(std::vector<std::vector<int>> v, const char* filename)
{
	std::ofstream outfile(filename);
	
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v[i].size(); j++)
			outfile << v[i][j] << " ";
		
		outfile << "\n";
	}

	outfile.close();
}

std::vector<std::vector<int>> get_random_data(int N)
{
	std::vector<std::vector<int>> points(N, std::vector<int>());
	
	for (int i = 0; i < N; i++)
	{
		// Mass
		points[i].push_back(rand() % 100 + 1);

		// Radius vector
		points[i].push_back(rand() % 10 + 1);
		points[i].push_back(rand() % 10 + 1);

		// Velocity vector
		points[i].push_back(rand() % 5 + 1);
		points[i].push_back(rand() % 5 + 1);
	}
	
	save_to_file(points, "input_data.txt");

	return points;
}
