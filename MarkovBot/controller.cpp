#include <string>
#include <vector>
#include "producer.h"
#include "consumer.h"

using MarkovBot::Consumer;
using MarkovBot::Producer;

/*
 * Responsible for controlling the interactions between I/O and the producer and consumer classes.
 */
int main()
{
	std::vector<std::string> files;
	files.push_back("test_input.txt");

	std::map<std::string, std::vector<std::string>> graph;
	Producer p = Producer::Producer();
	try
	{
		graph = p.generate_markov("test_output.markov", files);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	Consumer c = Consumer::Consumer(graph);

	return 0;
}