#include <vector>
#include <fstream>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/adapted/std_array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/multi_array.hpp>

void
printGrid(boost::multi_array<char, 2> const &grid) {
    for (auto row: grid) {
        for (auto column: row) {
            fmt::print("{}", column);
        }
        fmt::print("\n");
    }
}

enum Direction {
    Up,
    Left,
    Down,
    Right,
    Unknown,
};

struct Location {

    Location(long r, long c, char crop, Direction d) :
            row(r), col(c), crop(crop), direction(d) {}

    void sumAndReset() {
        perimeterTotal += std::accumulate(fenceSide.begin(), fenceSide.end(), 0L);
        fenceSide = {};
    }

    long row;
    long col;
    Direction direction{Unknown};
    std::array<bool, 4> fenceSide{};
    char crop;
    long perimeterTotal = 0;
    long areaTotal = 0;
};

using range = boost::multi_array_types::index_range;

[[nodiscard]] Location
calcRegion(boost::multi_array<char, 2> &field, Location start) {
    ++start.areaTotal;
    std::vector<Location> indices;

    // Change to lower to avoid recounting
    field[start.row][start.col] = static_cast<char>(std::tolower(start.crop));

    if (start.row > 0) {
        auto l = start;
        --l.row;
        l.direction = Direction::Up;
        indices.emplace_back(l);
    }
    else
    {
        start.fenceSide[Direction::Up] = true;
    }
    if (start.row < field.shape()[0] - 1) {
        auto l = start;
        ++l.row;
        l.direction = Direction::Down;
        indices.emplace_back(l);
    }
    else
    {
        start.fenceSide[Direction::Down] = true;
    }
    if (start.col > 0) {
        auto l = start;
        --l.col;
        l.direction = Direction::Left;
        indices.emplace_back(l);
    }
    else
    {
        start.fenceSide[Direction::Left] = true;
    }
    if (start.col < field.shape()[1] - 1) {
        auto l = start;
        ++l.col;
        l.direction = Direction::Right;
        indices.emplace_back(l);
    }
    else
    {
        start.fenceSide[Direction::Right] = true;
    }


    for (auto l: indices) {
        if (field[l.row][l.col] == l.crop) {
            auto prevLocation = calcRegion(field, l);
            if(start.direction != prevLocation.direction)
            {
                prevLocation.sumAndReset();
            }
            else
            {
                prevLocation.perimeterTotal += prevLocation.fenceSide[prevLocation.direction];
                prevLocation.fenceSide[prevLocation.direction] = false;
            }
            start = prevLocation;
        } else if (field[l.row][l.col] != tolower(l.crop)) {
            start.fenceSide[l.direction] = true;
        }
    }
    return start;
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

    long totalCost = 0;
    for (auto row: grid | boost::adaptors::indexed()) {
        auto rIdx = row.index();
        for (auto col: row.value() | boost::adaptors::indexed()) {
            auto cIdx = col.index();
            auto value = col.value();

            if (std::isupper(value)) {
                Location l{rIdx, cIdx, value, Direction::Right};
                auto location = calcRegion(grid, l);
                location.sumAndReset();
                auto cost = location.areaTotal * location.perimeterTotal;
                fmt::print("Region {}: Area: {} Perimeter: {} Cost: {}\n", value, location.areaTotal, location.perimeterTotal, cost);
                totalCost += cost;
            }
        }
    }

    fmt::print("Total Cost: {}", totalCost);

    return 0;
}