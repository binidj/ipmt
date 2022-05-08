#include <Huffman.h>
const int ALPHABET_SIZE = 128;

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
    const std::vector<unsigned int> frequency = GetCharFrequency(text);

    BuildTree(frequency);



    // std::ofstream zipStream(outputFile, std::ios::binary);
    // zipStream.write(reinterpret_cast<const char*>(&text[0]), text.size());

    // zipStream.close();   
}

std::vector<unsigned int> Huffman::GetCharFrequency(const std::vector<unsigned char> &text)
{
    std::vector<unsigned int> frequency(ALPHABET_SIZE);
    
    for (auto ch : text)
    {
        frequency[ch]++;
    }
    
    return frequency;
}

void Huffman::BuildTree(const std::vector<unsigned int> &frequency)
{
    std::vector<std::unique_ptr<Node>> heap;
    heap.reserve(ALPHABET_SIZE);

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (frequency[i] != 0)
        {
            heap.emplace_back(std::make_unique<Node>(static_cast<char>(i), frequency[i]));
        }
    }

    std::make_heap(heap.begin(), heap.end());

    for (auto &e : heap)
    {
        std::cout << e->frequency << "\n";
    }
}