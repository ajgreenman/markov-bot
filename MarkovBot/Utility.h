#ifndef __MARKOVBOT_UTILITY_H
#define __MARKOVBOT_UTILITY_H

#include <exception>
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
		static void parse_markov_file(std::string markov_file, std::map<std::string, std::vector<std::string>> &graph);
		static void write_markov_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph);

		static void combine_graphs(std::map<std::string, std::vector<std::string>> &a,
								   const std::map<std::string, std::vector<std::string>> &b);
		static void print_graph(std::map<std::string, std::vector<std::string>> graph);

		static std::vector<std::string> Utility::split_string_to_vector(std::string s);

		static bool is_markov(std::string file_name);
		static std::string get_output_name(std::string output_name);
	};
}

#endif