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
	p.generate_markov("test_output.markov", files);

	std::cout << "Done." << std::endl;
	return 0;
}