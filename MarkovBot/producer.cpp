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
			parse_file(file);

			transform_text();

			create_markov_graph(temp_graph, token_count);
		}

		MarkovBot::Utility::combine_graphs(graph, temp_graph);
	}

	Utility::write_markov_file(Utility::get_output_name(output_name), graph);

	return graph;
}

/*
 * Parses the input file into an array of individual words.
 */
void Producer::parse_file(std::string file_name)
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

	words = MarkovBot::Utility::split_string_to_vector(content);

	std::cout << "Successfully read " << file_name << "." << std::endl;
}

#pragma region Generate Markov Graph

/*
 * Transforms an array of words into a markov graph.
 */
void Producer::create_markov_graph(std::map<std::string, std::vector<std::string>> &graph, int token_count)
{
	std::cout << "Generating markov graph..." << std::endl;

	std::vector<std::string> w = tokenize_words(token_count);

	for(int i = 0; i + token_count < w.size(); ++i)
	{
		if(graph.find(w[i]) == graph.end())
		{
			graph[w[i]] = std::vector<std::string>();
		}

		graph[w[i]].push_back(w[i + token_count]);
	}
	
	std::cout << "Markov graph succesfully created." << std::endl;
}

/*
 * Combines an array of words into an array of phrases.
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
#pragma endregion

#pragma region Transform Text

/*
 * Contains the methods that help modify the text in a way that allows better text generation.
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

	for(auto &word : words)
	{
		for(auto &character : unwanted_characters)
		{
			word.erase(std::remove(word.begin(), word.end(), character), word.end());
		}
	}
}

/*
 * Transforms an array of words into the same array, but with all characters lowercase.
 */
void Producer::to_lower()
{
	for(int i = 0; i < words.size(); ++i)
	{
		for(int j = 0; j < words[i].size(); ++j)
		{
			std::transform(words[i].begin(), words[i].end(), words[i].begin(), ::tolower);
		}
	}
}

#pragma endregion