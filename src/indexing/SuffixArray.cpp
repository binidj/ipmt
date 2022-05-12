#include "SuffixArray.h"

const int ALPHABET_SIZE = 128;

std::vector<int> SuffixArray::buckets[maxLog];
std::vector<int> SuffixArray::suffixArray;
std::vector<int> SuffixArray::leftLCP;
std::vector<int> SuffixArray::rightLCP;
std::vector<int> SuffixArray::frequency;
std::set<int> SuffixArray::occurences;
std::string SuffixArray::text;
int SuffixArray::logSize = 0;
int SuffixArray::textSize = 0;
long long int SuffixArray::occCount = 0;
int SuffixArray::cmp = 0;
bool SuffixArray::processLines = false;

// Build lcp array with radix sort
void SuffixArray::BuildBucket()
{
    int n = textSize, c = 0;
    std::vector<int> temp(n), posBucket(n), bucket(n), bpos(n);
    
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
    for (int h = 1; h <= n; h <<= 1) 
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
            buckets[logSize][temp[i]] = c;
            bucket[i] = c;
            c += a;
        }
        buckets[logSize][temp[n - 1]] = c;
        bucket[n - 1] = c++;
        logSize += 1;
        temp.swap(suffixArray);
    }

    // print buckets's
    // std::cout << logSize << " !!! \n";
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
    text = std::string(size, ' ');
    inputStream.seekg(0);
    inputStream.read(&text[0], size);

    if (size == 0)
    {
        fprintf(stderr, "File %s is empty\n", inputFile.c_str());
        return;
    }

    textSize = size;
    suffixArray = std::vector<int>(textSize);
    frequency = std::vector<int>(ALPHABET_SIZE);
    
    BuildBucket();

    leftLCP = std::vector<int>(size, -1);
    rightLCP = std::vector<int>(size, -1);

    FillLCPData(0, size - 1);

    std::ofstream outputStream(outputFile);
    
    // std::cout << suffixArray.size() << "\n";
    // for (auto e : suffixArray)
    // {
    //     std::cout << e << " ";
    // }
    // std::cout << "\n";
    
    // std::cout << leftLCP.size() << "\n";
    // for (auto e : leftLCP)
    // {
    //     std::cout << e << " ";
    // }
    // std::cout << "\n";

    // std::cout << rightLCP.size() << "\n";
    // for (auto e : rightLCP)
    // {
    //     std::cout << e << " ";
    // }
    // std::cout << "\n";

    // write sizes
    outputStream.write(reinterpret_cast<char const*>(&logSize), sizeof(logSize));
    outputStream.write(reinterpret_cast<char const*>(&textSize), sizeof(textSize));

    // write suffix array data
    outputStream.write(reinterpret_cast<char const*>(suffixArray.data()), textSize * sizeof(int));
    outputStream.write(reinterpret_cast<char const*>(leftLCP.data()), textSize * sizeof(int));
    outputStream.write(reinterpret_cast<char const*>(rightLCP.data()), textSize * sizeof(int));

    // write frequency
    outputStream.write(reinterpret_cast<char*>(frequency.data()), ALPHABET_SIZE * sizeof(int));
}

void SuffixArray::RebuildText()
{
    int index = 0;
    for (int ch = 0; ch < ALPHABET_SIZE; ch++)
    {
        for (int i = 0; i < frequency[ch]; i++)
        {
            text[suffixArray[index++]] = char(ch);
        }
    }   
}

int SuffixArray::LexCmp(const std::string_view &lhs, const std::string_view &rhs)
{
    int lcp = 0;
    
    const int lsize = lhs.size();
    const int rsize = rhs.size();
    const int minSize = std::min(lsize, rsize);

    for (int i = 0; i < minSize; i++)
    {
        if (lhs[i] == rhs[i])
        {
            lcp += 1;
        }
        else if (lhs[i] < rhs[i])
        {
            cmp = -1;
            return lcp;
        }
        else 
        {
            cmp = 1;
            return lcp;
        }
    }

    if (lsize == rsize)
        cmp = 0;
    else if (lsize < rsize)
        cmp = -1;
    else
        cmp = 1;

    return lcp;
}

int SuffixArray::Successor(const std::string &pattern)
{
    const int patternSize = pattern.size();

    int L = LexCmp(pattern, std::string_view(&text[suffixArray[0]], textSize-suffixArray[0]));
    if (cmp <= 0)
        return 0;

    int R = LexCmp(pattern, std::string_view(&text[suffixArray[textSize-1]], textSize-suffixArray[textSize-1]));
    if (cmp > 0)
        return textSize;

    int l = 0, r = textSize-1;

    std::string_view lPattern(&pattern[L], patternSize-L);
    std::string_view rPattern(&pattern[R], patternSize-R);
    
    while (r-l > 1)
    {
        int h = (l+r)/2;
        int H = -1;
        if (L >= R)
        {
            if (L <= leftLCP[h])
            {
                int lcp = LexCmp(lPattern, std::string_view(&text[suffixArray[h]+L], textSize-suffixArray[h]-L));
                H = L + lcp; 
            }
            else 
                H = leftLCP[h];
        }
        else 
        {
            if (R <= rightLCP[h])
            {
                int lcp = LexCmp(rPattern, std::string_view(&text[suffixArray[h]+R], textSize-suffixArray[h]-R));
                H = R + lcp;
            }
            else 
                H = rightLCP[h];
        }

        if (H == patternSize || pattern[H] <= text[suffixArray[h]+H])
            r = h, R = H, rPattern = std::string_view(&pattern[R], patternSize-R);
        else
            l = h, L = H, lPattern = std::string_view(&pattern[L], patternSize-L);
    }

    return r;
}

int SuffixArray::Predecessor(const std::string &pattern)
{
    const int patternSize = pattern.size();
    
    int L = LexCmp(pattern, std::string_view(&text[suffixArray[0]], textSize-suffixArray[0]));
    if (cmp < 0)
        return -1;

    int R = LexCmp(pattern, std::string_view(&text[suffixArray[textSize-1]], textSize-suffixArray[textSize-1]));
    if (cmp >= 0)
        return textSize-1;

    int l = 0, r = textSize-1;
    
    std::string_view lPattern(&pattern[L], patternSize-L);
    std::string_view rPattern(&pattern[R], patternSize-R);

    while (r-l > 1)
    {
        int h = (l+r)/2;
        int H = -1;
        if (L >= R)
        {
            if (L <= leftLCP[h])
            {
                int lcp = LexCmp(lPattern, std::string_view(&text[suffixArray[h]+L], textSize-suffixArray[h]-L));
                H = L + lcp; 
            }
            else 
                H = leftLCP[h];
        }
        else 
        {
            if (R <= rightLCP[h])
            {
                int lcp = LexCmp(rPattern, std::string_view(&text[suffixArray[h]+R], textSize-suffixArray[h]-R));
                H = R + lcp;
            }
            else 
                H = rightLCP[h];
        }

        if (H == patternSize || pattern[H] > text[suffixArray[h]+H])
            l = h, L = H, lPattern = std::string_view(&pattern[L], patternSize-L);
        else
            r = h, R = H, rPattern = std::string_view(&pattern[R], patternSize-R);
    }

    return l;
}

void SuffixArray::SearchWord(const std::string &pattern)
{
    int leftBorder = Successor(pattern);
    int rightBorder = Predecessor(pattern);

    // std::cout << leftBorder << " " << rightBorder << " ### \n";
    if (processLines)
    {
        for (int i = leftBorder; i <= rightBorder; i++)
            occurences.insert(suffixArray[i]);
    }

    // Add to occurences array
    if (leftBorder <= rightBorder)
        occCount += (rightBorder-leftBorder+1);
}

void SuffixArray::Search(const std::string &indexFile, const std::vector<std::string> &patterns, bool printCount)
{
    std::ifstream inputStream(indexFile);

    if (inputStream.fail())
    {
        fprintf(stderr, "Failed to read file %s\n", indexFile.c_str());
        return;
    }

    inputStream.read(reinterpret_cast<char*>(&logSize), sizeof(int));
    inputStream.read(reinterpret_cast<char*>(&textSize), sizeof(int));

    suffixArray = std::vector<int>(textSize);
    leftLCP = std::vector<int>(textSize);
    rightLCP = std::vector<int>(textSize);
    frequency = std::vector<int>(ALPHABET_SIZE);

    inputStream.read(reinterpret_cast<char *>(suffixArray.data()), textSize * sizeof(int));
    inputStream.read(reinterpret_cast<char *>(leftLCP.data()), textSize * sizeof(int));
    inputStream.read(reinterpret_cast<char *>(rightLCP.data()), textSize * sizeof(int));   

    inputStream.read(reinterpret_cast<char*>(frequency.data()), ALPHABET_SIZE * sizeof(int));
        
    processLines = !printCount;
    
    // for (auto e : suffixArray)
    // {
    //     std::cout << e << " ";
    // }
    // std::cout << "\n";
    
    // for (auto e : leftLCP)
    // {
    //     std::cout << e << " ";
    // }
    // std::cout << "\n";

    // for (auto e : rightLCP)
    // {
    //     std::cout << e << " ";
    // }
    // std::cout << "\n";

    text = std::string(textSize, ' ');
    RebuildText();    

    // std::cout << "REBUILD TEXT: \n" << textSize << "\n";

    if (processLines)
        occurences = std::set<int>();

    for (const std::string &pattern : patterns)
    {
        SearchWord(pattern);    
    }

    if (processLines)
    {
        for (auto it = occurences.begin(); it != occurences.end(); it++)
        {
            int l = *it, r = *it;
            
            while (l >= 0 && text[l] != '\n')
                l--;
            
            while (r < textSize && text[r] != '\n')
                r++;

            l++;
            

            std::cout << "Line size: " << (r-l) << "\n";
            // std::cout << l << " " << *it << " " << r << "\n";
            // printf("%s\n", std::string_view(&text[l], r-l).data());
            bool flag = false;

            while (it != occurences.end() && l >= *it && *it < r)
                it++, flag = true;

            if (flag) it--;

            if (it == occurences.end())
                break;
        }
    }
    else 
    {
        printf("%lld\n", occCount);
    }
}