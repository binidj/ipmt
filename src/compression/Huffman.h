#pragma once
#include <fstream>
#include <iostream>

class Huffman 
{
    struct Node
    {
        char symbol;
        int frequency;
        Node* left;
        Node* right;
    };
    
public:
    static void Compress(const std::string &inputFile, const std::string &outputFile);
    static void Decompress(const std::string &inputFile, const std::string &outputFile);
};