#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

int
main() {

    std::vector<std::pair<long int,long int>> rangesToSearch;
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
            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(","));
            fmt::print("Tokens: {}\n", tokens);
            for (auto token: tokens)
            {
                std::vector<std::string> tokens2;
                boost::split(tokens2, token, boost::is_any_of("-"));
                rangesToSearch.emplace_back(std::stoll(tokens2[0]), std::stoll(tokens2[1]));
            }
            std::ranges::sort(rangesToSearch);
        }
    }

    fmt::print("Ranges: {}\n", rangesToSearch);

    long long part1Sum = 0;
    long long part2Sum = 0;
    for (auto range : rangesToSearch)
    {
        for (auto x = range.first; x <= range.second; ++x)
        {
            auto const value = std::to_string(x);
            // Part 1
            if ((value.length() % 2) == 0)
            {
                auto length = value.length() / 2;
                if (value.substr(0, length) == value.substr(length, length))
                {
                    fmt::print("{}\n", x);
                    part1Sum += x;
                }
            }

            // Part 2
            // Only check up to half the length of value, since there can't be a duplicate if it's longer.
            bool valid = true;
            for (auto sampleWidth = 1; sampleWidth <= value.length() / 2 && valid; ++sampleWidth)
            {
                // Length must be a multiple of the sampleWidth
                if (value.length() % sampleWidth == 0)
                {
                    valid = false;
                    std::string sample = value.substr(0, sampleWidth);
                    for (auto index = sampleWidth; index < value.length() && !valid; index += sampleWidth)
                    {
                        if (sample != value.substr(index, sampleWidth))
                        {
                            valid = true;
                        }
                    }
                }
            }
            if (!valid)
            {
                fmt::print("-- {}\n", x);
                part2Sum += x;
            }
        }
    }

    fmt::print("Sum: {}\n", part1Sum);
    fmt::print("Sum2: {}\n", part2Sum);

    return 0;
}
