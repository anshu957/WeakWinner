#pragma once
#include <string>
#include <fstream>
#include <vector>

// use to create a folder in the current directory with a specific name and inside that folder create folders(named by date)
// which are created on each different compilation date.
class Folder
{
private:
  std::string m_FolderName;
  void create_folder();

public:
  std::string m_path;

  Folder(std::string fn) : m_FolderName(fn)
  {
    create_folder();
  };
};

// TODO: check-out extern keyword for moving template function definition outside header files
template <typename T>
void ReadFromFile(const std::string fn, std::vector<std::vector<T>> &p_vector)
{
  std::fstream in_1(fn.c_str(), std::fstream::in);
  if (in_1.good())
  {
    std::string line;
    while (getline(in_1, line))
    {
      std::vector<T> rowVector;
      std::istringstream row(line);
      std::string value;
      while (getline(row, value, ','))
        rowVector.push_back(stod(value));

      p_vector.push_back(rowVector);
    }
  }
  in_1.close();
}
