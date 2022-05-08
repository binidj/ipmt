#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>

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
    static void PrintTree(std::unique_ptr<Node> &node, int level);
    static void BuildTree(const std::vector<unsigned int> &frequency);
    static std::vector<unsigned int> GetCharFrequency(const std::vector<unsigned char> &text);
    static void Encode(std::vector<unsigned char> &text);
    static void GetCodes(std::unique_ptr<Node> &cursor, std::string &codesBuffer);
public:
    static void Compress(const std::string &inputFile, const std::string &outputFile);
    static void Decompress(const std::string &inputFile, const std::string &outputFile);
};