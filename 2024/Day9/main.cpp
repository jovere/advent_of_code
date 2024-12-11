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

    char cLength;

    // Get all the data
    std::vector<int> blocks;
    for (int index = 0; input >> cLength; ++index) {
        auto length = static_cast<int>(cLength - '0');
        if(index % 2 == 0)
        {
            blocks.insert(blocks.end(),length,index/2);
        }
        else
        {
            blocks.insert(blocks.end(),length,-1);
        }
    }

    fmt::print("Disk Size: {} Before: {}\n", blocks.size(), blocks);
    auto empty = std::find(blocks.begin(), blocks.end(), -1);
    auto data = std::find_if_not(blocks.rbegin(), std::make_reverse_iterator(empty), [](auto item) {return item == -1;});
    while(std::make_reverse_iterator(empty) > data)
    {
        std::swap(*empty, *data);
        empty = std::find(empty, data.base(), -1);
        data = std::find_if_not(data, std::make_reverse_iterator(empty), [](auto item) {return item == -1;});
    }

    fmt::print("After: {}\n", blocks);

    long sum = 0;
    for(auto block : blocks | boost::adaptors::indexed())
    {
        if(block.value() != -1) { sum += block.index() * block.value(); }
    }
    fmt::print("Checksum: {}\n", sum);
    return 0;
}
