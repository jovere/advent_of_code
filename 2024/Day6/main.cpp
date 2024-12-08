#include <vector>
#include <fstream>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>
#include "multi_array_helper.hpp"

enum class Direction
{
    None,
    Up,
    Right,
    Down,
    Left
};

void
printGrid(boost::multi_array<char, 2> const &grid)
{
    for (auto row : grid)
    {
        for (auto column : row)
        {
            fmt::print("{}", column);
        }
        fmt::print("\n");
    }
}

auto
nextDirection(Direction &d)
{
    switch (d)
    {
        case Direction::None:return d = Direction::None;
        case Direction::Up: return d = Direction::Right;
        case Direction::Right:return d = Direction::Down;
        case Direction::Down:return d = Direction::Left;
        case Direction::Left:return d = Direction::Up;
    }
    abort();
};

std::tuple<int,int,Direction> nextStep(boost::multi_array<char, 2> &grid, std::tuple<int, int,Direction> location)
{
    auto const [row, col, direction] = location;
    auto nextRow = row;
    auto nextCol = col;
    auto nextDir = direction;
    auto reset = [&nextRow, &nextCol, &nextDir]()
    {
        nextRow = nextCol = -1;
        nextDir = Direction::None;
    };

    while(row == nextRow && col == nextCol)
    {
        switch (nextDir)
        {
            case Direction::Up:
                if (nextRow > 0 && grid[nextRow - 1][nextCol] != '#')
                {
                    --nextRow;
                    switch (grid[nextRow][nextCol])
                    {
                        case '.': grid[nextRow][nextCol] = '|';
                            break;
                        case '-': grid[nextRow][nextCol] = '+';
                            break;
                    }
                }
                else if (nextRow == 0)
                {
                    reset();
                }
                else
                {
                    grid[nextRow][nextCol] = '+';
                    nextDirection(nextDir);
                }
                break;
            case Direction::Right:
                if (nextCol < grid.shape()[1] - 1 && grid[nextRow][nextCol + 1] != '#')
                {
                    ++nextCol;
                    switch (grid[nextRow][nextCol])
                    {
                        case '.': grid[nextRow][nextCol] = '-';
                            break;
                        case '|': grid[nextRow][nextCol] = '+';
                            break;
                    }
                }
                else if (nextCol == grid.shape()[1] - 1)
                {
                    reset();
                }
                else
                {
                    grid[nextRow][nextCol] = '+';
                    nextDirection(nextDir);
                }
                break;
            case Direction::Down:
                if (nextRow < grid.shape()[0] - 1 && grid[nextRow + 1][nextCol] != '#')
                {
                    ++nextRow;
                    switch (grid[nextRow][nextCol])
                    {
                        case '.': grid[nextRow][nextCol] = '|';
                            break;
                        case '-': grid[nextRow][nextCol] = '+';
                            break;
                    }
                }
                else if (nextRow == grid.shape()[0] - 1)
                {
                    reset();
                }
                else
                {
                    grid[nextRow][nextCol] = '+';
                    nextDirection(nextDir);
                }
                break;
            case Direction::Left:
                if (nextCol > 0 && grid[nextRow][nextCol - 1] != '#')
                {
                    --nextCol;
                    switch (grid[nextRow][nextCol])
                    {
                        case '.': grid[nextRow][nextCol] = '-';
                            break;
                        case '|': grid[nextRow][nextCol] = '+';
                            break;
                    }
                }
                else if (nextCol == 0)
                {
                    reset();
                }
                else
                {
                    grid[nextRow][nextCol] = '+';
                    nextDirection(nextDir);
                }
                break;
            case Direction::None:
                break;
        }
    }
    return {nextRow,nextCol,nextDir};
}

std::pair<std::tuple<int, int>, int>
walkToNextObstacle(boost::multi_array<char, 2> &grid, std::tuple<int, int> start, Direction d)
{
    std::tuple<int, int> stop{-1, -1};
    int steps = 0;

    switch (d)
    {
        case Direction::Up:
            for (auto [row, col] = start; row >= 0; --row)
            {
                if (grid[row][col] == '#')
                {
                    stop = {++row, col};
                    break;
                }
                else if (grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Right:
            for (auto [row, col] = start; col < grid.shape()[1]; ++col)
            {
                if (grid[row][col] == '#')
                {
                    stop = {row, --col};
                    break;
                }
                else if (grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Down:
            for (auto [row, col] = start; row < grid.shape()[0]; ++row)
            {
                if (grid[row][col] == '#')
                {
                    stop = {--row, col};
                    break;
                }
                else if (grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Left:
            for (auto [row, col] = start; col >= 0; --col)
            {
                if (grid[row][col] == '#')
                {
                    stop = {row, ++col};
                    break;
                }
                else if (grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
    }
    fmt::print("Unique Steps: {} Stop: [{},{}]\n", steps, get<0>(stop), get<1>(stop));
    return {stop, steps};
}

int
main()
{

    std::vector<std::string> lines;
    {
        std::fstream input("input_test.txt", std::ios::in);
        if (!input.is_open())
        {
            fmt::print(stderr, "Can't open file.");
            return 1;
        }

        std::string line;
        while (std::getline(input, line))
        {
            lines.push_back(line);
        }
    }

    fmt::print("Width: {}\n", lines.at(0).size());
    fmt::print("Height: {}\n", lines.size());

    using range = boost::multi_array_types::index_range;
    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size()]};

    for (int i = 0; i < lines.size(); ++i)
    {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    auto location = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '^');
    fmt::print("Start Position: [{},{}] Value: {}\n", location.get_indices()[0], location.get_indices()[1], *location);

    printGrid(grid);

    if (0)
    {
        auto part1grid = grid;
        int totalSteps = 0;
        std::tuple<int, int> lastPosition{location.get_indices()[0], location.get_indices()[1]};
        Direction d = Direction::Left;
        do
        {
            auto [stop, steps] = walkToNextObstacle(part1grid, lastPosition, nextDirection(d));
            lastPosition = stop;
            totalSteps += steps;
        }
        while (lastPosition != std::tuple<int, int>{-1, -1});
        fmt::print("Ending Grid:\n");
        printGrid(part1grid);
        fmt::print("Total Unique Steps: {}\n", totalSteps);
    }

    {
        auto part2grid = grid;
        std::tuple<int, int, Direction>
            lastPosition{location.get_indices()[0], location.get_indices()[1], Direction::Up};
        int PassNumber = 1;
        do
        {
            auto [row,col,dir] = lastPosition;
            fmt::print("Pass: {} Start: ({},{},{})\n", PassNumber, row, col, [dir]{
                switch(dir)
                {
                    case Direction::Up: return "Up";
                    case Direction::Right: return "Right";
                    case Direction::Down: return "Down";
                    case Direction::Left: return "Left";
                    case Direction::None: break;
                }
                return "-";
            }());
            lastPosition = nextStep(part2grid, lastPosition);
            printGrid(part2grid);
            ++PassNumber;
        }while(lastPosition != std::tuple<int,int,Direction>{-1,-1,Direction::None});
    }

    return 0;
}