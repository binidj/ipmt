#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

class Huffman 
{
    struct Node
    {
        char symbol;
        int frequency;
        std::unique_ptr<Node> childs[2];

        Node();
        Node(char symbol, int frequency, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr) :
            symbol(symbol), frequency(frequency), childs { std::move(left), std::move(right) } {};
    };
private:
    static std::unique_ptr<Node> root = nullptr;

public:
    static void Compress(const std::string &inputFile, const std::string &outputFile);
    static void Decompress(const std::string &inputFile, const std::string &outputFile);
    static std::vector<unsigned int> GetCharFrequency(const std::vector<unsigned char> &text);
    static void BuildTree(const std::vector<unsigned int> &frequency);
};

bool operator<(const std::unique_ptr<Node> &left, const std::unique_ptr<Node> &right)
{
    return left->frequency < right->frequency;
}