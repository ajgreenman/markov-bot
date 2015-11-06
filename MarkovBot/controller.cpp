#include <algorithm>
#include <string>
#include <vector>
#include "producer.h"
#include "consumer.h"

using MarkovBot::Consumer;
using MarkovBot::Producer;

std::vector<std::string> get_input_files();
std::string get_output_file();

/*
 * Responsible for controlling the interactions between I/O and the producer and consumer classes.
 */
int main()
{
	std::vector<std::string> input_files = get_input_files();
	std::string output_file = get_output_file();

	std::map<std::string, std::vector<std::string>> graph;

	try
	{
		Producer p = Producer::Producer();
		graph = p.generate_markov(output_file, input_files);
		Consumer c = Consumer::Consumer(graph);
		std::cout << c.generate_text() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

/*
 * Gets a list of the input files from the user.
 */
std::vector<std::string> get_input_files()
{
	std::cout << "Enter a file to parse: " << std::endl;
	std::string file;
	std::cin >> file;
	
	std::vector<std::string> files;

	while(true)
	{
		files.push_back(file);

		std::cout << "Parse more files? (y/n)" << std::endl;
		std::string input;
		std::cin >> input;

		std::transform(input.begin(), input.end(), input.begin(), ::tolower);
		if(input != "y")
		{
			break;
		}

		std::cout << "Enter another file to parse: " << std::endl;
		std::cin >> file;
	}

	return files;
}

/*
 * Gets the name of the output file from the user.
 */
std::string get_output_file()
{
	std::string file;
	std::cout << "Enter the output file name: " << std::endl;
	std::cin >> file;

	return file;
}