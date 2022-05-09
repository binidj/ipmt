// ipmt.cpp : Defines the entry point for the application.
//

#include "ipmt.h"
#include "Text.h"
#include "BenchmarkTimer.h"
#include "SuffixArray.h"
#include "Huffman.h"
#include <getopt.h>
#include <map>
#include <memory>
#include <functional>
#include <vector>

void PrintUsage()
{
	fprintf(stderr, "\n--------PMT--------\n");
	fprintf(stderr, "-------IF767-------\n\n");
	fprintf(stderr, "Usage: pmt [options] pattern textfile [textfile...]\n");
	fprintf(stderr, "options - check \'pmt --help\' for more information\n");
	fprintf(stderr, "pattern - pattern to be searched\n");
	fprintf(stderr, "textfile - text file path.\n");
	fprintf(stderr, "More than one text file can be processed with multiple arguments or wildcards\n");
}

void PrintHelp()
{
	printf("--ADDITIONAL OPTIONS--\n");
	printf("-e | --edit emax - Search all ocurrences of the specified pattern with a max distance emax\n");
	printf("-p | --pattern patternfile - Specifies the file from which patterns will be read and used on the search\n");
	printf("-a | --algorithm algorithm_name - Specifies the algorith that will be used on the pattern search\n");
	printf("algorithm_name = sliding_window | kmp | boyer_moore | sellers | wu_manber | aho_corasick\n");
	printf("-c | --count - Prints the number of ocurrences of the specified pattern on all the text files.\n");
	printf("-n | --noacopt - disable optmization for large number of occurences when using aho_corasick algoritm\n");
	printf("-h | --help - Basic PMT options\n");
	printf("----------------------\n");
}

char PatternFile[128] = "";
char PatternArg[128] = "";
bool PrintCount = false;
bool Help = false;

int Index(int argc, char** argv);
int Search(int argc, char** argv);
int Zip(int argc, char** argv);
int Unzip(int argc, char** argv);

int main(int argc, char** argv)
{
	static int Option = -1;

	static int OptionIndex = 0;

	static struct option LongOptions[] =
	{
		{"count", no_argument, 0, 'c'},
		{"help", no_argument, 0, 'h'},
		{"pattern", required_argument, 0, 'p'}
	};
	
	while ((Option = getopt_long(argc, argv, "chp:", LongOptions, &OptionIndex)) != -1)
	{
		switch (Option)
		{
			case 'p':
				strcpy(PatternFile, optarg);
				break;

			case 'c':
				PrintCount = true;
				break;
			
			case 'h':
				Help = true;
				break;

			case '?':
				PrintUsage();
				return 1;
				break;

			default:
				printf("asdjashdjash\n");
				abort();
				break;
		}	
	}
	
	if (Help)
	{
		PrintHelp();
		return 0;
	}

	if (optind == argc)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}
	
	static const std::map<std::string, std::function<int(int, char**)>> AppModes = 
	{
		{"index", Index},
		{"search", Search},
		{"zip", Zip},
		{"unzip", Unzip}
	};

	std::function<int(int, char**)> mode = nullptr;
	
	if (AppModes.count(argv[optind]) == 0)
	{
		fprintf(stderr, "Unsuported mode %s\n", argv[optind]);
		PrintUsage();
		return 1;
	}

	mode = AppModes.at(argv[optind]);
	optind += 1;

	return mode(argc, argv);
}

int Index(int argc, char** argv)
{
	printf("Index mode\n");
}

int Search(int argc, char** argv)
{
	printf("Search mode\n");
	return 0;
}

int Zip(int argc, char** argv)
{
	printf("Zip mode\n");
	if (optind == argc)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}

	const std::string fileName = argv[optind];
	std::string outputFile = fileName + ".myz";

	Huffman::Compress(fileName, outputFile);
	
	return 0;
}

int Unzip(int argc, char** argv)
{
	printf("Unzip mode\n");
	if (optind == argc)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}

	const std::string fileName = argv[optind];
	
	if (fileName.size() < 5 || fileName.substr(fileName.size() - 4) != ".myz")
	{
		fprintf(stderr, "Your file must be a .myz\n");
		PrintUsage();
		return 1;
	}

	const std::string outputFile = fileName.substr(0, fileName.size() - 4) + "_test.txt";
	
	Huffman::Decompress(fileName, outputFile);
	
	return 0;
}