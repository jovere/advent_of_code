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

std::optional<std::vector<std::tuple<int, int, int, int>>>
canMove(boost::multi_array<char, 2> const &grid, std::tuple<int, int> position, std::vector<std::tuple<int, int>> &prev,
        char direction) {
    std::vector<std::tuple<int, int, int, int>> vec;
    auto [row, col] = position;
    if (auto f = std::find(prev.begin(), prev.end(), position); f != prev.end()) {
        return vec;
    }
    prev.push_back(position);
    if (grid[row][col] == '#') { return {}; }
    if (grid[row][col] == '.') {
        return vec;
    }
    if (int rowOffset = (direction == '^') ? -1 : (direction == 'v') ? 1 : 0; rowOffset != 0) {
        auto v1 = canMove(grid, {row + rowOffset, col}, prev, direction);
        if (v1 && grid[row][col] == '[') {
            auto v2 = canMove(grid, {row, col + 1}, prev, direction);
            if (v2 && v1) {
                vec.insert(vec.end(), v1->begin(), v1->end());
                vec.insert(vec.end(), v2->begin(), v2->end());
                vec.emplace_back(row, col, (direction == '^') ? row - 1 : row + 1, col);
                return vec;
            }
        } else if (v1 && grid[row][col] == ']') {
            auto v2 = canMove(grid, {row, col - 1}, prev, direction);
            if (v2 && v1) {
                vec.insert(vec.end(), v1->begin(), v1->end());
                vec.insert(vec.end(), v2->begin(), v2->end());
                vec.emplace_back(row, col, (direction == '^') ? row - 1 : row + 1, col);
                return vec;
            }
        }
    }
    return {};
}

std::optional<std::tuple<int, int>>
moveRobot(boost::multi_array<char, 2> &grid, std::tuple<int, int> position, char direction, char previous = ' ',
          bool moving = false) {
    auto [row, col] = position;
    if (grid[row][col] == '#') { return {}; }
    if (grid[row][col] == '.') { return position; }
    switch (direction) {
        case '^':
            if ((grid[row][col] == '[' || grid[row][col] == ']')) {
                std::vector<std::tuple<int, int>> prev;
                if (auto result = canMove(grid, {row, col}, prev, direction)) {
                    //std::ranges::sort(result.value());
                    //std::ranges::unique(result.value());
                    for (auto [r1, c1, r2, c2]: result.value()) {
                        std::swap(grid[r1][c1], grid[r2][c2]);
                    }
                    return std::tuple{row - 1, col};
                }
            } else if (grid[row][col] == '@') {
                if (moveRobot(grid, {row - 1, col}, direction)) {
                    std::swap(grid[row][col], grid[row - 1][col]);
                    return std::tuple{row - 1, col};
                }
            }
            break;
        case 'v':
            if ((grid[row][col] == '[' || grid[row][col] == ']')) {
                std::vector<std::tuple<int, int>> prev;
                if (auto result = canMove(grid, {row, col}, prev, direction)) {
                    //std::ranges::sort(result.value());
                    //std::ranges::reverse(result.value());
                    //std::ranges::unique(result.value());
                    for (auto [r1, c1, r2, c2]: result.value()) {
                        std::swap(grid[r1][c1], grid[r2][c2]);
                    }
                    return std::tuple{row + 1, col};
                }
            } else if (grid[row][col] == '@') {
                if (moveRobot(grid, {row + 1, col}, direction)) {
                    std::swap(grid[row][col], grid[row + 1][col]);
                    return std::tuple{row + 1, col};
                }
            }
            break;
        case '>':
            if (grid[row][col] == '[' || grid[row][col] == ']' || grid[row][col] == '@') {
                if (moveRobot(grid, {row, col + 1}, direction)) {
                    std::swap(grid[row][col], grid[row][col + 1]);
                    return std::tuple{row, col + 1};
                }
            }
            break;
        case '<':
            if (grid[row][col] == '[' || grid[row][col] == ']' || grid[row][col] == '@') {
                if (moveRobot(grid, {row, col - 1}, direction)) {
                    std::swap(grid[row][col], grid[row][col - 1]);
                    return std::tuple{row, col - 1};
                }
            }
            break;
        default:
            abort();
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

    fmt::print("Width: {}\n", lines.at(0).size() * 2);
    fmt::print("Height: {}\n", lines.size());

    using range = boost::multi_array_types::index_range;
    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size() * 2]};

    for (int i = 0; i < lines.size(); ++i) {
        std::vector<char> doubleWide;
        for (auto object: lines[i]) {
            switch (object) {
                case 'O':
                    doubleWide.push_back('[');
                    doubleWide.push_back(']');
                    break;
                case '@':
                    doubleWide.push_back('@');
                    doubleWide.push_back('.');
                    break;
                default:
                    doubleWide.push_back(object);
                    doubleWide.push_back(object);
            }
        }
        std::ranges::copy(doubleWide, grid[i].begin());
    }

    printGrid(grid);
    fmt::print("{}\n", moves);

    // Find the starting position
    auto start = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '@');

    std::tuple<int, int> position{start.get_indices()[0], start.get_indices()[1]};
    for (auto move: moves) {
        if (auto newPosition = moveRobot(grid, position, move)) {
            position = newPosition.value();
        }
    }
    printGrid(grid);

    long gpsSum = 0;
    for (auto itr = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '[');
         itr != multi_array_helper::end(grid);
         itr = std::find(++itr, multi_array_helper::end(grid), '[')) {
        gpsSum += itr.get_indices()[0]*100 + itr.get_indices()[1];
    }
    fmt::print("GPS Sum: {}\n", gpsSum);

    return 0;
}
