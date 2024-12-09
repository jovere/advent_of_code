#include <fstream>
#include <vector>
#include <unordered_map>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

int
main()
{
    std::fstream input("input_test.txt", std::ios::in);
    if (!input.is_open())
    {
        fmt::print(stderr, "Can't open file.");
        return 1;
    }

    std::string line;

    // Get all the order rules from the file in a map
    std::unordered_map<long, std::vector<long>> calibrationSet;
    while (getline(input, line) && !line.empty())
    {
        std::vector<std::string> result;
        boost::split(result, line, boost::is_any_of(":"));
        std::vector<long> operands;
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
        calibrationSet.insert({std::stol(result[0]), operands});
    }

    fmt::print("Map: {}", calibrationSet);


    return 0;
}
