#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <string_view>
#include <cstring>
#include <queue>

class Huffman 
{
public:
    struct Node
    {
        unsigned char symbol;
        unsigned int frequency;
        std::unique_ptr<Node> childs[2];

        Node();
        Node(unsigned char symbol, int frequency, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr) :
            symbol(symbol), frequency(frequency), childs { std::move(left), std::move(right) } {};
    };
private:
    static std::unique_ptr<Node> root;
    static std::unordered_map<unsigned char, std::string> huffmanCodes;
    static size_t curBytePos;
    static size_t textIndex;
    static unsigned char curBitPos;
    static void PrintTree(std::unique_ptr<Node> &node, int level);
    static void BuildTree(const std::vector<unsigned int> &frequency);
    static std::vector<unsigned int> GetCharFrequency(const std::vector<unsigned char> &text);
    static void Encode(std::vector<unsigned char> &text);
    static void GetCodes(std::unique_ptr<Node> &cursor, std::string &codesBuffer);
    inline static void TryWriteData(std::queue<unsigned char> &charBuffer, std::vector<unsigned char> &text);
    inline static void SetBit(unsigned char &byte, unsigned char pos);
public:
    static int Compress(const std::string &inputFile, const std::string &outputFile);
    static int Decompress(const std::string &inputFile, const std::string &outputFile);
};