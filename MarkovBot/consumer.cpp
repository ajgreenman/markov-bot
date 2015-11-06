#include "consumer.h"

using MarkovBot::Consumer;

Consumer::Consumer(std::map<std::string, std::vector<std::string>> graph)
{
	swap(graph);
}


Consumer::~Consumer()
{

}

/*
 * Swaps in a new markov graph, replacing the old one.
 */
void Consumer::swap(std::map<std::string, std::vector<std::string>> graph)
{
	markov_graph = graph;
}

/*
 * Generates pseudo-text from a markov graph.
 */
std::string Consumer::generate_text()
{
	if(markov_graph.begin() == markov_graph.end())
	{
		throw std::exception("Markov graph is empty.");
	}

	srand(time(NULL));
	std::string ret_val;
	std::string key = find_starting_place();

	int i = 0;
	while(i <= 1000)
	{
		ret_val.append(key);
		ret_val.append(" ");
		std::map<std::string, std::vector<std::string>>::iterator it = markov_graph.find(key);
		std::size_t value_length = it->second.size();

		int value_index = rand() % value_length;
		key = it->second[value_index];

		i++;
	}

	return ret_val;
}

/*
 * Finds a random starting point in the markov graph.
 */
std::string Consumer::find_starting_place()
{
	std::size_t graph_sz = markov_graph.size();
	int start = rand() % graph_sz;

	std::map<std::string, std::vector<std::string>>::iterator it = markov_graph.begin();
	for(int i = 0; i < start; i++)
	{
		it++;
	}

	return it->first;
}