#include <vector>
#include <fstream>
#include <fmt/format.h>
#include <fmt/ranges.h>

int
main() {

    std::vector<int> moves;
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
            line[0] = (line[0] == 'R') ? ' ' : '-';
            moves.push_back(std::stoi(line));
        }

        unsigned int index = 50;
        fmt::print("Moves: {:-4}\n", fmt::join(moves, ", "));
        fmt::print("Point: ");
        unsigned int zeros = 0;
        for (auto move: moves)
        {
            if (move < 0)
            {
                index = ((index + 100) - ((-move) % 100)) % 100;
            }
            else
            {
                index = (index + move) % 100;
            }

            if (index == 0)
            {
                ++zeros;
            }
            fmt::print("{:4}, ", index);
        }
        fmt::print("\nZeros: {}\n", zeros);
    }



    return 0;
}
