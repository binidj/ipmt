#include "SuffixArray.h"

const int ALPHABET_SIZE = 128;

std::vector<int> SuffixArray::buckets[maxLog];
std::vector<int> SuffixArray::suffixArray = std::vector<int>();
std::vector<int> SuffixArray::leftLCP;
std::vector<int> SuffixArray::rightLCP;
std::vector<int> SuffixArray::frequency = std::vector<int>(ALPHABET_SIZE);
int SuffixArray::logSize = 0;
int SuffixArray::textSize = 0;

// Build lcp array with radix sort
void SuffixArray::BuildBucket(std::string &text)
{
    int n = text.size(), c = 0;
    std::vector<int> temp(n), posBucket(n), bucket(n), bpos(n);
    suffixArray.resize(n);
    
    buckets[logSize] = std::vector<int>(n);

    for (int i = 0; i < n; i++) suffixArray[i] = i;
    
    sort(suffixArray.begin(), suffixArray.end(), 
        [&](int a, int b) -> bool { return text[a] < text[b]; }
    );

    for (int i = 0; i < n; i++) 
    {
        frequency[text[i]]++;
        buckets[logSize][suffixArray[i]] = c;
        bucket[i] = c;
        if (i + 1 == n || text[suffixArray[i]] != text[suffixArray[i + 1]]) c++;
    }
    
    logSize = 1;
    for (int h = 1; h < n && c < n; h <<= 1) 
    {
        for (int i = 0; i < n; i++) posBucket[suffixArray[i]] = bucket[i];
        
        for (int i = n - 1; i >= 0; i--) bpos[bucket[i]] = i;
        
        for (int i = 0; i < n; i++)
            if (suffixArray[i] >= n - h) temp[bpos[bucket[i]]++] = suffixArray[i];
        
        for (int i = 0; i < n; i++)
            if (suffixArray[i] >= h) temp[bpos[posBucket[suffixArray[i] - h]]++] = suffixArray[i] - h;
        
        c = 0;
        buckets[logSize] = std::vector<int>(n);
        for (int i = 0; i + 1 < n; i++) 
        {
            int a = (bucket[i] != bucket[i + 1]) || (temp[i] >= n - h) || (posBucket[temp[i + 1] + h] != posBucket[temp[i] + h]);
            buckets[logSize][suffixArray[i]] = c;
            bucket[i] = c;
            c += a;
        }
        buckets[logSize][suffixArray[n - 1]] = c;
        bucket[n - 1] = c++;
        logSize += 1;
        temp.swap(suffixArray);
    }

    // print buckets's
    // for (int i = 0; i < logSize; i++)
    // {
    //     std::cout << "LOG " << i << ": ";
    //     for (int j = 0; j < n; j++)
    //     {
    //         std::cout << buckets[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }
}

int SuffixArray::LCP(int leftSuffix, int rightSuffix)
{
    if (leftSuffix == rightSuffix)
    {
        return textSize - leftSuffix;
    }
    else
    {
        int lcp = 0;
        
        for (int log = logSize - 1; log >= 0 && leftSuffix < textSize && rightSuffix < textSize; log--)
        {
            int jump = (1 << log);
            if (buckets[log][leftSuffix] == buckets[log][rightSuffix])
            {
                lcp += jump;
                leftSuffix += jump;
                rightSuffix += jump;
            }
            log -= 1;
        }

        return lcp;
    }
}

void SuffixArray::FillLCPData(int l, int r)
{
    if (r-l <= 1)
        return;
    
    int h = (l+r)/2;
    leftLCP[h] = LCP(suffixArray[l], suffixArray[h]);
    rightLCP[h] = LCP(suffixArray[h], suffixArray[r]);

    FillLCPData(l, h);
    FillLCPData(h, r);
}

void SuffixArray::Index(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream inputStream(inputFile);

    if (inputStream.fail())
    {
        fprintf(stderr, "Failed to read file %s\n", inputFile.c_str());
        return;
    }

    inputStream.seekg(0, std::ios::end);
    size_t size = inputStream.tellg();
    std::string text(size, ' ');
    inputStream.seekg(0);
    inputStream.read(&text[0], size);
    
    if (size == 0)
    {
        fprintf(stderr, "File %s is empty\n", inputFile.c_str());
        return;
    }

    textSize = size;
    
    BuildBucket(text);

    leftLCP = std::vector<int>(size, -1);
    rightLCP = std::vector<int>(size, -1);

    FillLCPData(0, size - 1);

    std::ofstream outputStream(outputFile);
    
    // write sizes
    outputStream.write(reinterpret_cast<char*>(&logSize), sizeof(logSize));
    outputStream.write(reinterpret_cast<char*>(&textSize), sizeof(textSize));

    // write buckets
    for (int i = 0; i < logSize; i++)
    {
        outputStream.write(reinterpret_cast<char*>(buckets[i].data()), textSize * sizeof(int));
    }

    // write suffix array data
    outputStream.write(reinterpret_cast<char*>(suffixArray.data()), textSize * sizeof(int));
    outputStream.write(reinterpret_cast<char*>(leftLCP.data()), textSize * sizeof(int));
    outputStream.write(reinterpret_cast<char*>(rightLCP.data()), textSize * sizeof(int));

    // write frequency
    outputStream.write(reinterpret_cast<char*>(frequency.data()), ALPHABET_SIZE * sizeof(int));
}