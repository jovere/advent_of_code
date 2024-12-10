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

    using range = boost::multi_array_types::index_range;
    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size()]};

    for (int i = 0; i < lines.size(); ++i) {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    printGrid(grid);

    // Create a hash table of antennas for each "frequency"
    std::unordered_map<char, std::vector<std::array<int, 2>>> frequencies;
    for (auto item =
            std::find_if(multi_array_helper::begin(grid), multi_array_helper::end(grid), !boost::is_any_of("."));
         item != multi_array_helper::end(grid);
         item = std::find_if(++item, multi_array_helper::end(grid), !boost::is_any_of("."))) {
        frequencies[*item].push_back(
                {static_cast<int>(item.get_indices()[0]), static_cast<int>(item.get_indices()[1])});
    }

    // Lambda for checking if coordinates are outside grid
    auto outsideGrid = [&grid = std::as_const(grid)](auto const &node) {
        auto &[row, col] = node;
        return (row < 0 || row >= grid.shape()[0] || col < 0 || col >= grid.shape()[0]);
    };

    // Find the antiNodes
    std::set<std::array<int, 2>> antiNodes;
    for (auto &frequency: frequencies) {
        fmt::print("frequency: {}\n", frequency);
        for (auto antennaA = frequency.second.begin(); antennaA != frequency.second.end(); ++antennaA) {
            for (auto antennaB = antennaA + 1; antennaB != frequency.second.end(); ++antennaB) {
                auto antiNodeA = *antennaA;
                auto antiNodeB = *antennaB;
                auto diff = antiNodeB;
                // Calculate the difference
                boost::geometry::subtract_point(diff, antiNodeA);

                // Insert all nodes from antennaA and away
                if (boost::geometry::subtract_point(antiNodeA, diff);
                        !outsideGrid(antiNodeA)) {
                    antiNodes.insert(antiNodeA);
                }

                // Insert all nodes from antennaB and away
                if (boost::geometry::add_point(antiNodeB, diff);
                        !outsideGrid(antiNodeB)) {
                    antiNodes.insert(antiNodeB);
                }
            }
        }
        fmt::print("\n");
    }

    // Visualization
    {
        auto resultGrid = grid;
        for (auto [row, col]: antiNodes) {
            if (row >= 0 && row < resultGrid.shape()[0] && col >= 0 && col < resultGrid.shape()[0]) {
                resultGrid[row][col] = '#';
            }
        }
        printGrid(resultGrid);
    }

    // Final answer
    fmt::print("Count: {} AntiNodes: {}", antiNodes.size(), antiNodes);

    return 0;
}
