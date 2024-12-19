#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/tokenizer.hpp>

using Robot = std::tuple<std::tuple<int, int>, std::tuple<int, int>>;

void print(std::vector<Robot> const &robots) {
    for (auto [p, v]: robots) {
        auto [px, py] = p;
        auto [vx, vy] = v;
        fmt::print("{{({:#2},{:#2}),({:#2},{:#2})}}, ", px, py, vx, vy);
    }
    fmt::print("\n");
}

int
main() {
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

    constexpr int systemWidth = 101;
    constexpr int systemHeight = 103;
    for (int x = 0; x < 100; ++x) {
        std::transform(robots.begin(), robots.end(), robots.begin(), [systemHeight, systemWidth](auto &robot) {
            auto [p, v] = robot;
            auto [px, py] = p;
            auto const [vx, vy] = v;

            px = (px + vx) % systemWidth;
            if (px < 0) px += systemWidth;
            py = (py + vy) % systemHeight;
            if (py < 0) py += systemHeight;

            return std::tuple{std::tuple{px, py}, std::tuple{vx, vy}};
        });
    }

    fmt::print("After:  ");
    print(robots);

    std::array<int, 4> quads{};
    auto constexpr lrBounds = systemWidth / 2;
    auto constexpr udBounds = systemHeight / 2;
    for (auto [p, _]: robots) {
        if (auto [x, y] = p; x < lrBounds && y < udBounds) {
            ++quads[0];
        } else if (x > lrBounds && y < udBounds) {
            ++quads[1];
        } else if (x < lrBounds && y > udBounds) {
            ++quads[2];
        } else if (x > lrBounds && y > udBounds){
            ++quads[3];
        }
    }

    auto safety = std::reduce(quads.begin(),quads.end(),1,[](auto x, auto y){ return x*y;});

    fmt::print("Quadrants: {}, Safety: {}", quads, safety);

    return 0;
}
