#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

void ReadDataFile(std::string &file, std::vector<std::vector<double>> &ts);
void AdjacencyToEdgeList(const std::vector<std::vector<double>> &ts, std::vector<std::vector<double>> &edgelist);