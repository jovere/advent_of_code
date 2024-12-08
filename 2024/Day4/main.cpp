#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <boost/multi_array.hpp>

using namespace std::literals;

[[nodiscard]] int
countStrings(std::ranges::forward_range auto const &str, std::ranges::forward_range auto const &find)
{
    int count = 0;
    for (auto found = std::ranges::search(str, find);
         !found.empty();
         found = std::ranges::search(found.end(), str.end(), find.begin(), find.end()))
    {
        ++count;
    }
    return count;
}

[[nodiscard]] bool
isXMas(boost::multi_array<char, 2> const &grid)
{
    return ((grid[1][1] == 'A')
        && ((grid[0][0] == 'M' && grid[2][2] == 'S')
            || (grid[0][0] == 'S' && grid[2][2] == 'M'))
        && ((grid[0][2] == 'M' && grid[2][0] == 'S')
            || (grid[0][2] == 'S' && grid[2][0] == 'M')));
}

int
main()
{

    std::vector<std::string> lines;
    {
        std::fstream input("input.txt", std::ios::in);
        if (!input.is_open())
        {
            std::cerr << "Can't open file." << std::endl;
            return 1;
        }

        std::string line;
        while (std::getline(input, line))
        {
            lines.push_back(line);
        }
    }

    std::cout << "Width: " << lines.at(0).size() << "\n";
    std::cout << "Height: " << lines.size() << "\n";

    using range = boost::multi_array_types::index_range;
    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size()]};

    for (int i = 0; i < lines.size(); ++i)
    {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    int occurrences = 0;
    long const columnCount = static_cast<long>(grid.shape()[0]);
    long const rowCount = static_cast<long>(grid.shape()[1]);

    for (int i = 1; i < columnCount - 1; ++i)
    {
        for (int j = 1; j < rowCount - 1; ++j)
        {
            auto subgrid = grid[boost::indices[range{i - 1, i + 2}][range{j - 1, j + 2}]];
            if (isXMas(subgrid))
            {
                ++occurrences;
            }
        }
    }

#ifdef OLDCODE
    for (int i = 0; i < columnCount; ++i)
    {
        occurrences += countStrings(grid[i], "XMAS"sv) + countStrings(grid[i], "SAMX"sv);
    }

    for (int i = 0; i < rowCount; ++i)
    {
        auto subgrid = grid[boost::indices[range{0, columnCount}][i]];
        occurrences += countStrings(subgrid, "XMAS"sv) + countStrings(subgrid, "SAMX"sv);
    }

    std::string diagonals;
    for (int row = 0; row < rowCount; ++row)
    {
        for (int j = 0, k = row; j <= row; ++j, --k)
        {
            diagonals += (grid[j][k]);
        }
        diagonals += ("\n");
        for (long j = 0, k = columnCount - row - 1; k < columnCount; ++j, ++k)
        {
            diagonals += (grid[j][k]);
        }
        diagonals += ("\n");
    }

    for (auto row = 1; row < rowCount; ++row)
    {
        for (long j = row, k = columnCount - 1; j < rowCount; ++j, --k)
        {
            diagonals += (grid[j][k]);
        }
        diagonals += ("\n");
        for (long j = row, k = 0; j < rowCount; ++j, ++k)
        {
            diagonals += (grid[j][k]);
        }
        diagonals += ("\n");
    }
    occurrences += countStrings(diagonals, "XMAS"sv) + countStrings(diagonals, "SAMX"sv);
#endif
    std::cout << "Occurrences: " << occurrences << "\n";
}