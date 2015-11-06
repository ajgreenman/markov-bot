#pragma once

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
		~Consumer(void);
		void swap(std::map<std::string, std::vector<std::string>> graph);
	private:
		std::map<std::string, std::vector<std::string>> markov_graph;
	};
}