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

std::optional<std::tuple<int, int>>
moveRobot(boost::multi_array<char, 2> &grid, std::tuple<int, int> position, char direction) {
    auto [row, col] = position;
    if (grid[row][col] == '#') { return {}; }
    if (grid[row][col] == '.') { return position; }
    if (grid[row][col] == 'O' || grid[row][col] == '@') {
        switch (direction) {
            case '^':
                if (moveRobot(grid, {row - 1, col}, direction)) {
                    std::swap(grid[row][col], grid[row - 1][col]);
                    return std::tuple{row - 1, col};
                }
                break;
            case 'v':
                if (moveRobot(grid, {row + 1, col}, direction)) {
                    std::swap(grid[row][col], grid[row + 1][col]);
                    return std::tuple{row + 1, col};
                }
                break;
            case '>':
                if (moveRobot(grid, {row, col + 1}, direction)) {
                    std::swap(grid[row][col], grid[row][col + 1]);
                    return std::tuple{row, col + 1};
                }
                break;
            case '<':
                if (moveRobot(grid, {row, col - 1}, direction)) {
                    std::swap(grid[row][col], grid[row][col - 1]);
                    return std::tuple{row, col - 1};
                }
                break;
            default:
                abort();
        }
    }
    return {};
}

int
main() {

    std::vector<std::string> lines;
    std::vector<char> moves;
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

        char move;
        while (input >> move) {
            moves.push_back(move);
        }
    }

    fmt::print("Width: {}\n", lines.at(0).size());
    fmt::print("Height: {}\n", lines.size());

    using range = boost::multi_array_types::index_range;
    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size()]};

    for (int i = 0; i < lines.size(); ++i) {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    printGrid(grid);
    fmt::print("{}\n", moves);

    // Find the starting position
    auto start = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '@');

    std::tuple<int,int> position{start.get_indices()[0],start.get_indices()[1]};
    for (auto move: moves) {
        if(auto newPosition = moveRobot(grid,position,move))
        {
            position = newPosition.value();
        }
    }
    printGrid(grid);

    long gpsSum = 0;
    for (auto itr = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), 'O');
         itr != multi_array_helper::end(grid);
         itr = std::find(++itr, multi_array_helper::end(grid), 'O')) {
        gpsSum += itr.get_indices()[0]*100 + itr.get_indices()[1];
    }
    fmt::print("GPS Sum: {}\n", gpsSum);
    return 0;
}
