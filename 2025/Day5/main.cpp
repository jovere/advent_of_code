#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

int
main() {

    std::vector<std::pair<long int,long int>> rangesToSearch;
    std::vector<long int> numbersToSearchFor;
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

        while (std::getline(input, line))
        {
            numbersToSearchFor.push_back(std::stoll(line));
        }
    }

    fmt::print("Ranges: {}\n", rangesToSearch);
    fmt::print("Range Count: {}\n", rangesToSearch.size());

    auto condenseRanges = [](std::vector<std::pair<long int,long int>> & ranges)
    {
        std::vector<std::pair<long int,long int>> rangesCopy;
        for (int i = 0; i < ranges.size() - 1; ++i)
        {
            if (ranges[i].second >= ranges[i + 1].first)
            {
                if (ranges[i].second < ranges[i + 1].second)
                {
                    rangesCopy.emplace_back(ranges[i].first, ranges[i+1].second);
                }
                else
                {
                    rangesCopy.emplace_back(ranges[i]);
                }
                ++i;
            }
            else
            {
                rangesCopy.emplace_back(ranges[i]);
            }
        }
        if (rangesCopy.end()->second != ranges.end()->second)
        {
            rangesCopy.push_back(ranges.back());
        }
        return rangesCopy;
    };

    std::vector<std::pair<long int, long int>> condensedRanges;
    while ((condensedRanges = condenseRanges(rangesToSearch)) != rangesToSearch)
    {
        rangesToSearch = condensedRanges;
    }

    fmt::print("Ranges: {}\n", rangesToSearch);
    fmt::print("Range Count: {}\n", rangesToSearch.size());
    fmt::print("Condensed Ranges {}\n", rangesToSearch);

    long long freshCount = 0;
    for (auto range : rangesToSearch)
    {
        freshCount += range.second - range.first + 1;
    }
    fmt::print("Fresh count: {}\n", freshCount);
    return 0;
}
