#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>
#include "multi_array_helper.hpp"

void
printGrid(boost::multi_array<char, 2> const &grid) {
    for (auto row: grid) {
        for (auto column: row) {
            fmt::print("{}", column);
        }
        fmt::print("\n");
    }
}

using range = boost::multi_array_types::index_range;

bool isAccessible(boost::multi_array<char, 2> &grid, unsigned int x, unsigned int y)
{
    int count = 0;
    // Check if at the edge of the grid
    if (x > 0)
    {
        count += grid[x-1][y] == '@' ? 1 : 0;
    }
    if (y > 0)
    {
        count += grid[x][y-1] == '@' ? 1 : 0;
    }
    if (x < grid.shape()[0]-1)
    {
        count += grid[x+1][y] == '@' ? 1 : 0;
    }
    if (y < grid.shape()[1]-1)
    {
        count += grid[x][y+1] == '@' ? 1 : 0;
    }
    if (x > 0 && y > 0)
    {
        count += grid[x-1][y-1] == '@' ? 1 : 0;
    }
    if (x < grid.shape()[0]-1 && y < grid.shape()[1]-1)
    {
        count += grid[x+1][y+1] == '@' ? 1 : 0;
    }
    if (x < grid.shape()[0]-1 && y > 0)
    {
        count += grid[x+1][y-1] == '@' ? 1 : 0;
    }
    if (x > 0 && y < grid.shape()[1]-1 )
    {
        count += grid[x-1][y+1] == '@' ? 1 : 0;
    }
    if (count < 4)
    {
        return true;
    }
    return false;
}

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

    boost::multi_array<char, 2> grid(boost::extents[lines.at(0).size()][lines.size()]);

    for (int i = 0; i < lines.size(); i++)
    {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    printGrid(grid);

    long accessible = 0;

    auto removeRolls = [](boost::multi_array<char, 2>& grid)
    {
        auto rollsRemoved = 0;
        for (auto itr = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '@');
           itr != multi_array_helper::end(grid);
           itr = std::find(++itr, multi_array_helper::end(grid), '@'))
        {
            auto indices = itr.get_indices();
            if (isAccessible(grid, indices.at(0), indices.at(1)))
            {
                grid[indices.at(0)][indices.at(1)] = 'x';
                rollsRemoved++;
            }
        }
        return rollsRemoved;
    };

    while (auto removed = removeRolls(grid))
    {
        accessible += removed;
    }

    fmt::print("Accessible: {}\n", accessible);
    return 0;
}
