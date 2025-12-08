#include <vector>
#include <fstream>
#include <utility>
#include <set>
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
    std::deque<Connection> distanceTable;
    for (int i = 0; i < coordinates.size(); ++i)
    {
        for (int j = i + 1; j < coordinates.size(); ++j)
        {
            distanceTable.emplace_back(distance(coordinates[i], coordinates[j]), i, j);
        }
    }

    std::ranges::sort(distanceTable);
    fmt::print("distanceTable = {}\n", distanceTable);

    // Now combine the circuits
    std::vector<std::set<size_t>> circuits;
    Connection lastConnection= distanceTable.front();
    distanceTable.pop_front();

    // Fill the first entry to start the loop
    circuits.emplace_back();
    circuits.back().insert(std::get<1>(lastConnection));
    circuits.back().insert(std::get<2>(lastConnection));

    while (circuits[0].size() < coordinates.size())
    {
        lastConnection = distanceTable.front();
        distanceTable.pop_front();

        auto ConnectedCircuit = [&lastConnection](auto const& circuit)
        {
            return std::ranges::any_of(circuit, [&lastConnection](auto const& junction)
            {
                return junction == std::get<1>(lastConnection)
                    || junction == std::get<2>(lastConnection);
            });
        };

        auto cktItr = std::ranges::find_if(circuits, ConnectedCircuit);

        if (cktItr == circuits.end())
        {
            circuits.emplace_back();
            circuits.back().insert(std::get<1>(lastConnection));
            circuits.back().insert(std::get<2>(lastConnection));
        }
        else
        {
            // Check if there's another circuit with this connection
            auto cktItr2 = std::find_if(cktItr+1, circuits.end(), ConnectedCircuit);
            if (cktItr2 == circuits.end())
            {
                // No additional connection, put in first circuit
                cktItr->insert(std::get<1>(lastConnection));
                cktItr->insert(std::get<2>(lastConnection));
            }
            else
            {
                // Combine circuits
                cktItr->merge(*cktItr2);

                // Delete the second one
                circuits.erase(cktItr2);
            }
        }
    }

    auto coordinate1 = coordinates[std::get<1>(lastConnection)];
    auto coordinate2 = coordinates[std::get<2>(lastConnection)];
    fmt::print("Last Connection: {}, {} ; X*X: {}", coordinate1, coordinate2, coordinate1.x * coordinate2.x);

    return 0;
}
