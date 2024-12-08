#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>

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

    boost::regex e(R"(mul\((\d{1,3}),(\d{1,3})\)|don't\(\)|do\(\))");

    std::string input;
    unsigned long result = 0;
    bool doing = true;
    while (std::getline(data, input))
    {
        boost::sregex_iterator itr(input.begin(), input.end(), e);

        std::for_each(itr, boost::sregex_iterator{}, [&result, &doing](boost::smatch const &r)
        {
            if (r[0] == "don't()")
            {
                doing = false;
            }
            else if (r[0] == "do()")
            {
                doing = true;
            }
            else if (doing)
            {
                result += std::stoul(r[1]) * std::stoul(r[2]);
            }
        });
    }
    std::cout << result << "\n";
    return 0;
}