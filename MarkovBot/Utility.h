#ifndef __MARKOVBOT_UTILITY_H
#define __MARKOVBOT_UTILITY_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace MarkovBot
{
	/*
	 * Provides utility methods for use by the producer or consumer.
	 */
	class Utility
	{
	public:
		static bool parse_markov_file(std::string markov_file, std::map<std::string, std::vector<std::string>> &graph);
		static void print_graph(std::map<std::string, std::vector<std::string>> graph);
		static std::vector<std::string> Utility::split_string_to_vector(std::string s);
	};
}

#endif