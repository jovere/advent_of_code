#include <vector>
#include <fstream>
#include <utility>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/algorithm/string.hpp>

struct Coordinates
{
    long x;
    long y;
    long z;
};

template <>
struct fmt::formatter<Coordinates>
{
    static constexpr auto parse(format_parse_context& f)
    {
        return f.begin();
    }

    template <typename ParseContext>
    auto format(const Coordinates& p, ParseContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({},{},{})", p.x, p.y, p.z);
    }
};

int
main()
{
    auto const coordinates = []() -> std::vector<Coordinates>
    {
        std::vector<Coordinates> coordinates;
        std::fstream input("input.txt", std::ios::in);
        if (!input.is_open())
        {
            fmt::print(stderr, "Can't open file.");
            return {};
        }
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(","));
            coordinates.emplace_back(std::stol(tokens[0]), std::stol(tokens[1]), std::stol(tokens[2]));
        }
        return coordinates;
    }();

    fmt::print("coordinates = {}\n", coordinates);

    auto distance = [](Coordinates const& a, Coordinates const& b)
    {
        auto x = a.x - b.x;
        auto y = a.y - b.y;
        auto z = a.z - b.z;
        return x * x + y * y + z * z;
    };

    // Make a coordinate distance lookup table
    using Connection = std::tuple<long, size_t, size_t>;
    std::vector<Connection> distanceTable;
    for (int i = 0; i < coordinates.size(); ++i)
    {
        for (int j = i + 1; j < coordinates.size(); ++j)
        {
            distanceTable.emplace_back(distance(coordinates[i], coordinates[j]), i, j);
        }
    }

    // Only want the first 1000 connections
    std::ranges::partial_sort(distanceTable, distanceTable.begin() + 1000);
    distanceTable.resize(1000);
    fmt::print("distanceTable = {}\n", distanceTable);

#if 1
    // Now combine the circuits
    std::vector<std::vector<size_t>> circuits;
    int connectionsUsed = 0;
    while (connectionsUsed < distanceTable.size())
    {
        auto IsNotUsed = [](Connection const& c) { return std::get<0>(c) != -1; };

        circuits.emplace_back();
        auto& circuit = circuits.back();
        auto Use = [&circuit, &connectionsUsed](auto itr)
        {
            circuit.push_back(std::get<1>(*itr));
            circuit.push_back(std::get<2>(*itr));
            std::get<0>(*itr) = -1;
            connectionsUsed++;
        };

        auto IsConnectedAndNotUsed = [&IsNotUsed, &circuit](Connection const& c)
        {
            return IsNotUsed(c) && std::ranges::any_of(circuit, [&c](auto const& junction)
            {
                return junction == std::get<1>(c)
                    || junction == std::get<2>(c);
            });
        };

        auto firstNotUsed = std::ranges::find_if(distanceTable, IsNotUsed);
        if (firstNotUsed != distanceTable.end())
        {
            Use(firstNotUsed);
            for (auto itr = std::find_if(firstNotUsed + 1, distanceTable.end(), IsConnectedAndNotUsed);
                 itr != distanceTable.end();
                 itr = std::find_if(firstNotUsed + 1, distanceTable.end(), IsConnectedAndNotUsed))
            {
                Use(itr);
            }
        }
    }

    for (auto& circuit : circuits)
    {
        std::ranges::sort(circuit);
        circuit.erase(std::ranges::unique(circuit).begin(), circuit.end());
    }

    std::ranges::sort(circuits, [](auto const& a, auto const& b) { return a.size() > b.size(); });

    for (auto circuit : circuits)
    {
        fmt::print("size = {}, circuit = {}\n", circuit.size(), circuit);
    }

    fmt::print("Mult 3: {}", circuits[0].size() * circuits[1].size() * circuits[2].size());
#else
    // Now combine the circuits
    std::vector<std::vector<Connection>> circuits;
    int connectionsUsed = 0;
    while (connectionsUsed < distanceTable.size())
    {
        auto IsNotUsed = [](Connection const& c) { return std::get<0>(c) != -1; };

        circuits.emplace_back();
        auto& circuit = circuits.back();
        auto firstNotUsed = std::ranges::find_if(distanceTable, IsNotUsed);

        auto Use = [&circuit, &connectionsUsed](auto itr)
        {
            circuit.push_back(*itr);
            std::get<0>(*itr) = -1;
            connectionsUsed++;
        };

        if (firstNotUsed != distanceTable.end())
        {
            Use(firstNotUsed);
            for (auto itr = std::find_if(firstNotUsed + 1, distanceTable.end(), IsNotUsed);
                 itr != distanceTable.end();
                 itr = std::find_if(itr + 1, distanceTable.end(), IsNotUsed))
            {
                if (std::ranges::any_of(circuit, [itr](Connection const& innerConnection)
                {
                    return std::get<1>(innerConnection) == std::get<1>(*itr)
                        || std::get<1>(innerConnection) == std::get<2>(*itr)
                        || std::get<2>(innerConnection) == std::get<1>(*itr)
                        || std::get<2>(innerConnection) == std::get<2>(*itr);
                }))
                {
                    Use(itr);
                }
            }
        }
    }

    std::ranges::sort(circuits, [](auto const& a, auto const& b) { return a.size() > b.size(); });

    for (auto circuit : circuits)
    {
        fmt::print("size = {}, circuit = {}\n", circuit.size(), circuit);
    }

    fmt::print("Mult 3: {}", circuits[0].size() * circuits[1].size() * circuits[2].size());
#endif

    return 0;
}
