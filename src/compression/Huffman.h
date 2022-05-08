#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

class Huffman 
{
    struct Node
    {
        char symbol;
        int frequency;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node();
        Node(char symbol, int frequency, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr) :
            symbol(symbol), frequency(frequency), left(std::move(left)), right(std::move(right)) {};
    };
    
public:
    static void Compress(const std::string &inputFile, const std::string &outputFile);
    static void Decompress(const std::string &inputFile, const std::string &outputFile);
};