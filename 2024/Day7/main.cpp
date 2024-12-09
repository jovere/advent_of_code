#include <fstream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

int
main()
{
    std::fstream input("input.txt", std::ios::in);
    if (!input.is_open())
    {
        fmt::print(stderr, "Can't open file.");
        return 1;
    }

    std::string line;

    // Get all the order rules from the file in a vector
    std::vector<std::pair<uint64_t, std::vector<uint64_t>>> calibrationSet;
    while (getline(input, line) && !line.empty())
    {
        std::vector<std::string> result;
        boost::split(result, line, boost::is_any_of(":"));
        std::vector<uint64_t> operands;
        {
            std::vector<std::string> strOperands;
            boost::trim(result[1]);
            boost::split(strOperands, result[1], boost::is_any_of(" "));
            operands.reserve(strOperands.size());
            for(auto& op: strOperands)
            {
                operands.push_back(std::stol(op));
            }
        }
        calibrationSet.emplace_back(std::stol(result[0]), operands);
    }

    fmt::print("Map: {}\n", calibrationSet);

    uint64_t sumTotal = 0;
    for(auto const& cal: calibrationSet)
    {
        auto& expectedTotal = cal.first;
        auto& data = cal.second;
        auto operatorCount = data.size()-1;
        uint64_t operatorCombinations = 1UL << operatorCount;

        fmt::print("Operator count of {} is {}. ", data, operatorCount);
        fmt::print("Possible operator combinations is {}. ", operatorCombinations);
        fmt::print("Desired Total is {}.\n", expectedTotal);

        bool match = false;
        do
        {
            --operatorCombinations;
            auto combo = operatorCombinations;
            uint64_t total = std::accumulate(std::next(data.begin()), data.end(), data[0], [&combo](uint64_t a, uint64_t b)
            {
                uint64_t retVal = 0;
                if((combo & 0x1) == 0x1)
                {
                    retVal = a * b;
                }
                else
                {
                    retVal = a + b;
                }
                combo /= 2;
                return retVal;
            });
            match |= total == expectedTotal;
            fmt::print("Operations {:0{}b} is {}. {}\n", operatorCombinations, operatorCount, total, total == expectedTotal ? "MATCH" : "");
        }while(operatorCombinations != 0);// && !match);
        sumTotal += (match) ? expectedTotal : 0;
    }

    fmt::print("Total calibration result: {}\n", sumTotal);
    return 0;
}
