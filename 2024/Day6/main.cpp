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
#ifdef TRASH
std::tuple<int, int, Direction>
nextStep(boost::multi_array<char, 2> &grid, std::tuple<int, int, Direction> location)
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

    while (row == nextRow && col == nextCol)
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
            case Direction::None:break;
        }
    }
    return {nextRow, nextCol, nextDir};
}

bool
willLoop(boost::multi_array<char, 2> const &grid,
         std::tuple<int, int, Direction> location,
         std::vector<std::tuple<int, int, Direction>> &history,
         std::tuple<int,int> blockLocation)
{
    auto [row, col, direction] = location;
    if(std::ranges::find(history,location) != history.end())
    {
        return true;
    }
    history.push_back(location);
    switch (direction)
    {
        case Direction::Up:
            for (; row > 0; --row)
            {
                if (grid[row - 1][col] == '#' || blockLocation == std::tuple<int,int>{row-1,col})
                {
                    switch (grid[row][col])
                    {
                        case '+': return true;
                        default: return willLoop(grid, {row, col, nextDirection(direction)}, history, blockLocation);
                    }
                }
            }
            break;
        case Direction::Right:
            for (; col < grid.shape()[1] - 1; ++col)
            {
                if (grid[row][col + 1] == '#'|| blockLocation == std::tuple<int,int>{row,col+1})
                {
                    switch (grid[row][col])
                    {
                        case '+': return true;
                        default: return willLoop(grid, {row, col, nextDirection(direction)}, history, blockLocation);
                    }
                }
            }
            break;
        case Direction::Down:
            for (; row < grid.shape()[0] - 1; ++row)
            {
                if (grid[row + 1][col] == '#'|| blockLocation == std::tuple<int,int>{row+1,col})
                {
                    switch (grid[row][col])
                    {
                        case '+': return true;
                        default: return willLoop(grid, {row, col, nextDirection(direction)}, history, blockLocation);
                    }
                }
            }
            break;
        case Direction::Left:
            for (; col > 0; --col)
            {
                if (grid[row][col - 1] == '#'|| blockLocation == std::tuple<int,int>{row,col-1})
                {
                    switch (grid[row][col])
                    {
                        case '+': return true;
                        default: return willLoop(grid, {row, col, nextDirection(direction)}, history, blockLocation);
                    }
                }
            }
            break;
        case Direction::None:break;
    }
    return false;
}

bool
canPlaceBlock(boost::multi_array<char, 2> const &grid,
              std::tuple<int, int, Direction> location,
              std::vector<std::tuple<int, int, Direction>> &history)
{
    auto [row, col, direction] = location;
    history.push_back(location);
    switch (direction)
    {
        case Direction::None: return false;
        case Direction::Up:
            if (row - 1 < 0 || boost::is_any_of("^#")(grid[row - 1][col]))
                return false;
            return willLoop(grid, {row, col, nextDirection(direction)}, history,{row-1,col});
        case Direction::Right:
            if (col + 1 >= grid.shape()[1] || boost::is_any_of("^#")(grid[row][col + 1]))
                return false;
            return willLoop(grid, {row, col, nextDirection(direction)}, history,{row,col+1});
        case Direction::Down:
            if (row + 1 >= grid.shape()[0] || boost::is_any_of("^#")(grid[row + 1][col]))
                return false;
            return willLoop(grid, {row, col, nextDirection(direction)}, history, {row+1,col});
        case Direction::Left:
            if (col - 1 < 0 || boost::is_any_of("^#")(grid[row][col - 1]))
                return false;
            return willLoop(grid, {row, col, nextDirection(direction)}, history,{row,col-1});
    }
    return false;
}
#endif
std::pair<std::tuple<int, int, Direction>, int>
walkToNextObstacle(boost::multi_array<char, 2> &grid, std::tuple<int, int, Direction> start)
{
    std::tuple<int, int, Direction> stop{-1, -1, Direction::None};
    int steps = 0;
    auto [row, col, d] = start;
    switch (d)
    {
        case Direction::Up:
            for (; row >= 0; --row)
            {
                if (grid[row][col] == '#')
                {
                    stop = {++row, col, nextDirection(d)};
                    break;
                }
                else if (!boost::is_any_of("X^")(grid[row][col]))
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Right:
            for (; col < grid.shape()[1]; ++col)
            {
                if (grid[row][col] == '#')
                {
                    stop = {row, --col, nextDirection(d)};
                    break;
                }
                else if (!boost::is_any_of("X^")(grid[row][col]))
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Down:
            for (; row < grid.shape()[0]; ++row)
            {
                if (grid[row][col] == '#')
                {
                    stop = {--row, col, nextDirection(d)};
                    break;
                }
                else if (!boost::is_any_of("X^")(grid[row][col]))
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Left:
            for (; col >= 0; --col)
            {
                if (grid[row][col] == '#')
                {
                    stop = {row, ++col, nextDirection(d)};
                    break;
                }
                else if (!boost::is_any_of("X^")(grid[row][col]))
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::None: break;
    }
    //fmt::print("Unique Steps: {} Stop: [{},{}]\n", steps, get<0>(stop), get<1>(stop));
    return {stop, steps};
}

std::tuple<bool, boost::multi_array<char, 2>>
executeWalk(boost::multi_array<char, 2> const &grid, std::tuple<int, int, Direction> position)
{
    auto part1grid = grid;
    int totalSteps = 0;
    Direction d = Direction::Up;
    std::vector<std::tuple<int, int, Direction>> history;
    do
    {
        history.push_back(position);
        auto [stop, steps] = walkToNextObstacle(part1grid, position);
        position = stop;
        totalSteps += steps;
    }
    while (position != std::tuple<int, int, Direction>{-1, -1, Direction::None}
        && std::ranges::find(history, position) == history.end());

    auto loops = position != std::tuple<int, int, Direction>{-1, -1, Direction::None};
    fmt::print("Total Unique Steps: {} Loops? {}\n", totalSteps + 1, loops);
    return {loops, part1grid};
}

int
main()
{

    std::vector<std::string> lines;
    {
        std::fstream input("input.txt", std::ios::in);
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

    auto startLocation = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '^');
    fmt::print("Start Position: [{},{}] Value: {}\n",
               startLocation.get_indices()[0],
               startLocation.get_indices()[1],
               *startLocation);

    printGrid(grid);

    int loopCount = 0;
    {
        auto [loops, initial] =
            executeWalk(grid, {startLocation.get_indices()[0], startLocation.get_indices()[1], Direction::Up});
        printGrid(initial);
        for (auto location = std::find(multi_array_helper::begin(initial), multi_array_helper::end(initial), 'X');
             location != multi_array_helper::end(initial);
             location = std::find(++location, multi_array_helper::end(initial), 'X'))
        {
            auto row = location.get_indices()[0];
            auto col = location.get_indices()[1];
            fmt::print("Looking at: [{},{}] ", row, col);
            grid[row][col] = '#';
            auto [looping, _] =
                executeWalk(grid, {startLocation.get_indices()[0], startLocation.get_indices()[1], Direction::Up});
            grid[row][col] = '.';
            if (looping)
                ++loopCount;
        }

        fmt::print("Loop Count: {}", loopCount);
    }

#ifdef TRASH
    {
        auto part2grid = grid;
        std::tuple<int, int, Direction>
            lastPosition{location.get_indices()[0], location.get_indices()[1], Direction::Up};
        int PassNumber = 1;
        int blockCount = 0;
        do
        {
            auto [row, col, dir] = lastPosition;
            std::vector<std::tuple<int, int, Direction>> history{};
            auto blockable = canPlaceBlock(part2grid, lastPosition, history);
            fmt::print("Step: {} Start: ({},{},{}) Can place block: {}\n", PassNumber, row, col, [dir]{
                switch(dir)
                {
                    case Direction::Up: return "Up";
                    case Direction::Right: return "Right";
                    case Direction::Down: return "Down";
                    case Direction::Left: return "Left";
                    case Direction::None: break;
                }
                return "-";
            }(), blockable);
            if (blockable)
            {
                ++blockCount;
            }
            lastPosition = nextStep(part2grid, lastPosition);
            //printGrid(part2grid);
            ++PassNumber;
        }
        while (lastPosition != std::tuple<int, int, Direction>{-1, -1, Direction::None});

        fmt::print("\n\n");
        printGrid(part2grid);
        fmt::print("Block Count: {}\n", blockCount);
    }
#endif

    return 0;
}