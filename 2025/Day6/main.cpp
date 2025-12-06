#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

int
main() {
    std::vector<long long> firstNumber;
    std::vector<long long> secondNumber;
    std::vector<long long> thirdNumber;
    std::vector<long long> fourthNumber;
    std::vector<char> operators;
    {
        std::fstream input("input.txt", std::ios::in);
        if (!input.is_open()) {
            fmt::print(stderr, "Can't open file.");
            return 1;
        }

        auto getNumbers = [&]()
        {
            std::string line;
            std::getline(input, line);
            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "), boost::token_compress_on);

            std::vector<long long> numbers;
            for (auto token : tokens)
            {
                numbers.push_back(std::stoi(token));
            }

            return numbers;
        };
        auto getOperators = [&]()
        {
            std::string line;
            std::getline(input, line);
            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "), boost::token_compress_on);

            fmt::print(stderr, "tokens: {}\n", tokens);
            std::vector<char> ops;
            for (auto token : tokens)
            {
                ops.push_back(token[0]);
            }
            return ops;
        };

        firstNumber = getNumbers();
        secondNumber = getNumbers();
        thirdNumber = getNumbers();
        fourthNumber = getNumbers();
        operators = getOperators();
        fmt::print("size: {} tokens: {}\n", firstNumber.size(), firstNumber);
        fmt::print("size: {} tokens: {}\n", secondNumber.size(), secondNumber);
        fmt::print("size: {} tokens: {}\n", thirdNumber.size(), thirdNumber);
        fmt::print("size: {} tokens: {}\n", fourthNumber.size(), fourthNumber);
        fmt::print("size: {} operators: {}\n", operators.size(), operators);

        long long total = 0;
        for (int i = 0; i < operators.size(); i++)
        {
            if (operators[i] == '*')
            {
                total += firstNumber[i] * secondNumber[i] * thirdNumber[i] * fourthNumber[i];
            }
            else if (operators[i] == '+')
            {
                total += firstNumber[i] + secondNumber[i] + thirdNumber[i] + fourthNumber[i];
            }
        }

        fmt::print("Total numbers: {}\n", total);
    }

    return 0;
}
