#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <set>

static const int maxLog = 25;
static const int maxSize = 25000000;

class SuffixArray 
{
private:
    static int logSize;
    static int textSize;
    static int cmp;
    static long long occCount;
    static bool processLines;
    static std::string text;
    static std::vector<int> buckets[maxLog];
    static std::vector<int> suffixArray;
    static std::vector<int> leftLCP;
    static std::vector<int> rightLCP;
    static std::vector<int> frequency;
    // static std::vector<int> occurences;
    static std::set<int> occurences;
    static void BuildBucket();
    static void FillLCPData(int l, int r);
    static int LCP(int leftSuffix, int rightSuffix);
    static void RebuildText();
    static void SearchWord(const std::string &pattern);
    static int Successor(const std::string &pattern);
    static int Predecessor(const std::string &pattern);
    static int LexCmp(const std::string_view &lhs, const std::string_view &rhs);
public:
    static void Index(const std::string &inputFile, const std::string &outputFile);
    static void Search(const std::string &indexFile, const std::vector<std::string> &patterns, bool printCount);
};