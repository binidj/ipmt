#include "SuffixArray.h"

std::vector<int> SuffixArray::buckets[maxLog];
std::vector<int> SuffixArray::suffixArray = std::vector<int>();
std::vector<int> SuffixArray::leftLcp = std::vector<int>();
std::vector<int> SuffixArray::rightLcp = std::vector<int>();
int SuffixArray::bucketIndex = 0;

void SuffixArray::BuildBucket(std::string &text)
{
    int n = text.size(), c = 0;
    std::vector<int> temp(n), posBucket(n), bucket(n), bpos(n);
    suffixArray.resize(n);
    
    buckets[bucketIndex] = std::vector<int>(n);

    for (int i = 0; i < n; i++) suffixArray[i] = i;
    
    sort(suffixArray.begin(), suffixArray.end(), 
        [&](int a, int b) -> bool { return text[a] < text[b]; }
    );

    for (int i = 0; i < n; i++) 
    {
        buckets[bucketIndex][suffixArray[i]] = c;
        bucket[i] = c;
        if (i + 1 == n || text[suffixArray[i]] != text[suffixArray[i + 1]]) c++;
    }
    
    bucketIndex = 1;
    for (int h = 1; h < n && c < n; h <<= 1) 
    {
        for (int i = 0; i < n; i++) posBucket[suffixArray[i]] = bucket[i];
        
        for (int i = n - 1; i >= 0; i--) bpos[bucket[i]] = i;
        
        for (int i = 0; i < n; i++)
            if (suffixArray[i] >= n - h) temp[bpos[bucket[i]]++] = suffixArray[i];
        
        for (int i = 0; i < n; i++)
            if (suffixArray[i] >= h) temp[bpos[posBucket[suffixArray[i] - h]]++] = suffixArray[i] - h;
        
        c = 0;
        buckets[bucketIndex] = std::vector<int>(n);
        for (int i = 0; i + 1 < n; i++) 
        {
            int a = (bucket[i] != bucket[i + 1]) || (temp[i] >= n - h) || (posBucket[temp[i + 1] + h] != posBucket[temp[i] + h]);
            buckets[bucketIndex][suffixArray[i]] = c;
            bucket[i] = c;
            c += a;
        }
        buckets[bucketIndex][suffixArray[n - 1]] = c;
        bucket[n - 1] = c++;
        bucketIndex += 1;
        temp.swap(suffixArray);
    }

    // print buckets's
    // for (int i = 0; i < bucketIndex; i++)
    // {
    //     std::cout << "LOG " << i << ": ";
    //     for (int j = 0; j < n; j++)
    //     {
    //         std::cout << buckets[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }
}

void SuffixArray::Index(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream stream(inputFile);
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    std::string text(size, ' ');
    stream.seekg(0);
    stream.read(&text[0], size);
    
    if (size == 0)
    {
        // set error
    }
    
    BuildBucket(text);

    FillLcpData(0, size - 1);

    // dump data to index file  
}