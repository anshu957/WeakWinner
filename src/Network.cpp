#include <Network.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Folder.h>

void Network::ReadNetworkFromFile()
{
    ReadFromFile(m_fn, m_edgeList);

    m_networkSize = m_edgeList.size();

    m_AdjMat.resize(m_networkSize, std::vector<bool>(m_networkSize));

    size_t idx_tmp1;
    size_t idx_tmp2;
    for (size_t i = 0; i < m_edgeList.size(); i++)
    {
        idx_tmp1 = m_edgeList[i][0];
        for (size_t j = 1; j < m_edgeList[i].size(); j++)
        {
            idx_tmp2 = m_edgeList[i][j];
            m_AdjMat[idx_tmp1][idx_tmp2] = true;
        }
    }
}

void Network::PrintEdgeList()
{
    for (auto i : m_edgeList)
    {
        for (auto j : i)
            std::cout << j << "\t";
        std::cout << std::endl;
    }
}

void Network::PrintAdjacencyMatrix()
{
    for (size_t i = 0; i < m_AdjMat.size(); i++)
    {
        for (size_t j = 0; j < m_AdjMat[i].size(); j++)
            std::cout << m_AdjMat[i][j] << "\t";
        std::cout << std::endl;
    }
}

/*
int main(int argc, char *argv[])
{
    Network net1("Adj.dat");

    net1.PrintEdgeList();
    std::cout << "\n\n";
    std::cout << "\n Adjacency Matrix : \n";
    net1.PrintAdjacencyMatrix();
    return 0;
}
/*
void Network::ReadNetworkFromFile()
{
    std::fstream in_1(m_fn.c_str(), std::fstream::in);

    // First read the whole file(CSV) and store it in a temporary adjacency matrix
    // Reason for doing so is pure laziness...who wants to code a parser anyway
    std::vector<std::vector<bool>> tmp_AdjMat;
    size_t count = 0;
    if (in_1.good())
    {
        std::string line;
        while (getline(in_1, line))
        {
            std::vector<bool> rowVector;
            std::istringstream row(line);
            std::string value;
            while (getline(row, value, ','))
                rowVector.push_back(stod(value));

            tmp_AdjMat.push_back(rowVector);
        }
        count++;
    }
    in_1.close();

    // Code below converts the adjacency matrix into edgelist and store it as a member variable

    m_networkSize = tmp_AdjMat.size();
    for (size_t i = 0; i < m_networkSize; i++)
    {
        std::vector<size_t> row;
        row.push_back(i);
        for (size_t j = 0; j < m_networkSize; j++)
        {
            if (tmp_AdjMat[i][j])
                row.push_back(j);
        }
        m_edgeList.push_back(row);
    }
}
*/