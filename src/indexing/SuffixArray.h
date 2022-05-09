#pragma once

class SuffixArray 
{
public:
    static void Index(const std::string &inputFile, const std::string &outputFile);
    static void Search(const std::string &inputFile, const std::string &pattern);
};