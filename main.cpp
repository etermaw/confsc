#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "string_view.h"
#include "confsc.h"

int main()
{
	dict vars;
	std::unordered_map<string_view, dict> objects;
	
	std::ifstream input("test.txt");
	std::stringstream tmp;

	tmp << input.rdbuf();
	auto str = tmp.str();

	parse(string_view(str.data(), str.length()), vars, objects);

	for(auto& i : vars)
	{
		std::cout << "Name: " << i.first << "\nValue: ";

		for (auto& j : i.second)
			std::cout << j << " ";

		std::cout << "\n";
	}

    std::cout << "\n\nOBJECTS:\n\n";

    for(auto& i : objects)
	{
		std::cout << "Name: " << i.first << "\n";

		for (auto& j : i.second)
		{ 
			std::cout << j.first << ": ";

            for (auto& k : j.second)
			    std::cout << k << " ";
		    
            std::cout << "\n";
		}

		std::cout << "\n";
	}
}
