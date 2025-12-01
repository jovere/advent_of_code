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
            line[0] = (line[0] == 'R') ? '+' : '-';
            moves.push_back(std::stoi(line));
        }

        unsigned int index = 50;
        unsigned int stopsOnZero = 0;
        unsigned int zeros = 0;
        for (auto move: moves)
        {
            fmt::print("{:4}, ", move);
            if (move > 0)
            {
                zeros += (move / 100);
                index += (move % 100);
                zeros += (index / 100);
                index %= 100;
            }
            else
            {
                // There has to be a better way of doing this...
                if (index == 0) --zeros;
                move = -move;
                zeros += (move / 100);
                index += 100;
                index -= (move % 100);
                zeros += (index / 100) == 0 ? 1 : 0;
                index %= 100;
                if (index == 0) ++zeros;
            }

            if (index == 0)
            {
                ++stopsOnZero;
            }
            fmt::print("{:2}, {}\n", index, zeros);
        }
        fmt::print("\nZeros: {}\n", stopsOnZero);
    }



    return 0;
}
