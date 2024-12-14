#include <fstream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>

int
main() {
    std::fstream input("input.txt", std::ios::in);
    if (!input.is_open()) {
        fmt::print(stderr, "Can't open file.");
        return 1;
    }

    std::string stone;

    // Get all the data
    std::vector<std::string> stones;
    for (int index = 0; input >> stone; ++index) {
        stones.push_back(stone);
    }
    fmt::print("Stone Count: {}\nStones: {}\n", stones.size(), stones);

    for (int iteration = 0; iteration < 25; ++iteration) {
        std::vector<std::string> newStones;
        for (auto &stone: stones) {
            if (stone == "0") {
                newStones.emplace_back("1");
            } else if (auto size = stone.size(); size % 2 == 0) {
                auto half = size / 2;
                newStones.push_back(stone.substr(0, half));
                newStones.push_back(std::to_string(std::stol(stone.substr(half))));
            } else {
                newStones.push_back(std::to_string(std::stol(stone) * 2024));
            }
        }
        stones = std::move(newStones);
        fmt::print("Iteration: {}\nStone Count: {}\n", iteration, stones.size());
    }
    return 0;
}
