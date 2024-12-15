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

using range = boost::multi_array_types::index_range;

[[nodiscard]] std::tuple<long, long>
calcRegion(boost::multi_array<char, 2> &field, std::tuple<long, long> start, char crop) {
    long perimeter = 0;
    long area = 1;
    std::vector<std::tuple<long, long>> indices;
    auto [cRow, cCol] = start;

    // Change to lower to avoid recounting
    field[cRow][cCol] = static_cast<char>(std::tolower(crop));

    if (cRow > 0) {
        indices.emplace_back(cRow - 1, cCol);
    }
    else
    {
        ++perimeter;
    }
    if (cRow < field.shape()[0] - 1) {
        indices.emplace_back(cRow + 1, cCol);
    }
    else
    {
        ++perimeter;
    }
    if (cCol > 0) {
        indices.emplace_back(cRow, cCol - 1);
    }
    else
    {
        ++perimeter;
    }
    if (cCol < field.shape()[1] - 1) {
        indices.emplace_back(cRow, cCol + 1);
    }
    else
    {
        ++perimeter;
    }

    for (auto [row, col]: indices) {
        if (field[row][col] == crop) {
            auto [a,p] = calcRegion(field, {row,col}, crop);
            area += a;
            perimeter += p;
        }
        else if (field[row][col] != tolower(crop))
        {
            ++perimeter;
        }
    }
    return {area, perimeter};
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

    long totalCost = 0;
    for(auto row : grid | boost::adaptors::indexed())
    {
        auto rIdx = row.index();
        for(auto col : row.value() | boost::adaptors::indexed())
        {
            auto cIdx = col.index();
            auto value = col.value();

            if(std::isupper(value))
            {
                auto [a, p] = calcRegion(grid, {rIdx,cIdx}, value);
                auto cost = a * p;
                fmt::print("Region {}: Area: {} Perimeter: {} Cost: {}\n", value, a, p, cost);
                totalCost += cost;
            }
        }
    }

    fmt::print("Total Cost: {}", totalCost);

    return 0;
}