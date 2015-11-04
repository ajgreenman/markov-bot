#include "producer.h"

using MarkovBot::Producer;

Producer::Producer(void)
	: words()
{

}


Producer::~Producer(void)
{

}


std::string Producer::generate_markov(std::string output_name, std::string file_name)
{
	if(!parse_file(file_name, words))
	{
		std::cout << "Unable to read " << file_name << "." << std::endl;
		return "";
	}

	if(write_file(output_name, words))
	{
		return output_name;
	}

	return "";
}

bool Producer::parse_file(std::string file_name, std::vector<std::string> &w)
{
	std::ifstream ifs;
	ifs.open(file_name);
	if(!ifs.good())
	{
		return false;
	}

	// Read entire contents of the file into a single string.
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	// Split the string up into words and add it to the word vector.
	std::istringstream iss(content);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(w));

	return true;
}

bool Producer::write_file(std::string output_name, const std::vector<std::string> &w)
{
	return true;
}