#include "producer.h"

using MarkovBot::Producer;

Producer::Producer(void)
	: words()
{

}

Producer::~Producer(void)
{

}

/*
 * Parses the input into a markov graph and writes it to a file.
 */
std::string Producer::generate_markov(std::string output_name, std::string file_name)
{
	if(!parse_file(file_name, words))
	{
		std::cout << "Unable to read " << file_name << "." << std::endl;
		return "";
	}

	output_name = get_output_name(output_name);

	if(create_markov_graph(words, output_name))
	{
		return output_name;
	}

	std::cout << "Unable to write to " << output_name << "." << std::endl;

	return "";
}

/*
 * Parses the input file into an array of individual words.
 */
bool Producer::parse_file(std::string file_name, std::vector<std::string> &w)
{
	std::ifstream ifs;
	ifs.open(file_name);
	if(!ifs.good())
	{
		return false;
	}

	// Read entire contents of the file into a single string.
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	// Split the string up into words and add it to the word vector.
	std::istringstream iss(content);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(w));

	return true;
}

/*
 * Transforms an array of words into a markov graph.
 */
bool Producer::create_markov_graph(std::vector<std::string> w, std::string output_name)
{
	std::map<std::string, std::vector<std::string>> graph;

	for(int i = 0; i < w.size() - 1; ++i)
	{
		if(graph.find(w[i]) == graph.end())
		{
			graph[w[i]] = std::vector<std::string>();
		}
		
		graph[w[i]].push_back(w[i + 1]);
	}

	return write_file(output_name, graph);
}

/*
 * Writes a markov graph to a file.
 */
bool Producer::write_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph)
{
	std::ofstream ofs(output_name);

	if(!ofs.is_open())
	{
		return false;
	}

	for(auto &words : graph)
	{
		ofs << words.first << ":";
		for(auto &word : words.second)
		{
			ofs << word << " ";
		}
		ofs << std::endl;
	}

	ofs.close();
	return true;
}

/*
 * Strips out file extensions and appends the markov file extension.
 */ 
std::string Producer::get_output_name(std::string output_name)
{
	std::size_t pos = output_name.find(".");

	std::string ret_val = output_name.substr(0, pos);
	ret_val.append(".markov");

	return ret_val;
}