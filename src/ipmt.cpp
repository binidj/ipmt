// ipmt.cpp : Defines the entry point for the application.
//

#include "ipmt.h"
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
	fprintf(stderr, "\n--------IPMT--------\n");
	fprintf(stderr, "-------IF767-------\n\n");
	fprintf(stderr, "Usages: \n");
	fprintf(stderr, "ipmt index textfile\n");
	fprintf(stderr, "ipmt search [options] pattern indexfile.idx\n");
	fprintf(stderr, "ipmt zip textfile\n");
	fprintf(stderr, "ipmt unzip textfile.myz\n\n");
	fprintf(stderr, "options - check \'pmt --help\' for more information\n");
	fprintf(stderr, "pattern - pattern to be searched\n");
	fprintf(stderr, "textfile - text file path.\n\n");
}

void PrintHelp()
{
	printf("--ADDITIONAL OPTIONS--\n");
	printf("-p | --pattern patternfile - Specifies the file from which patterns will be read and used on the search\n");
	printf("-c | --count - Prints the number of ocurrences of the specified pattern on all the text files.\n");
	printf("-h | --help - Basic IPMT options\n");
	printf("----------------------\n");
}

char patternBuffer[1024] = "";
char PatternFile[128] = "";
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
		PrintUsage();
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
	if (optind == argc)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}

	const std::string fileName = argv[optind];
	const std::string outputFile = fileName + ".idx";

	return SuffixArray::Index(fileName, outputFile);
}

int Search(int argc, char** argv)
{
	int minArgs = 2;
	std::vector<std::string> patterns; 
	if (strcmp(PatternFile,"") != 0)
	{
		minArgs = 1;
	}

	if (argc-optind < minArgs)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}

	if (minArgs == 2)
	{
		patterns.emplace_back(argv[optind++]);
	}
	else
	{
		std::ifstream patternStream(PatternFile);
		
		if (patternStream.fail())
		{
			fprintf(stderr, "Could not read pattern file %s\n", PatternFile);
			PrintUsage();
			return 1;
		}
		
		while (patternStream.getline(patternBuffer, 1024))
		{
			if (strcmp(patternBuffer, "") != 0)
				patterns.emplace_back(patternBuffer);
		}

		patternStream.close();
	}

	std::string indexFile = argv[optind];
	
	if (indexFile.size() < 5 || indexFile.substr(indexFile.size() - 4) != ".idx")
	{
		fprintf(stderr, "Your index file must be a .idx\n");
		PrintUsage();
		return 1;
	}

	return SuffixArray::Search(indexFile, patterns, PrintCount);
}

int Zip(int argc, char** argv)
{
	if (optind == argc)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}

	const std::string fileName = argv[optind];
	const std::string outputFile = fileName + ".myz";

	return Huffman::Compress(fileName, outputFile);
}

int Unzip(int argc, char** argv)
{
	if (optind == argc)
	{
		fprintf(stderr, "Few arguments\n");
		PrintUsage();
		return 1;
	}

	const std::string fileName = argv[optind];
	
	if (fileName.size() < 5 || fileName.substr(fileName.size() - 4) != ".myz")
	{
		fprintf(stderr, "Your compressed file must be a .myz\n");
		PrintUsage();
		return 1;
	}

	const std::string outputFile = fileName.substr(0, fileName.size() - 4);
	
	return Huffman::Decompress(fileName, outputFile);
}