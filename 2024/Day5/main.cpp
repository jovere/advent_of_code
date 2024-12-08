#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

int
main()
{
    std::fstream input("input.txt", std::ios::in);
    if (!input.is_open())
    {
        std::cerr << "Can't open file." << std::endl;
        return 1;
    }

    std::string line;

    // Get all the order rules from the file in a map
    std::multimap<int, int> orderRules;
    while (getline(input, line) && !line.empty())
    {
        std::vector<std::string> result;
        boost::split(result, line, boost::is_any_of("|"));
        orderRules.insert({std::stoi(result[0]), std::stoi(result[1])});
    }

    // Get all the updates from the file
    std::vector<std::vector<int>> updates;
    while (getline(input, line))
    {
        // Split the line
        std::vector<std::string> result;
        boost::split(result, line, boost::is_any_of(","));

        // Put the line in its own vector
        updates.emplace_back();
        auto &update = updates.back();
        update.reserve(result.size());
        for (auto &page : result)
        {
            update.push_back(std::stoi(page));
        }
    }

    int goodSum = 0;
    int fixedBadSum = 0;
    for (auto &pages : updates)
    {
        bool good = true;
        bool startOver = false;
        do
        {
            startOver = false;
            for (long index = 0; index < pages.size() && !startOver; ++index)
            {
                auto results = orderRules.equal_range(pages[index]);
                for (auto itr = results.first; itr != results.second && !startOver; ++itr)
                {
                    auto &item = *itr;
                    if (auto badPage = std::find(pages.begin(), pages.begin() + index, item.second);
                        badPage != pages.begin() + index)
                    {
                        std::swap(pages.at(index), *badPage);
                        good = false;
                        startOver = true;
                    }
                }
            }
        }
        while (startOver);

        if (good)
        {
            goodSum += pages.at(pages.size() / 2);
        }
        else
        {
            fixedBadSum += pages.at(pages.size() / 2);
        }
    }

    std::cout << "Good Sum: " << goodSum << "\n";
    std::cout << "Fixed Bad Sum: " << fixedBadSum << "\n";
}