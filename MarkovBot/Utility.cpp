#include "Utility.h"

using MarkovBot::Utility;

/*
 * Parses a .markov file into a markov graph.
 */
bool Utility::parse_markov_file(std::string markov_file, std::map<std::string, std::vector<std::string>> &graph)
{
	if(is_markov(markov_file))
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

	std::size_t pos;
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
 * Combines two markov graphs into one.
 */
void Utility::combine_graphs(std::map<std::string, std::vector<std::string>> &a,
					   const std::map<std::string, std::vector<std::string>> &b)
{
	std::string key;
	std::vector<std::string> value;

	for(auto &key_value : b)
	{
		key = key_value.first;
		value = key_value.second;

		if(a.find(key) == a.end())
		{
			a.insert(key_value);
		}
		else 
		{
			a[key].insert(a[key].end(), value.begin(), value.end());
		}
	}
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

/*
 * Checks the file extension to see if it's a markov file or not.
 */
bool Utility::is_markov(std::string file_name)
{
	std::size_t pos = file_name.find(".");
	std::string file_extension = file_name.substr(pos);

	return file_extension == ".markov";
}