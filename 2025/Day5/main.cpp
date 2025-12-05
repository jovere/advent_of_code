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
    fmt::print("Numbers: {}\n", numbersToSearchFor);

    long freshCount = 0;
    for (auto number : numbersToSearchFor)
    {
        int fresh = 0;
        for (auto range : rangesToSearch)
        {
            if (number >= range.first && number <= range.second)
            {
                fresh = 1;
                break;
            }
        }

        freshCount+= fresh;
    }

    fmt::print("Fresh count: {}\n", freshCount);
    return 0;
}
