#include <iostream>
#include <algorithm>
#include <fstream>
#include <ranges>
#include <sstream>

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

    std::vector<int> team1;
    std::vector<int> team2;
    int x, y;
    while (data >> x >> y)
    {
        team1.push_back(x);
        team2.push_back(y);
    }

    std::ranges::sort(team1);
    std::ranges::sort(team2);

    int total = 0;
    for (int i = 0; i < team1.size() && i < team2.size(); ++i)
    {
        total += std::abs(team1[i] - team2[i]);
    }
    std::cout << "Difference Total: " << total << "\n";

    long similarity = 0;
    for (auto id : team1)
    {
        similarity += std::ranges::count(team2, id) * id;
    }

    std::cout << "Similarity: " << similarity << "\n";

    return 0;
}
