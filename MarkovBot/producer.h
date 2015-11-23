#ifndef __MARKOVBOT_PRODUCER_H
#define __MARKOVBOT_PRODUCER_H

#include "Utility.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
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

		markov generate_markov_graph(std::vector<std::string> w, int token_count, markov graph);
		markov generate_markov(std::string output_name, std::vector<std::string> file_names, int token_count);
		std::vector<std::string> parse_file(std::string file_name) const;
	private:
		std::vector<std::string> words;

		void parse_file_version1(std::string file_name);

		void create_markov_graph(markov &graph, int token_count);
		void create_markov_graph_old(markov &graph, int token_count);
		std::vector<std::string> tokenize_words(int token_count);

		void transform_text();
		void remove_unwanted_characters();
		void to_lower();
		std::string rtrim(std::string &s);
	};
}

#endif