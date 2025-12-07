#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>
#include "multi_array_helper.hpp"

void
printGrid(boost::multi_array<std::pair<char,long long>, 2> const &grid) {
    for (auto row: grid) {
        for (auto column: row) {
            if (column.first != '|')
            {
                fmt::print("{}", column.first);
            }
            else
            {
                fmt::print("{}", column.second);
            }
        }
        fmt::print("\n");
    }
}

using range = boost::multi_array_types::index_range;

int
main() {

    std::vector<std::string> lines;
    {
        std::fstream input("input.txt", std::ios::in);
        if (!input.is_open()) {
            fmt::print(stderr, "Can't open file.");
            return 1;
        }

        std::string line;
        while (std::getline(input, line)) {
            if (line.empty()) {
                break;
            }
            lines.push_back(line);
        }
    }

    fmt::print("Width: {}\n", lines.at(0).size());
    fmt::print("Height: {}\n", lines.size());

    boost::multi_array<std::pair<char,long long>, 2> grid(boost::extents[lines.size()][lines.at(0).size()]);

    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            grid[i][j].first = lines[i][j];
        }
    }

    printGrid(grid);

    long splits = 0;
    for (auto itr = std::find_if(multi_array_helper::begin(grid), multi_array_helper::end(grid),
                                 [](std::pair<char,long long> i) { return i.first == 'S' || i.first == '|'; });
         itr != multi_array_helper::end(grid);
         itr = std::find_if(++itr, multi_array_helper::end(grid), [](std::pair<char,long long> i) { return i.first == 'S' || i.first == '|'; }))
    {
        auto indices = itr.get_indices();
        if ((*itr).first == 'S')
        {
            grid[indices.at(0)][indices.at(1)].second = 1;
        }
        auto row = indices.at(0)+1;
        if (row < grid.shape()[0])
        {
            auto column = indices.at(1);
            if (grid[row][column].first == '^')
            {
                ++splits;
                grid[row][column-1] = {'|', grid[row][column-1].second + (*itr).second};
                grid[row][column+1] = {'|', grid[row][column+1].second + (*itr).second};
            }
            else
            {
                grid[row][column] = {'|', grid[row][column].second + (*itr).second};
            }
        }
    }

    printGrid(grid);
    fmt::print("Splits: {}\n", splits);

    long long timelines = 0;
    for (auto column : grid[grid.shape()[0]-1])
    {
        timelines += column.second;
    }

    fmt::print("Timelines: {}\n", timelines);
    return 0;
}
