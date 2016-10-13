#include "producer.h"

using MarkovBot::Producer;

/*
 * Constructor.
 */
Producer::Producer()
	: words()
{

}

/*
 * Deconstructor.
 */
Producer::~Producer()
{

}

/*
 * Parses the input into a markov graph and writes it to a file.
 */
markov Producer::generate_markov(std::string output_name, std::vector<std::string> file_names, int token_count)
{
	markov graph;
	markov temp_graph;

	for(auto &file : file_names)
	{
		temp_graph.clear();

		if(MarkovBot::MarkovUtility::is_markov(file))
		{
			MarkovBot::MarkovUtility::parse_markov_file(file, temp_graph);
		} 
		else
		{
			words = parse_file(file);

			transform_text();

			create_markov_graph(temp_graph, token_count);
		}

		MarkovBot::MarkovUtility::combine_graphs(graph, temp_graph);
	}

	MarkovUtility::write_markov_file(MarkovUtility::get_output_name(output_name), graph);

	return graph;
}

/*
* Parses a vector of words into a new markov graph, then combines it with a passed in graph and returns it.
*/
markov Producer::combine_markov_graphs(std::vector<std::string> w, int token_count, markov temp_graph)
{
	markov graph;

	words = w;

	create_markov_graph(graph, token_count);

	MarkovBot::MarkovUtility::combine_graphs(graph, temp_graph);

	return graph;
}

/*
 * Public-facing method to parse a file's contents and return a vector of individual words.
 * The public nature of the method allows the user to try again until success.
 */
std::vector<std::string> Producer::parse_file(std::string file_name) const
{
	std::vector<std::string> ret;

	std::ifstream ifs;
	ifs.open(file_name);
	if(!ifs.good())
	{
		throw std::exception("Could not find file.");
	}

	std::cout << "Reading " << file_name << "..." << std::endl;

	// Read entire contents of the file into a single string.
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	ret = MarkovBot::MarkovUtility::split_string_to_vector(content);

	std::cout << "Successfully read " << file_name << "." << std::endl;

	return ret;
}

/*
 * Pushes the tokenized vector of words into the markov graph data structure.
 */
void Producer::create_markov_graph(markov &graph, int token_count)
{
	std::cout << "Generating markov graph..." << std::endl;

	std::vector<std::string> w = tokenize_words(token_count);

	for(int i = 0; i + token_count < w.size(); ++i)
	{
		if(graph.find(w[i]) == graph.end())
		{
			graph[w[i]] = std::vector<std::string>();
		}

		graph[w[i]].push_back(words[i + token_count]);
	}

	std::cout << "New markov graph successfully created." << std::endl;
}

/*
 * Combines a vector of words into a vector of phrases.
 */
std::vector<std::string> Producer::tokenize_words(int token_count)
{
	std::vector<std::string> tokenized;
	std::string value;
	for(int i = 0; i + token_count < words.size(); i++)
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
 * Contains the methods that help modify the text in a way that allows better text generation.
 * Note: These methods significantly slow down the program.
 */
void Producer::transform_text()
{
	std::cout << "Transforming text for better generation..." << std::endl;

	remove_unwanted_characters();
	to_lower();
}

/*
 * Removes unwanted characters from a string that might hurt the markov graph generation.
 */
void Producer::remove_unwanted_characters()
{
	std::vector<char> unwanted_characters;
	unwanted_characters.push_back('<');
	unwanted_characters.push_back('>');
	unwanted_characters.push_back('|');
	unwanted_characters.push_back('~');
	unwanted_characters.push_back('`');
	unwanted_characters.push_back('(');
	unwanted_characters.push_back(')');

	for(auto &word : words)
	{
		for(auto &character : unwanted_characters)
		{
			word.erase(std::remove(word.begin(), word.end(), character), word.end());
		}
	}
}

/*
 * Transforms a vector of words into the same vector, but with all characters lowercase.
 */
void Producer::to_lower()
{
	for(int i = 0; i < words.size(); ++i)
	{
		for(auto &c : words[i])
		{
			std::transform(words[i].begin(), words[i].end(), words[i].begin(), ::tolower);
		}
	}
}