#ifndef __MARKOVBOT_PRODUCER_H
#define __MARKOVBOT_PRODUCER_H

#include "Utility.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace MarkovBot
{
	/*
	 * Produces the markov data structure for use by the consumer.
	 */
	class Producer
	{
	public:
		Producer();
		~Producer();

		std::map<std::string, std::vector<std::string>> generate_markov(std::string output_name, std::vector<std::string> file_names, int token_count);
	private:
		std::vector<std::string> words;

		void parse_file(std::string file_name);

		void create_markov_graph(std::map<std::string, std::vector<std::string>> &graph, int token_count);
		std::vector<std::string> tokenize_words(int token_count);

		void transform_text();
		void remove_unwanted_characters();
		void to_lower();
	};
}

#endif