#ifndef __MARKOVBOT_UTILITY_MARKOV_H
#define __MARKOVBOT_UTILITY_MARKOV_H

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef std::map<std::string, std::vector<std::string>> markov;

namespace MarkovBot
{
	/*
	* Provides utility methods related to markov files and markov graphs.
	*/
	class MarkovUtility
	{
	public:
		static void parse_markov_file(std::string markov_file, markov &graph);
		static void write_markov_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph);

		static void combine_graphs(markov &a, const markov &b);
		static void print_graph(markov graph);

		static std::vector<std::string> split_string_to_vector(std::string s);
		
		static bool is_markov(std::string file_name);
		static std::string get_output_name(std::string output_name);
	};
}

#endif