#include "SuffixArray.h"

std::vector<int> SuffixArray::P[maxLog];
std::vector<int> SuffixArray::suffixArray = std::vector<int>();
int SuffixArray::prevP = 0;

void SuffixArray::Build_P(std::string &text)
{
    int n = text.size(), c = 0;
    std::vector<int> temp(n), posBucket(n), bucket(n), bpos(n);
    suffixArray.resize(n);
    
    P[prevP] = std::vector<int>(n);

    for (int i = 0; i < n; i++) suffixArray[i] = i;
    
    sort(suffixArray.begin(), suffixArray.end(), 
        [&](int a, int b) -> bool { return text[a] < text[b]; }
    );

    for (int i = 0; i < n; i++) 
    {
        P[prevP][suffixArray[i]] = c;
        bucket[i] = c;
        if (i + 1 == n || text[suffixArray[i]] != text[suffixArray[i + 1]]) c++;
    }
    
    prevP = 1;
    for (int h = 1; h < n && c < n; h <<= 1) 
    {
        for (int i = 0; i < n; i++) posBucket[suffixArray[i]] = bucket[i];
        
        for (int i = n - 1; i >= 0; i--) bpos[bucket[i]] = i;
        
        for (int i = 0; i < n; i++)
            if (suffixArray[i] >= n - h) temp[bpos[bucket[i]]++] = suffixArray[i];
        
        for (int i = 0; i < n; i++)
            if (suffixArray[i] >= h) temp[bpos[posBucket[suffixArray[i] - h]]++] = suffixArray[i] - h;
        
        c = 0;
        P[prevP] = std::vector<int>(n);
        for (int i = 0; i + 1 < n; i++) 
        {
            int a = (bucket[i] != bucket[i + 1]) || (temp[i] >= n - h) || (posBucket[temp[i + 1] + h] != posBucket[temp[i] + h]);
            P[prevP][suffixArray[i]] = c;
            bucket[i] = c;
            c += a;
        }
        P[prevP][suffixArray[n - 1]] = c;
        bucket[n - 1] = c++;
        prevP += 1;
        temp.swap(suffixArray);
    }

    // print P's
    // for (int i = 0; i < prevP; i++)
    // {
    //     std::cout << "LOG " << i << ": ";
    //     for (int j = 0; j < n; j++)
    //     {
    //         std::cout << P[i][j] << " ";
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
    
    Build_P(text);
}