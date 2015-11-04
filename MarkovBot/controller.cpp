#include "producer.h"
#include "consumer.h"

using MarkovBot::Consumer;
using MarkovBot::Producer;

/*
 * Responsible for controlling the interactions between I/O and the Producer and Consumer classes.
 */
int main()
{
	Producer p = Producer::Producer();
	std::string markov_file = p.generate_markov("test.txt", "test_data.txt");

	return 0;
}