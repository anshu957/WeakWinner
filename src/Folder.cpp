#ifdef _WIN64
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;
#elif __linux
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include <iostream>
#include <algorithm>
#include <fstream>
#include <Folder.h>
#include <ctime>

void Folder::create_folder()
{
	auto dirname = fs::current_path();
	dirname /= m_FolderName;

	time_t now = time(0);
	tm *ltm = localtime(&now);
	char buffer[60];

	std::strftime(buffer, 60, "%Y-%m-%d", ltm);

	//std::string d = __DATE__;
	std::string d = buffer;
	d.erase(std::remove(d.begin(), d.end(), ' '), d.end());

	dirname /= d;
	if (!fs::create_directories(dirname))
	{
		std::cout << " Directory already exists !" << std::endl;
	}

	m_path = dirname.string() + "/";
}

/*
int main()
{
	std::vector<std::vector<bool>> tmp;
	ReadFromFile("Adj.dat", tmp);

	for (auto i : tmp)
	{
		for (auto j : i)
			std::cout << j << "\t";
		std::cout << "\n";
	}
	//	Folder f("test");
	//
	//
	//	// sends file to the directory which has path : path_where_executable_lives/foldername/currentdate/
	//	std::string fn = f.m_path + "sample_output.txt";
	//
	//	std::cout << " path for file : " << fn << std::endl;
	//
	//
	//	std::ofstream out_1(fn, std::ofstream::out);
	//
	//	if (out_1.is_open())
	//	{
	//		out_1 << " Test message written successfully !" << std::endl;
	//	}
	//
	//	else
	//		std::cout << "\n Error opening the file !!" << std::endl;
	//
	//	out_1.close();
	//
	std::cin.get();
}
*/