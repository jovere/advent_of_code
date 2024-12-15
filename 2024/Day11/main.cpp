#include <fstream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>

std::map<std::tuple<long,long>, long> lookupTable;

long stoneCount(long stone, int depth) {
    if (depth == 75) {
        return 1;
    } else if (auto itr = lookupTable.find({stone,depth}); itr != lookupTable.end()) {
        return itr->second;
    } else {
        long out;
        if (stone == 0) {
            out = stoneCount(1, depth + 1);
        } else if (auto str = std::to_string(stone); str.size() % 2 == 0) {
            auto half = str.size() / 2;
            auto first = std::stol(str.substr(0, half));
            auto second = std::stol(str.substr(half));
            out = stoneCount(first, depth + 1) + stoneCount(second, depth + 1);
        } else {
            out = stoneCount((stone) * 2024, depth + 1);
        }
        lookupTable[{stone,depth}] = out;
        return out;
    }
}

int
main() {
    std::fstream input("input.txt", std::ios::in);
    if (!input.is_open()) {
        fmt::print(stderr, "Can't open file.");
        return 1;
    }

    std::vector<long> stones;
    {    // Get all the data
        long stone;
        for (int index = 0; input >> stone; ++index) {
            stones.push_back(stone);
        }
        fmt::print("Stone Count: {}\nStones: {}\n", stones.size(), stones);
    }

    long count = 0;
    for (auto const &stone: stones) {
        fmt::print("Start Stone: {}", stone);
        count += stoneCount(stone, 0);
        fmt::print(" Done -- Count: {}\n", count);
    }
    for(auto entry:lookupTable)
    {
        fmt::print("{}\n", entry);
    }
    fmt::print("Lookup Table Size: {}\n", lookupTable.size());
    fmt::print("Stone Count: {}\n", count);

    return 0;
}
