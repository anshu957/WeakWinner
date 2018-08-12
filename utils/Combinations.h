#pragma once
#include <vector>
#include <algorithm>
#include <assert.h>

template <typename T>
void Combination(const std::vector<T> &v, std::size_t count, std::vector<std::vector<T>> &v2)
{
    assert(count <= v.size());
    std::vector<bool> bitset(v.size() - count, 0);
    bitset.resize(v.size(), 1);

    do
    {
        std::vector<T> tmp;
        for (std::size_t i = 0; i != v.size(); ++i)
        {
            if (bitset[i])
            {
                tmp.push_back(v[i]);
            }
        }
        v2.push_back(tmp);
    } while (std::next_permutation(bitset.begin(), bitset.end()));
}

/*
int main(int argc, char const *argv[])
{
    std::vector<int> v1{1, 2, 3, 4};
    std::vector<std::vector<int>> v2;

    Combination(v1, 2, v2);

    for (auto i : v2)
    {
        for (auto j : i)
            std::cout << j << "\t";
        std::cout << std::endl;
    }

    return 0;
}
*/