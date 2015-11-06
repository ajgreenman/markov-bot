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

	Producer p = Producer::Producer();
	std::string markov_file = p.generate_markov("test_output.txt", files);

	return 0;
}