#include <stdio.h>
#include <cstdlib>
#include "Filemanager.h"

std::vector<std::vector<int>> v;

int main()
{
	srand((unsigned)time(NULL));

	v = get_random_data(10);

	// Print input data
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v[i].size(); j++)
			std::cout << v[i][j] << " ";

		std::cout << "\n";
	}

	return 0;
}
