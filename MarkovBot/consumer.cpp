#include "consumer.h"

using MarkovBot::Consumer;

Consumer::Consumer(std::map<std::string, std::vector<std::string>> graph)
{
	swap(graph);
}


Consumer::~Consumer()
{

}

void Consumer::swap(std::map<std::string, std::vector<std::string>> graph)
{
	markov_graph = graph;
}

std::string Consumer::generate_text()
{
	srand(time(NULL));
	std::string ret_val;
	std::string key = find_starting_place();

	int i = 0;
	while(i <= 10000)
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