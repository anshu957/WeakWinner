#pragma once
#include <vector>
#include <string>

// All analysis functions/features related to network aspect of the problem will go here

typedef double real_t;

class Network
{
private:
  std::string m_fn;

  void ReadNetworkFromFile();

public:
  std::vector<std::vector<size_t>> m_edgeList;
  std::vector<std::vector<bool>> m_AdjMat;
  size_t m_networkSize;

  // member functions
  Network(std::string fn) : m_fn(fn)
  {
    ReadNetworkFromFile();
  };
  void PrintEdgeList();
  void PrintAdjacencyMatrix();

  //TODO: All network related methods to be implemented here
};
