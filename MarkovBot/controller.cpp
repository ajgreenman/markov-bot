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

std::string get_file(std::string type = "");
std::vector<std::string> get_input(const Producer &p);
std::vector<std::string> get_input_files();
std::string get_output_file();

void combine_markov_graphs(Producer &p, Consumer &c, std::vector<std::string> words, int token_count);
void generate_markov_text(const Consumer &c);

int get_int(std::string, int default = 1);

void write_output_to_file(std::string output, std::string output_name);

/*
 * Determines what version of the Bot to run, then runs it. Main entry point of the program.
 */
int main()
{
	std::cout << "Welcome to my Markov Generator." << std::endl << "Press (1) to do version 1.0 of the Bot, or anything else to do version 2.0." << std::endl;
	int input = _getch();

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

/*
 * Performs version one of the MarkovBot. This version takes in a list of files, gives them to the Producer to create a markov graph,
 * and then automatically feeds this graph into the Consumer to generate text.
 */
int version_one()
{
	markov graph;
	std::vector<std::string> input_files = get_input_files();
	std::string output_file = get_output_file();

	int token_count = get_int("Enter the amount of tokens per phrase: ");

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

/*
 * Performs version two of the MarkovBot. This version was designed to add numerous efficiencies to the previous version. These include
 * the ability to generate text based off of an already-made graph, the ability to use as many files or graphs as you like, 
 */
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

/*
 * Creates a markov graph from an existing markov file and feeds this to the Consumer to generate text. The Producer is not required.
 */
int generate_off_markov()
{
	markov graph;
	std::string file;

	bool done_parsing = false;
	while(!done_parsing) // This will loop until a graph is parsed or the user quits. Exceptions will keep it looping.
	{
		file = get_file("markov ");
		if(file == "q")
		{
			return 0;
		}

		try
		{
			MarkovBot::MarkovUtility::parse_markov_file(file, graph);
			done_parsing = true;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	Consumer c = Consumer::Consumer(graph);
	generate_markov_text(c);

	return 0;
}

/*
 * Generates a new markov graph from a file, and gives it to the Consumer to generate text.
 * This version two method has an additional feature: You can do multiple "passes" which will
 * causes the program to generate new markov graphs based on generated text, which results in
 * more readable text. Entering a high number of passes will bog the system down.
 */
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

	int passes = get_int("Enter the number of generations: ");
	int token_count = get_int("Enter the amount of tokens per phrase: ");

	std::string output, answer;
	std::vector<std::string> w;
	for(int i = 0; i < passes; ++i)
	{
		std::cout << "Starting generation " << i + 1 << "..." << std::endl;
		combine_markov_graphs(p, c, words, token_count);
		output = c.generate_text(OUTPUT_PARAMETERS, OUTPUT_PARAMETERS);
		w = MarkovBot::MarkovUtility::split_string_to_vector(output);
		words.insert(words.begin(), w.begin(), w.end());
	}

	std::cout << "All generations completed. Starting final run-through..." << std::endl;
	combine_markov_graphs(p, c, words, token_count);

	MarkovBot::MarkovUtility::write_markov_file(MarkovBot::MarkovUtility::get_output_name(output_file), c.get_graph());

	generate_markov_text(c);

	return 0;
}

/*
 * Asks the user for a file and then parses it into a vector of words. 
 */
std::vector<std::string> get_input(const Producer &p)
{
	bool done_parsing = false, no_more_files = false;
	
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
					if (file == "q")
					{
						ret.clear();
						return ret;
					}
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

/*
 * Gets a single file of the requested type.
 */
std::string get_file(std::string type)
{
	std::cout << "Enter a " << type << "file to parse: " << std::endl;
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
 * Gets an int value from the user. If the user does not provide a valid value, it returns the default value.
 */
int get_int(std::string prompt, int default)
{
	int value;
	std::cout << prompt << std::endl;
	std::cin >> value;

	if(std::cin.fail())
	{
		value = default;
	}

	return value;
}

/*
 * Combines an existing markov graph with one created from a new vector of words.
 */
void combine_markov_graphs(Producer &p, Consumer &c,std::vector<std::string> words, int token_count)
{
	c.set_graph(p.combine_markov_graphs(words, token_count, c.get_graph()));
}

/*
 * Gets the parameters required for the Consumer to generate text, then generates and writes that text to a file.
 */
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

/*
 * Writes the output to a text file.
 */
void write_output_to_file(std::string output, std::string output_file)
{
	// This chunk of code ensures that we write to a .txt file.
	std::size_t pos = output_file.find(".");
	std::string output_name = output_file.substr(0, pos);
	output_name.append(".txt");
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