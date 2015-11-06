#ifndef __MARKOVBOT_PRODUCER_H
#define __MARKOVBOT_PRODUCER_H

#include "Utility.h"
#include <fstream>
#include <iostream>
#include <map>
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
		std::map<std::string, std::vector<std::string>> generate_markov(std::string output_name, std::vector<std::string> file_names);
	private:
		std::vector<std::string> words;
		std::string get_output_name(std::string output_name);
		void parse_file(std::string file_name, std::vector<std::string> &w);
		void create_markov_graph(std::vector<std::string> w, std::map<std::string, std::vector<std::string>> &graph);
		void write_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph);
	};
}

#endif