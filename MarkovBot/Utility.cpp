#include "Utility.h"

using MarkovBot::Utility;

/*
 * Parses a .markov file into a markov graph.
 */
bool Utility::parse_markov_file(std::string markov_file, std::map<std::string, std::vector<std::string>> &graph)
{
	std::size_t pos = markov_file.find(".");
	std::string file_extension = markov_file.substr(pos);

	if(file_extension != ".markov")
	{
		std::cout << "Not a .markov file" << std::endl;
		return false;
	}

	std::ifstream ifs;
	ifs.open(markov_file);
	if(!ifs.good())
	{
		std::cout << "Can't open file." << std::endl;
		return false;
	}

	std::cout << "Parsing .markov file..." << std::endl;

	graph.clear();

	std::string line, key, v;
	std::vector<std::string> value;
	while(std::getline(ifs, line))
	{
		pos = line.find("|");
		key = line.substr(0, pos);
		v = line.substr(pos + 1);
		value = split_string_to_vector(v);

		
		graph[key] = value;
	}

	std::cout << "Done parsing..." << std::endl;

	return true;
}

/*
 * Prints a markov graph for debugging purposes.
 */
 void Utility::print_graph(std::map<std::string, std::vector<std::string>> graph)
{
	std::cout << "Printing markov graph..." << std::endl;

	for(auto &words : graph)
	{
		std::cout << words.first << "|";
		for(auto &word : words.second)
		{
			std::cout << word << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Done printing markov graph..." << std::endl;
}
 
/*
 * Splits a string up into words and adds it to a vector.
 */
std::vector<std::string> Utility::split_string_to_vector(std::string s)
{
	std::vector<std::string> ret_val;

	std::istringstream iss(s);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(ret_val));

	return ret_val;
}