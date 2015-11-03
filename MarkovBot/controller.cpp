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
	Consumer c = Consumer::Consumer();

	return 0;
}