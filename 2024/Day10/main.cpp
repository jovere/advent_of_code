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

std::set<std::tuple<std::tuple<long, long>, std::tuple<long, long>>>
pathsFound(boost::multi_array<char, 2> const &totalGrid, std::tuple<long, long> searchCenter,
           std::tuple<long, long> base, char find) {
    std::set<std::tuple<std::tuple<long, long>, std::tuple<long, long>>> pathCount;
    fmt::print("Searching:\n");
    std::vector<std::tuple<long, long>> indices;
    auto [cRow, cCol] = searchCenter;

    if (cRow > 0) {
        indices.emplace_back(cRow - 1, cCol);
    }
    if (cRow < totalGrid.shape()[0] - 1) {
        indices.emplace_back(cRow + 1, cCol);
    }
    if (cCol > 0) {
        indices.emplace_back(cRow, cCol - 1);
    }
    if (cCol < totalGrid.shape()[1] - 1) {
        indices.emplace_back(cRow, cCol + 1);
    }

    for (auto [row, col]: indices) {
        if (totalGrid[row][col] == find) {
            if (find == '9') {
                pathCount.emplace(base, std::tuple{row, col});
                fmt::print("Found: '9' at ({},{}) Path Count: {}\n", row, col, pathCount);
            } else {
                fmt::print("Found: '{}' at ({},{})\n", find, row, col);
                pathCount.merge(pathsFound(totalGrid, {row, col}, base, static_cast<char>(find + 1)));
            }
        }
    }
    return pathCount;
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

    std::set<std::tuple<std::tuple<long, long>, std::tuple<long, long>>> paths;
    for (auto itr = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '0');
         itr != multi_array_helper::end(grid);
         itr = std::find(++itr, multi_array_helper::end(grid), '0')) {
        std::tuple<long,long> base{itr.get_indices()[0],itr.get_indices()[1]};
        paths.merge(pathsFound(grid, base, base, '1'));
    }


    for (auto path: paths) {
        fmt::print("({})\n", path);
    }
    fmt::print("Paths Found: {}", paths.size());

    return 0;
}