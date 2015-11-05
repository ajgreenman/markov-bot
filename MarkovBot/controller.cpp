#include "producer.h"
#include "consumer.h"

using MarkovBot::Consumer;
using MarkovBot::Producer;

/*
 * Responsible for controlling the interactions between I/O and the producer and consumer classes.
 */
int main()
{
	Producer p = Producer::Producer();
	std::string markov_file = p.generate_markov("test_output.markov", "test_input.txt");

	return 0;
}