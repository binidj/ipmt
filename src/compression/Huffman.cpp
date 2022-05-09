#include <Huffman.h>
const int ALPHABET_SIZE = 128;

std::unique_ptr<Huffman::Node> Huffman::root = nullptr;
std::unordered_map<unsigned char, std::string> Huffman::huffmanCodes = std::unordered_map<unsigned char, std::string>();
size_t Huffman::curBytePos = 0;
size_t Huffman::textIndex = 0;
unsigned char Huffman::curBitPos = 0;

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

    if (text.size() == 0)
    {
        fprintf(stderr, "File %s is empty\n", inputFile.c_str());
        return;
    }
    
    const size_t fileSize = text.size();
    const std::vector<unsigned int> frequency = GetCharFrequency(text);

    BuildTree(frequency);

    Encode(text);

    std::ofstream zipStream(outputFile, std::ios::binary);
    zipStream.write(reinterpret_cast<const char*>(&frequency[0]), sizeof(unsigned int) * frequency.size());
    zipStream.write(reinterpret_cast<const char*>(&curBitPos), sizeof(unsigned char));
    zipStream.write(reinterpret_cast<const char*>(&fileSize), sizeof(size_t));
    zipStream.write(reinterpret_cast<const char*>(&text[0]), curBytePos + (curBitPos != 0));
    zipStream.close();
}

void Huffman::Decompress(const std::string &inputFile, const std::string &outputFile)
{
     std::ifstream fileStream(inputFile, std::ios::binary);

    if (fileStream.fail())
    {
        fprintf(stderr, "Failed to read file %s\n", inputFile.c_str());
        return;
    }

    std::vector<unsigned char> text(std::istreambuf_iterator<char>(fileStream), {});

    fileStream.close();

    if (text.size() == 0)
    {
        fprintf(stderr, "File %s is empty\n", inputFile.c_str());
        return;
    }

    std::vector<unsigned int> frequency(ALPHABET_SIZE);
    curBytePos = 0;
    for (curBytePos = 0; curBytePos < sizeof(unsigned int) * ALPHABET_SIZE; curBytePos += sizeof(unsigned int))
    {
        frequency[curBytePos / sizeof(unsigned int)] = static_cast<unsigned int>(
            text[curBytePos + 3] << 24 |
            text[curBytePos + 2] << 16 |
            text[curBytePos + 1] << 8 |
            text[curBytePos]
        );
    }

    unsigned char lastByteBits = text[curBytePos];
    curBytePos += 1;

    size_t fileSize = 0;
    std::memcpy(&fileSize, &text[curBytePos], sizeof(size_t));

    curBytePos += sizeof(size_t);

    BuildTree(frequency);

    if (text.size() < fileSize)
        text.reserve(fileSize);
    
    Node* cursor = root.get();
    
    std::queue<unsigned char> charPool;
    textIndex = 0;
    size_t lastFullByte = text.size()-1;
    
    // corner case: single node tree
    if (cursor->childs[0] == nullptr)
    {
        text.resize(fileSize);
        std::memset(&text[0],cursor->symbol,fileSize);
        goto WriteFile;
    }

    
    for (; curBytePos < lastFullByte; curBytePos++)
    {
        TryWriteData(charPool, text);
        
        for (unsigned char bitPos = 0; bitPos < 8; bitPos++)
        {
            unsigned char bitValue = text[curBytePos] & 1;
            text[curBytePos] >>= 1;
            cursor = cursor->childs[bitValue].get();
            if (cursor->childs[0] == nullptr)
            {
                charPool.push(cursor->symbol);
                cursor = root.get();
            }
        }
    }

    // iteration for last byte
    lastByteBits = (lastByteBits == 0) ? 8 : lastByteBits;
    for (unsigned char bitPos = 0; bitPos < lastByteBits; bitPos++)
    {
        unsigned char bitValue = text[curBytePos] & 1;
        text[curBytePos] >>= 1;
        cursor = cursor->childs[bitValue].get();
        if (cursor->childs[0] == nullptr)
        {
            charPool.push(cursor->symbol);
            cursor = root.get();
        }
    }
    
    curBytePos += 1;
    TryWriteData(charPool, text);
    while (!charPool.empty())
    {
        text.emplace_back(charPool.front());
        charPool.pop();
    }
    
WriteFile:

    std::ofstream zipStream(outputFile, std::ios::binary);
    zipStream.write(reinterpret_cast<const char*>(&text[0]), fileSize);
    zipStream.close();
}

inline void Huffman::TryWriteData(std::queue<unsigned char> &charBuffer, std::vector<unsigned char> &text)
{
    while (textIndex < curBytePos && !charBuffer.empty())
    {
        text[textIndex] = charBuffer.front();
        charBuffer.pop();
        textIndex += 1;
    }
}

void Huffman::GetCodes(std::unique_ptr<Huffman::Node> &cursor, std::string &codesBuffer)
{
    if (cursor->childs[0] == nullptr)
    {
        huffmanCodes[cursor->symbol] = codesBuffer;
        return;
    }
    for (int bit = 0; bit < 2; bit++)
    {
        codesBuffer.push_back(char(bit+48));
        GetCodes(cursor->childs[bit], codesBuffer);
        codesBuffer.pop_back();    
    }
    return;
}

inline void Huffman::SetBit(unsigned char &byte, unsigned char pos)
{
    byte |= (static_cast<unsigned char>(1) << pos);
}

void Huffman::Encode(std::vector<unsigned char> &text)
{
    std::string codesBuffer = "";
    GetCodes(root, codesBuffer);
    
    curBytePos = 0;
    curBitPos = 0;

    std::string leftOverBits = "";
    size_t leftOverIndex = 0;

    for (size_t i = 0; i < text.size(); i++)
    {
        unsigned char curChar = text[i];
        text[i] = 0;
        
        // Place possible remaining bits from previous iterations
        if (leftOverBits.size() != leftOverIndex)
        {
            for (size_t j = leftOverIndex; j < leftOverBits.size(); j++)
            {
                if (curBytePos > i)
                {
                    break;
                }
                if (leftOverBits[j] != '0') SetBit(text[curBytePos], curBitPos);
                curBitPos += 1;
                curBitPos %= 8;
                curBytePos += (curBitPos == 0);
                leftOverIndex += 1;
            } 
        }

        if (curBytePos > i)
            continue;
        
        const std::string &charCode = huffmanCodes.at(curChar);
        
        // Place current charCode
        for (size_t j = 0; j < charCode.size(); j++)
        {
            if (curBytePos > i)
            {
                leftOverBits += charCode.substr(j);
                break;
            }
            if (charCode[j] != '0') SetBit(text[curBytePos], curBitPos);
            curBitPos += 1;
            curBitPos %= 8;
            curBytePos += (curBitPos == 0);
        }
    }
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
            heap.emplace_back(std::make_unique<Node>(static_cast<unsigned char>(i), frequency[i]));
        }
    }

    std::make_heap(heap.begin(), heap.end(), std::greater<>());

    while (heap.size() > 1)
    {
        std::pop_heap(heap.begin(), heap.end(), std::greater<>());
        std::unique_ptr<Node> left = std::move(heap.back());
        heap.pop_back();

        std::pop_heap(heap.begin(), heap.end(), std::greater<>());
        std::unique_ptr<Node> right = std::move(heap.back());
        heap.pop_back();

        std::unique_ptr<Node> parentNode = std::make_unique<Node>(
            static_cast<char>(-1),
            left->frequency + right->frequency,
            std::move(left),
            std::move(right)
        );

        heap.push_back(std::move(parentNode));
        std::push_heap(heap.begin(), heap.end(), std::greater<>());
    }

    root = std::move(heap.back()); 
}

void Huffman::PrintTree(std::unique_ptr<Node> &node, int level)
{
    if (node == nullptr)
        return;

    PrintTree(node->childs[1], level + 1);
    std::cout << std::string(level*4, ' ') << "[" << int(node->symbol) << " " << node->frequency << "]\n";
    PrintTree(node->childs[0], level + 1);    
}

bool operator<(const std::unique_ptr<Huffman::Node> &left, const std::unique_ptr<Huffman::Node> &right)
{
    return left->frequency < right->frequency;
}