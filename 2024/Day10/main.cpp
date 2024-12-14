#include <vector>
#include <fstream>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/adapted/std_array.hpp>
#include <boost/algorithm/string.hpp>
#include "multi_array_helper.hpp"

BOOST_GEOMETRY_REGISTER_STD_ARRAY_CS(cs::cartesian)

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

std::set<std::tuple<std::tuple<long,long>,std::tuple<long, long>>>
pathsFound(boost::multi_array<char, 2> const &totalGrid, boost::multi_array<char, 2> const &searchGrid,
           std::tuple<long, long> offset, std::tuple<long, long> base, char find) {
    std::set<std::tuple<std::tuple<long,long>,std::tuple<long, long>>> pathCount;
    fmt::print("Searching:\n");
    for (auto itr = std::find(multi_array_helper::begin(searchGrid), multi_array_helper::end(searchGrid), find);
         itr != multi_array_helper::end(searchGrid); itr = std::find(++itr, multi_array_helper::end(searchGrid),
                                                                     find)) {
        auto [rowOff,colOff] = offset;
        auto row = itr.get_indices()[0] + rowOff;
        auto col = itr.get_indices()[1] + colOff;

        if (find == '9') {
            pathCount.emplace(base, std::tuple{row,col});
            fmt::print("Found: '9' at ({},{}) Path Count: {}\n", row, col, pathCount);
        } else {
            fmt::print("Found: '{}' at ({},{})\n", find, row, col);

            range rowRange{row > 0 ? row - 1 : row, row < totalGrid.shape()[0] - 1 ? row + 2 : row + 1};
            range colRange{col > 0 ? col - 1 : col, col < totalGrid.shape()[1] - 1 ? col + 2 : col + 1};
            pathCount.merge(pathsFound(totalGrid, totalGrid[boost::indices[rowRange][colRange]],
                                       {rowRange.start(), colRange.start()},
                                       (base != std::tuple{-1, -1}) ? base : std::tuple{row, col},
                                       static_cast<char>(find + 1)));
        }
    }
    return pathCount;
}

int
main() {

    std::vector<std::string> lines;
    {
        std::fstream input("input_test.txt", std::ios::in);
        if (!input.is_open()) {
            fmt::print(stderr, "Can't open file.");
            return 1;
        }

        std::string line;
        while (std::getline(input, line)) {
            lines.push_back(line);
        }
    }

    fmt::print("Width: {}\n", lines.at(0).size());
    fmt::print("Height: {}\n", lines.size());

    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size()]};

    for (int i = 0; i < lines.size(); ++i) {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    printGrid(grid);

    auto paths = pathsFound(grid, grid, {0,0}, {-1,-1}, '0');


    for(auto path : paths)
    {
        fmt::print("({})\n", path);
    }
    fmt::print("Paths Found: {}", paths.size());

    return 0;
}