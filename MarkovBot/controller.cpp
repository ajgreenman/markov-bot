#include <algorithm>
#include <conio.h>
#include <string>
#include <vector>
#include "producer.h"
#include "consumer.h"

#define KEY_ONE 49
#define KEY_Q 113
#define KEY_Y 121
#define KEY_N 110
#define KEY_M 109

#define OUTPUT_PARAMETERS 500

using MarkovBot::Consumer;
using MarkovBot::Producer;

int version_one();
int version_two();
int generate_off_markov();
int generate_off_text();

std::string get_file(std::string modifier = "");
std::vector<std::string> get_input(const Producer &p);
std::vector<std::string> get_input_files();
std::string get_output_file();

void generate_markov_graph(Producer &p, Consumer &c,std::vector<std::string> words);
void generate_markov_text(const Consumer &c);

int get_int(std::string);

void write_output_to_file(std::string output, std::string output_name);

/*
 * Responsible for controlling the interactions between I/O and the producer and consumer classes.
 */
int main()
{
	std::cout << "Welcome to my Markov Generator." << std::endl << "Press (1) to do version 1.0 of the Bot, or anything else to do version 2.0." << std::endl;
	int input = _getch();
	//std::cout << input;

	if(input == KEY_ONE)
	{
		if(version_one() != 0)
		{
			return 1;
		}
	}
	else
	{
		if(version_two() != 0)
		{
			return 1;
		}
	}

	std::cout << std::endl << "Done!" << std::endl;

	return 0;
}

int version_one()
{
	std::vector<std::string> input_files = get_input_files();
	std::string output_file = get_output_file();
	int token_count = get_int("Enter the amount of tokens per phrase: ");

	markov graph;

	try
	{
		Producer p = Producer::Producer();
		graph = p.generate_markov(output_file, input_files, token_count);
		Consumer c = Consumer::Consumer(graph);
		int phrases, count;
		std::string output, answer;
		bool more = true;
		while(more)
		{
			phrases = get_int("Enter the amount of phrases per output: ");
			count = get_int("Enter the amount of outputs: ");
			output = c.generate_text(phrases, count);

			std::cout << "Write output to a file? (y/n)" << std::endl;
			std::cin >> answer;
			if(answer == "y")
			{
				std::cout << "Enter output file name: " << std::endl;
				std::cin >> answer;
				write_output_to_file(output, answer);
			}

			std::cout << "Produce more output? (y/n)" << std::endl;
			std::cin >> answer;
			if(answer != "y")
			{
				more = false;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

int version_two()
{
	std::cout << std::endl << "Welcome to Version 2.0 of my Markov Generator. Press (q) at any time to quit." << std::endl;
	std::cout << "Press (m) to use a Markov graph, or anything else to upload a text file." << std::endl;

	int input = _getch();

	if(input == KEY_M)
	{
		return generate_off_markov();
	}
	else
	{
		return generate_off_text();
	}
}

int generate_off_markov()
{
	markov graph;
	std::string file;

	bool done_parsing = false;
	while(!done_parsing)
	{
		file = get_file("markov ");
		if(file == "q")
		{
			return 0;
		}

		try
		{
			MarkovBot::Utility::parse_markov_file(file, graph);
			done_parsing = true;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	Producer p = Producer::Producer();
	Consumer c = Consumer::Consumer(graph);
	generate_markov_text(c);

	return 0;
}

int generate_off_text()
{
	markov graph;
	Producer p = Producer::Producer();
	Consumer c = Consumer::Consumer(graph);

	std::vector<std::string> words = get_input(p);
	if(words.empty())
	{
		return 0;
	}

	std::string output_file = get_output_file();
	if(output_file == "q")
	{
		return 0;
	}

	int token_count, passes = get_int("Enter the number of generations: ");

	std::string output, answer;
	std::vector<std::string> w;
	
	for(int i = 0; i < passes; ++i)
	{
		generate_markov_graph(p, c, words);
		output = c.generate_text(OUTPUT_PARAMETERS, OUTPUT_PARAMETERS);
		w = MarkovBot::Utility::split_string_to_vector(output);
		words.insert(words.begin(), w.begin(), w.end());
	}

	std::cout << "All generations completed. Starting final run-through..." << std::endl;
	generate_markov_graph(p, c, words);

	MarkovBot::Utility::write_markov_file(MarkovBot::Utility::get_output_name(output_file), c.get_graph());

	generate_markov_text(c);
}

std::vector<std::string> get_input(const Producer &p)
{
	bool done_parsing, no_more_files = false;
	
	std::string file;
	std::vector<std::string> w, ret;

	do
	{
		file = get_file();
		if(file == "q")
		{
			ret.clear();
			return ret;
		}

		done_parsing = false;
		do
		{
			try
			{
				w = p.parse_file(file);
				done_parsing = true;
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;

				std::cout << "Try again? (y/n)" << std::endl;
				int input = tolower(_getch());
				switch(input)
				{
				case KEY_Q:
					ret.clear();
					return ret;
				case KEY_Y:
					file = get_file();
					break;
				case KEY_N:
				default:
					done_parsing = true;
				}
			}

		} while(!done_parsing);
		
		ret.insert(ret.end(), w.begin(), w.end());

		std::cout << "Add more files? (y/n)" << std::endl;
		int input = tolower(_getch());
		switch(input)
		{
		case KEY_Q:
			ret.clear();
			return ret;
		case KEY_Y:
			break;
		case KEY_N:
		default:
			no_more_files = true;
		}

	} while(!no_more_files);

	return ret;
}

std::string get_file(std::string modifier)
{
	std::cout << "Enter a " << modifier << "file to parse: " << std::endl;
	std::string file;
	std::cin >> file;

	return file;
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

/*
 * Gets an int value from the user.
 */
int get_int(std::string prompt)
{
	int value;
	std::cout << prompt << std::endl;
	std::cin >> value;

	if(std::cin.fail())
	{
		value = 1;
	}

	return value;
}

void generate_markov_graph(Producer &p, Consumer &c,std::vector<std::string> words)
{
	int token_count = get_int("Enter the amount of tokens per phrase: ");
	
	c.swap(p.generate_markov_graph(words, token_count, c.get_graph()));
}

void generate_markov_text(const Consumer &c)
{
	int phrases, count;
	std::string output, answer;

	phrases = get_int("Enter the amount of phrases per output: ");
	count = get_int("Enter the amount of outputs: ");
	output = c.generate_text(phrases, count);

	std::cout << "Enter output file name: " << std::endl;
	std::cin >> answer;
	write_output_to_file(output, answer);
}

void write_output_to_file(std::string output, std::string output_name)
{
	std::ofstream ofs(output_name);

	if(!ofs.is_open())
	{
		throw std::exception("Unable to write file.");
	}

	std::cout << "Writing output to " << output_name << "..." << std::endl;

	ofs << output;

	std::cout << "Finished writing to " << output_name << "." << std::endl;
	ofs.close();
}