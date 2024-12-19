#include <fstream>
#include <fmt/format.h>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/tokenizer.hpp>

int
main() {
    std::fstream input("input.txt", std::ios::in);
    if (!input.is_open()) {
        fmt::print(stderr, "Can't open file.");
        return 1;
    }

    std::vector<std::tuple<Eigen::Matrix2d, Eigen::Vector2d>> machines;
    {
        // Get all the data
        using tokenizer = boost::tokenizer<boost::char_separator<char>>;
        std::tuple<Eigen::Matrix2d, Eigen::Vector2d> machine;
        boost::char_separator<char> seperator(":+,= XY");
        std::string line;
        while (std::getline(input, line)) {
            if (line.empty()) {
                machines.push_back(machine);
                machine = {};
            } else {
                tokenizer tokens(line, seperator);
                auto itr = tokens.begin();
                if (*itr == "Button") {
                    if (*(++itr) == "A") {
                        std::get<0>(machine)(0, 0) = std::stoi(*(++itr));
                        fmt::print("A: {},", *itr);
                        std::get<0>(machine)(1, 0) = std::stoi(*(++itr));
                        fmt::print("{}\n", *itr);
                    } else {
                        std::get<0>(machine)(0, 1) = std::stoi(*(++itr));
                        fmt::print("B: {},", *itr);
                        std::get<0>(machine)(1, 1) = std::stoi(*(++itr));
                        fmt::print("{}\n", *itr);
                    }
                } else {
                    std::get<1>(machine)(0) = std::stoi(*(++itr)) + 10000000000000;
                    fmt::print("T: {},", *itr);
                    std::get<1>(machine)(1) = std::stoi(*(++itr)) + 10000000000000;
                    fmt::print("{}\n\n", *itr);
                }
            }
        }
        machines.push_back(machine);
    }

    long total = 0;
    for (auto &[A, b]: machines) {
        auto result = A.colPivHouseholderQr().solve(b);
        auto buttonA = std::round(result(0));
        auto buttonB = std::round(result(1));

        if (buttonA * A(0, 0) + buttonB * A(0, 1) != b(0) || buttonA * A(1, 0) + buttonB * A(1, 1) != b(1)) {
                fmt::print("Doesn't Work!\n");
            } else {
                std::cout << result << "\n";
                fmt::print("|{} {}||{}| |{}|\n", A(0, 0), A(0, 1), b(0), buttonA);
                fmt::print("|{} {}||{}| |{}|\n", A(1, 0), A(1, 1), b(1), buttonB);
                fmt::print("Equal!");
                total += buttonA * 3 + buttonB * 1;
        }
        fmt::print("\n");
    }

    fmt::print("Total Cost: {}", total);
    return 0;
}
