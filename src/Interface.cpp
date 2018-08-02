#pragma once
#include <Interface.h>

void ReadDataFile(std::string &file, std::vector<std::vector<double>> &ts)
{
    // Reading data from the file...
    // Knowledge of rows or columns is not required...Just the name of local file is required
    std::fstream in_1(file.c_str(), std::fstream::in);
    int count = 0;
    if (in_1.good())
    {
        std::string line;
        while (getline(in_1, line))
        {
            std::vector<double> rowVector;
            std::istringstream row(line);
            std::string value;
            while (getline(row, value, ','))
                rowVector.push_back(stod(value));

            ts.push_back(rowVector);
        }
        count++;
    }
    in_1.close();
}

void AdjacencyToEdgeList(const std::vector<std::vector<double>> &ts, std::vector<std::vector<double>> &edgelist)
{
    for (size_t i = 0; i < ts.size(); i++)
    {
        std::vector<double> row;
        row.push_back(i);
        for (size_t j = 0; j < ts[0].size(); j++)
        {
            if (ts[i][j])
                row.push_back(j);
        }
        edgelist.push_back(row);
    }
}

/*

int main(int argc, char *argv[])
{
    vector<vector<double>> ts;
    string fn = "Adj.dat";
    ReadDataFile(fn, ts);
    cout << "\n Shape of Adj matrix is : ( " << ts.size() << ", " << ts[0].size() << " )" << endl;
    cout << endl;
    for (auto i : ts)
    {
        for (auto j : i)
            cout << j << "\t";
        cout << endl;
    }

    vector<vector<double>> edgelist;
    AdjacencyToEdgeList(ts, edgelist);
    cout << endl
         << endl;
    cout << "Edge List : " << endl;
    for (auto i : edgelist)
    {
        for (auto j : i)
            cout << j << "\t";
        cout << endl;
    }

    return 0;
}
*/