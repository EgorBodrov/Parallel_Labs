#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>

void save_to_file(std::vector<std::vector<int>> v, const char* filename);
std::vector<std::vector<int>> get_random_data(int N);

#endif
