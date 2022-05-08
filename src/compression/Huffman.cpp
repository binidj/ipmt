#include <Huffman.h>

void Huffman::Compress(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream fileStream(inputFile, std::ios::binary);

    if (fileStream.fail())
    {
        fprintf(stderr, "Failed to read file %s\n", inputFile.c_str());
        return;
    }

    std::vector<unsigned char> text(std::istreambuf_iterator<char>(fileStream), {});

    fileStream.close();

    std::vector<unsigned char> codedText;
    codedText.reserve(text.size());

    // std::ofstream zipStream(outputFile, std::ios::binary);
    // zipStream.write(reinterpret_cast<const char*>(&text[0]), text.size());

    // zipStream.close();   
}