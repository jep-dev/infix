#include "load.hpp"
#include <fstream>

int load(const char *filename, std::vector<std::string> &buffer)
{
	int lines = 0;
	std::ifstream file;
	file.open(filename);
	if(!file.is_open()) {
		return -1;
	}
	std::string line;
	while(std::getline(file, line)) {
		buffer.emplace_back(line);
		lines ++;
	}
	file.close();
	return lines;
}
