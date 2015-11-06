#include "consumer.h"

using MarkovBot::Consumer;

Consumer::Consumer(std::map<std::string, std::vector<std::string>> graph)
{
	swap(graph);
}


Consumer::~Consumer(void)
{

}

void Consumer::swap(std::map<std::string, std::vector<std::string>> graph)
{
	markov_graph = graph;
}