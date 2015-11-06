#ifndef __MARKOVBOT_CONSUMER_H
#define __MARKOVBOT_CONSUMER_H

#include <time.h>
#include "Utility.h"

namespace MarkovBot
{
	/*
	 * Consumes the markov data structure to output "Markovian" text.
	 */
	class Consumer
	{
	public:
		Consumer(std::map<std::string, std::vector<std::string>> graph);
		~Consumer();
		void swap(std::map<std::string, std::vector<std::string>> graph);
		std::string generate_text();
	private:
		std::map<std::string, std::vector<std::string>> markov_graph;
		std::string find_starting_place();
	};
}

#endif