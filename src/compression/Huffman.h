#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

class Huffman 
{
public:
    struct Node
    {
        char symbol;
        unsigned int frequency;
        std::unique_ptr<Node> childs[2];

        Node();
        Node(char symbol, int frequency, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr) :
            symbol(symbol), frequency(frequency), childs { std::move(left), std::move(right) } {};
    };
private:
    static std::unique_ptr<Node> root;
    static void PrintTree(std::unique_ptr<Node> &node, int level);
    static void BuildTree(const std::vector<unsigned int> &frequency);
    static std::vector<unsigned int> GetCharFrequency(const std::vector<unsigned char> &text);
    static void Encode(std::vector<unsigned char> &text, std::vector<unsigned char> &encodedText);
public:
    static void Compress(const std::string &inputFile, const std::string &outputFile);
    static void Decompress(const std::string &inputFile, const std::string &outputFile);
};