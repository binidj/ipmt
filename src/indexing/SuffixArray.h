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
    static int logSize;
    static int textSize;
    static std::vector<int> buckets[maxLog];
    static std::vector<int> suffixArray;
    static std::vector<int> leftLCP;
    static std::vector<int> rightLCP;
    static std::vector<int> frequency;
    static void BuildBucket(std::string &text);
    static void FillLCPData(int l, int r);
    static int LCP(int leftSuffix, int rightSuffix);
public:
    static void Index(const std::string &inputFile, const std::string &outputFile);
    static void Search(const std::string &inputFile, const std::string &pattern);
};