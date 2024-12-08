#include <iostream>
#include <algorithm>
#include <fstream>
#include <ranges>
#include <sstream>

enum class Direction
{
    Unknown,
    Increase,
    Decrease
};

auto
find_bad_index(std::vector<int> const &levels)
{
    Direction d = Direction::Unknown;

    return std::ranges::adjacent_find(levels, [&d](auto const &l, auto const &r) -> bool
    {
        // looking for unsafe
        if (l == r)
        {
            return true;
        }

        if (d == Direction::Unknown)
        {
            d = (l < r) ? Direction::Increase : Direction::Decrease;
        }

        int diff = (d == Direction::Increase) ? r - l : l - r;

        if (diff > 3 || diff < 1)
        {
            return true;
        }
        return false;
    });
}

int
main()
{

    std::stringstream data;
    {
        std::fstream input("input.txt", std::ios::in);
        if (!input.is_open())
        {
            std::cerr << "Can't open file." << std::endl;
            return 1;
        }
        data << input.rdbuf();
    }

    std::string line;
    int safe = 0;
    int lineNo = 0;
    while (std::getline(data, line))
    {
        std::cout << ++lineNo << " ";
        std::vector<int> levels;
        {
            std::stringstream lineData{line};
            int level;
            while (lineData >> level)
            {
                levels.push_back(level);
            }
        }

        if (auto result = find_bad_index(levels); result == levels.end())
        {
            std::cout << std::boolalpha << true;
            ++safe;
        }
        else
        {
            auto badIndex = std::distance(std::as_const(levels).begin(), result);
            std::cout << std::boolalpha << false;
            for (int i = 0; i < levels.size(); ++i)
            {
                auto levels2{levels};
                levels2.erase(levels2.begin() + i);
                if (result = find_bad_index(levels2); result == levels2.end())
                {
                    std::cout << std::boolalpha << " " << true;
                    ++safe;
                    break;
                }

                std::cout << std::boolalpha << " " << false;
            }
        }
        std::cout << "\n";
    }

    std::cout << "Total Safe: " << safe;
}