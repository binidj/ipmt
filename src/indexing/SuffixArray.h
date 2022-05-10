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
    static int bucketIndex;
    static std::vector<int> buckets[maxLog];
    static std::vector<int> suffixArray;
    static std::vector<int> leftLcp;
    static std::vector<int> rightLcp;
    static void BuildBucket(std::string &text);
    static void FillLcpData(int l, int r);
public:
    static void Index(const std::string &inputFile, const std::string &outputFile);
    static void Search(const std::string &inputFile, const std::string &pattern);
};