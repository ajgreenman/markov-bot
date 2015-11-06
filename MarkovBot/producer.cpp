#include "producer.h"

using MarkovBot::Producer;

Producer::Producer()
	: words()
{

}

Producer::~Producer()
{

}

/*
 * Parses the input into a markov graph and writes it to a file.
 */
std::map<std::string, std::vector<std::string>> Producer::generate_markov(std::string output_name, std::vector<std::string> file_names, int token_count)
{
	std::map<std::string, std::vector<std::string>> graph;
	std::map<std::string, std::vector<std::string>> temp_graph;

	for(auto &file : file_names)
	{
		temp_graph.clear();

		if(MarkovBot::Utility::is_markov(file))
		{
			MarkovBot::Utility::parse_markov_file(file, temp_graph);
		} 
		else
		{
			parse_file(file, words);

			create_markov_graph(words, temp_graph, token_count);
		}

		MarkovBot::Utility::combine_graphs(graph, temp_graph);
	}

	write_file(get_output_name(output_name), graph);

	return graph;
}

/*
 * Parses the input file into an array of individual words.
 */
void Producer::parse_file(std::string file_name, std::vector<std::string> &w)
{
	std::ifstream ifs;
	ifs.open(file_name);
	if(!ifs.good())
	{
		throw std::exception("Could not find file.");
	}
	
	std::cout << "Reading " << file_name << "..." << std::endl;

	// Read entire contents of the file into a single string.
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	w = MarkovBot::Utility::split_string_to_vector(content);

	std::cout << "Successfully read " << file_name << "." << std::endl;
}

/*
 * Transforms an array of words into a markov graph.
 */
void Producer::create_markov_graph(std::vector<std::string> words, std::map<std::string, std::vector<std::string>> &graph, int token_count)
{
	std::cout << "Generating markov graph..." << std::endl;

	std::vector<std::string> w = tokenize_words(words, token_count);

	for(int i = 0; i + 1 < w.size(); ++i)
	{
		if(graph.find(w[i]) == graph.end())
		{
			graph[w[i]] = std::vector<std::string>();
		}

		graph[w[i]].push_back(w[i + 1]);
	}
	
	std::cout << "Markov graph succesfully created." << std::endl;
}

/*
 * Combines an array of words into an array of phrases.
 */
std::vector<std::string> Producer::tokenize_words(std::vector<std::string> &words, int token_count)
{
	std::vector<std::string> tokenized;
	std::string value;
	for(int i = 0; i + token_count < words.size(); i = i + token_count)
	{
		value.clear();
		for(int j = 0; j < token_count; j++)
		{
			value.append(words[i + j]);
			value.append(" ");
		}
		tokenized.push_back(value);
	}

	return tokenized;
}

/*
 * Writes a markov graph to a file.
 */
void Producer::write_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph)
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
 * Strips out file extensions and appends the markov file extension.
 */ 
std::string Producer::get_output_name(std::string output_name)
{
	std::size_t pos = output_name.find(".");

	std::string ret_val = output_name.substr(0, pos);
	ret_val.append(".markov");

	return ret_val;
}