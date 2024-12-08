#include <vector>
#include <fstream>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>
#include "multi_array_helper.hpp"

enum class Direction{
    Up,
    Right,
    Down,
    Left
};

std::pair<std::tuple<int,int>, int> walkForward(boost::multi_array<char, 2>& grid, std::tuple<int,int> start, Direction d)
{
    std::tuple<int, int> stop{-1,-1};
    int steps = 0;

    switch(d)
    {
        case Direction::Up:
            for(auto [row,col] = start; row >= 0 ; --row)
            {
                if(grid[row][col] == '#')
                {
                    stop = {++row, col};
                    break;
                }
                else if(grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Right:
            for(auto[row,col] = start; col < grid.shape()[1]; ++col)
            {
                if(grid[row][col] == '#')
                {
                    stop = {row, --col};
                    break;
                }
                else if(grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Down:
            for(auto[row,col] = start; row < grid.shape()[0]; ++row)
            {
                if(grid[row][col] == '#')
                {
                    stop = {--row, col};
                    break;
                }
                else if(grid[row][col] != 'X')
                {
                    grid[row][col] = 'X';
                    ++steps;
                }
            }
            break;
        case Direction::Left:
            for(auto[row,col] = start; col >= 0; --col)
            {
                if(grid[row][col] == '#')
                {
                    stop = {row, ++col};
                    break;
                }
                else if(grid[row][col] != 'X')
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
int main() {

    std::vector<std::string> lines;
    {
        std::fstream input("input.txt", std::ios::in);
        if (!input.is_open()) {
            fmt::print(stderr, "Can't open file.");
            return 1;
        }

        std::string line;
        while (std::getline(input, line)) {
            lines.push_back(line);
        }
    }

    fmt::print("Width: {}\n", lines.at(0).size());
    fmt::print("Height: {}\n", lines.size());

    using range = boost::multi_array_types::index_range;
    boost::multi_array<char, 2> grid{boost::extents[lines.at(0).size()][lines.size()]};

    for (int i = 0; i < lines.size(); ++i) {
        std::ranges::copy(lines[i], grid[i].begin());
    }

    auto location = std::find(multi_array_helper::begin(grid), multi_array_helper::end(grid), '^');
    fmt::print("Start Position: [{},{}] Value: {}\n", location.get_indices()[0], location.get_indices()[1], *location);

    int totalSteps = 0;
    std::tuple<int, int> lastPosition{location.get_indices()[0], location.get_indices()[1]};
    Direction d = Direction::Left;

    auto nextDirection = [](Direction& d){
        switch(d)
        {
            case Direction::Up: return d = Direction::Right;
            case Direction::Right:return d = Direction::Down;
            case Direction::Down:return d = Direction::Left;
            case Direction::Left:return d = Direction::Up;
        }
        abort();
    };

    do
    {
        auto [stop, steps] = walkForward(grid, lastPosition, nextDirection(d));
        lastPosition = stop;
        totalSteps += steps;

    }while(lastPosition != std::tuple<int,int>{-1,-1});

    fmt::print("Total Unique Steps: {}\n", totalSteps);

    return 0;
}