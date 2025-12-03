#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>


bool findLargestDigitLocation(std::string_view s, std::string& result, int width)
{
    if (width == 0)
        return true;

    for (char i = '9'; i >= '0'; --i)
    {
        auto digitIndex = s.find(i);
        if (digitIndex != std::string::npos && digitIndex + width - 1 <= s.size())
        {
            if (findLargestDigitLocation( s.substr(digitIndex+1), result, width - 1))
            {
                result[12 - width] = s[digitIndex];
                return true;
            }
        }
    }

    return false;
}

int
main() {

    std::vector<std::string> batteryBanks;
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
            batteryBanks.push_back(line);
        }
    }

    long long total = 0;
    for (const auto &bank : batteryBanks)
    {
        std::string line{"            "};
        findLargestDigitLocation(bank, line, 12);
        fmt::print("Bank is {}\n", line);
        long long value = std::stoll(line);
        total += value;
    }

    fmt::print("Total is {}\n", total);

    return 0;
}
