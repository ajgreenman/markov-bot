#include "consumer.h"

using MarkovBot::Consumer;

/*
 * Constructor.
 */
Consumer::Consumer(markov graph)
{
	set_graph(graph);
}

/*
 * Gets the graph the Consumer is currently using.
 */
markov Consumer::get_graph()
{
	return markov_graph;
}

/*
 * Sets the graph the Consumer is currently using.
 */
void Consumer::set_graph(markov graph)
{
	markov_graph = graph;
}

/*
 * Generates pseudo-text by creating markov chains from the markov graph.
 */
std::string Consumer::generate_text(int phrases, int count) const
{
	if(markov_graph.begin() == markov_graph.end())
	{
		throw std::exception("Markov graph is empty.");
	}

	srand(time(NULL));
	std::string ret_val, key;

	std::cout << "Generating text..." << std::endl;

	for(int i = 0; i < count; ++i)
	{
		key = find_starting_place();
		ret_val.append(key);
		for(int j = 0; j < phrases; ++j)
		{
			markov::const_iterator it = markov_graph.find(key);
			std::size_t value_length = it->second.size();

			int value_index = rand() % value_length;
			ret_val.append(it->second[value_index] + " ");
			key = find_new_key(key, it->second[value_index]);
		}

		ret_val.append("\n\n");
	}

	std::cout << "Generation completed." << std::endl;

	return ret_val;
}

/*
 * Returns a random starting point in the markov graph.
 */
std::string Consumer::find_starting_place() const
{
	std::size_t graph_sz = markov_graph.size();
	int start = rand() % graph_sz;

	markov::const_iterator it = markov_graph.begin();
	for(int i = 0; i < start; i++)
	{
		it++;
	}

	std::string ret_val = it->first;

	return ret_val;
}

/*
 * Finds a new key by removing the first word from the old key and appending the new word to the remainder.
 */
std::string Consumer::find_new_key(std::string current_key, std::string new_word) const
{
	std::string ret_val = current_key.substr(current_key.find_first_of(' ') + 1);
	ret_val.append(new_word + " ");

	return ret_val;
}