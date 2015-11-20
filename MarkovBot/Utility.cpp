#include "Utility.h"

using MarkovBot::Utility;

/*
 * Parses a .markov file into a markov graph.
 */
void Utility::parse_markov_file(std::string markov_file, std::map<std::string, std::vector<std::string>> &graph)
{
	if(!is_markov(markov_file))
	{
		throw std::exception("Could not parse file because it was not a .markov file.");
	}

	std::ifstream ifs;
	ifs.open(markov_file);
	if(!ifs.good())
	{
		throw std::exception("Could not find file.");
	}

	std::cout << "Parsing " << markov_file << "..." << std::endl;

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

	std::cout << "Done parsing. Markov graph created." << std::endl;
}

/*
 * Writes a markov graph to a file.
 */
void Utility::write_markov_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph)
{
	std::ofstream ofs(output_name);

	if(!ofs.is_open())
	{
		throw std::exception("Unable to write file.");
	}

	std::cout << "Writing final markov graph to " << output_name << "..." << std::endl;

	for(auto &words : graph)
	{
		ofs << words.first << "|";
		for(auto &word : words.second)
		{
			ofs << word << " ";
		}
		ofs << std::endl;
	}

	std::cout << "Finished writing to " << output_name << "." << std::endl;
	ofs.close();
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
	if(pos == std::string::npos)
	{
		return false;
	}

	std::string file_extension = file_name.substr(pos);

	return file_extension == ".markov";
}

/*
 * Strips out file extensions and appends the markov file extension.
 */ 
std::string Utility::get_output_name(std::string output_name)
{
	std::size_t pos = output_name.find(".");

	std::string ret_val = output_name.substr(0, pos);
	ret_val.append(".markov");

	return ret_val;
}