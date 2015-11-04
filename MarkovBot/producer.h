#ifndef __MARKOVBOT_PRODUCER_H
#define __MARKOVBOT_PRODUCER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace MarkovBot
{
	/*
	 * Produces the Markov data structure for use by the Consumer.
	 */
	class Producer
	{
	public:
		Producer(void);
		~Producer(void);
		std::string generate_markov(std::string output_name, std::string file_name);
	private:
		std::vector<std::string> words;
		bool parse_file(std::string file_name, std::vector<std::string> &w);
		bool write_file(std::string output_name, const std::vector<std::string> &w);
	};
}

#endif