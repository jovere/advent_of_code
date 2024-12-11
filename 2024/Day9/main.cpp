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

    // Part 1
    if(0) {
        auto empty = std::find(blocks.begin(), blocks.end(), -1);
        auto data = std::find_if_not(blocks.rbegin(), std::make_reverse_iterator(empty),
                                     [](auto item) { return item == -1; });
        while (std::make_reverse_iterator(empty) > data) {
            std::swap(*empty, *data);
            empty = std::find(empty, data.base(), -1);
            data = std::find_if_not(data, std::make_reverse_iterator(empty), [](auto item) { return item == -1; });
        }
    }
    else // Part 2
    {
        auto dataRStart = std::find_if_not(blocks.rbegin(), blocks.rend(), [](auto item) { return item == -1; });
        auto dataREnd = std::find_if_not(dataRStart, blocks.rend(), [dataRStart](auto item) {return item == *dataRStart;});
        auto emptyStart = std::find(blocks.begin(), dataREnd.base(), -1);
        auto emptyEnd = std::find_if_not(emptyStart, dataREnd.base(), [](auto item) { return item == -1; });
        fmt::print("Data Size: {}, Empty Size: {}\n", dataREnd-dataRStart, emptyEnd-emptyStart);
        while(std::make_reverse_iterator(emptyEnd) > dataREnd )
        {
            auto holeStart = emptyStart;
            auto holeEnd = emptyEnd;
            while(std::make_reverse_iterator(holeEnd) > dataREnd)
            {
                // If the hole is big enough, swap the data
                if(dataREnd - dataRStart <= holeEnd - holeStart)
                {
                    std::swap_ranges(dataRStart,dataREnd, holeStart);
                    break;
                }
                holeStart = std::find(holeEnd+1, dataREnd.base(), -1);
                holeEnd = std::find_if_not(holeStart, dataREnd.base(), [](auto item) { return item == -1; });
            }
            dataRStart = std::find_if_not(dataREnd, std::make_reverse_iterator(emptyStart), [](auto item) { return item == -1; });
            dataREnd = std::find_if_not(dataRStart, std::make_reverse_iterator(emptyStart), [dataRStart](auto item) {return item == *dataRStart;});
            emptyStart = std::find(blocks.begin(), dataREnd.base(), -1);
            emptyEnd = std::find_if_not(emptyStart, dataREnd.base(), [](auto item) { return item == -1; });
        }
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
