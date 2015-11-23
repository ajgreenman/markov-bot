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
		Consumer(markov graph);
		markov get_graph();
		void swap(markov graph);
		std::string generate_text(int phrases, int count) const;
	private:
		markov markov_graph;
		std::string find_starting_place() const;
		std::string find_new_key(std::string current_key, std::string new_word) const;
	};
}

#endif