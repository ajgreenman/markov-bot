#include "consumer.h"

using MarkovBot::Consumer;

Consumer::Consumer(markov graph)
{
	swap(graph);
}

/*
 * Gets the graph the Consumer is currently using.
 */
markov Consumer::get_graph()
{
	return markov_graph;
}

/*
 * Swaps in a new markov graph, replacing the old one.
 */
void Consumer::swap(markov graph)
{
	markov_graph = graph;
}

/*
 * Generates pseudo-text from a markov graph.
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
		for(int j = 0; j < phrases; ++j)
		{
			ret_val.append(key);
			markov::const_iterator it = markov_graph.find(key);
			std::size_t value_length = it->second.size();

			int value_index = rand() % value_length;
			key = it->second[value_index];
		}

		ret_val.append("\n\n");
	}

	std::cout << "Generation completed." << std::endl;

	return ret_val;
}

/*
 * Finds a random starting point in the markov graph.
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

	return it->first;
}