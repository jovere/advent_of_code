#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

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

    long total = 0;
    for (const auto &bank : batteryBanks)
    {
        int value = 0;
        for (char i = '9'; i >= '0' && value == 0; --i)
        {
            auto firstDigit = bank.find(i);
            if (firstDigit != std::string::npos)
            {
                for ( char j = '9'; j >= '0' && value == 0; --j)
                {
                    auto secondDigit = bank.find(j, firstDigit+1);
                    if (secondDigit != std::string::npos)
                    {
                        std::string s {bank[firstDigit]};
                        s += bank[secondDigit];
                        value = std::stoi(s);
                    }
                }
            }
        }
        total += value;
        fmt::print("Bank is {}\n", value);
    }

    fmt::print("Total is {}\n", total);

    return 0;
}
