#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <limits.h>

static const int maxLog = 32;

class SuffixArray 
{
private:
    static int prevP;
    static std::vector<int> P[maxLog];
    static std::vector<int> suffixArray;
    static void Build_P(std::string &text);
public:
    static void Index(const std::string &inputFile, const std::string &outputFile);
    static void Search(const std::string &inputFile, const std::string &pattern);
};