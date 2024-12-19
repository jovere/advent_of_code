#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/tokenizer.hpp>
#include <boost/multi_array.hpp>

using Robot = std::tuple<std::tuple<int, int>, std::tuple<int, int>>;
static constexpr int systemWidth = 101;
static constexpr int systemHeight = 103;

void print(std::vector<Robot> const &robots) {
    for (auto &[p, v]: robots) {
        auto [px, py] = p;
        auto [vx, vy] = v;
        fmt::print("{{({:#2},{:#2}),({:#2},{:#2})}}, ", px, py, vx, vy);
    }
    fmt::print("\n");
}

void
printGrid(boost::multi_array<char, 2> const &grid) {
    for (auto row: grid) {
        for (auto column: row) {
            fmt::print("{}", column);
        }
        fmt::print("\n");
    }
}

static boost::multi_array<char, 2> grid(boost::extents[systemWidth][systemHeight]);

void printGrid(std::vector<Robot> const &robots) {
    for (auto &[p, _]: robots) {
        auto [px, py] = p;
        grid[px][py] = 'X';
    }
    printGrid(grid);
    for (auto &[p, _]: robots) {
        auto [px, py] = p;
        grid[px][py] = ' ';
    }
}

int
main() {
    std::fill_n(grid.origin(), grid.max_size(), ' ');
    std::fstream input("input.txt", std::ios::in);
    if (!input.is_open()) {
        fmt::print(stderr, "Can't open file.");
        return 1;
    }

    std::vector<Robot> robots;
    {    // Get all the data
        using tokenizer = boost::tokenizer<boost::char_separator<char>>;
        boost::char_separator<char> separator("p=,v ");
        std::string line;
        while (std::getline(input, line)) {
            tokenizer tokens(line, separator);
            auto itr = tokens.begin();
            robots.emplace_back(std::tuple{std::tuple{std::stol(*itr), std::stol(*(++itr))},
                                           std::tuple{std::stol(*(++itr)), std::stol(*(++itr))}});
        }
    }

    fmt::print("Before: ");
    print(robots);

    auto beforeRobots = robots;

    int safety = 0;
    std::array<int, 4> quads{};
    // Through chance found that it's cyclical and 10403 is the cycle
    for (int seconds = 1; seconds <= 10403; ++seconds) {
        std::transform(robots.begin(), robots.end(), robots.begin(), [](auto &robot) {
            auto [p, v] = robot;
            auto [px, py] = p;
            auto const [vx, vy] = v;

            px = (px + vx) % systemWidth;
            if (px < 0) px += systemWidth;
            py = (py + vy) % systemHeight;
            if (py < 0) py += systemHeight;

            return std::tuple{std::tuple{px, py}, std::tuple{vx, vy}};
        });
        if (std::count_if(robots.begin(), robots.end(), [](auto robot) {
            auto &[p, _] = robot;
            auto [px, py] = p;
            return (px > 20 & px < 80);
        }) > 400) {
            printGrid(robots);
            fmt::print("Seconds: {}\n\n\n", seconds);
        }

        // Part 1
        if (seconds == 100) {
            auto constexpr lrBounds = systemWidth / 2;
            auto constexpr udBounds = systemHeight / 2;
            for (auto [p, _]: robots) {
                if (auto [x, y] = p; x < lrBounds && y < udBounds) {
                    ++quads[0];
                } else if (x > lrBounds && y < udBounds) {
                    ++quads[1];
                } else if (x < lrBounds && y > udBounds) {
                    ++quads[2];
                } else if (x > lrBounds && y > udBounds) {
                    ++quads[3];
                }
            }

            safety = std::reduce(quads.begin(), quads.end(), 1, [](auto x, auto y) { return x * y; });
        }
    }

    fmt::print("After:  ");
    print(robots);
    printGrid(robots);

    fmt::print("Quadrants: {}, Safety: {}\n", quads, safety);
    fmt::print("Identical? {}", robots == beforeRobots);

    return 0;
}
