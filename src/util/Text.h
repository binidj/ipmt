#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

class Text 
{
private:
    char* Data;
    size_t Size;
    bool Borrow = false;
    void RemoveLineFeed();
public:
    void Clear();
    const size_t Length() const;
    char& operator[](const size_t Index);
    const char operator[](const size_t Index) const;
    const char* GetData() const;
    char* begin() const;
    char* end() const;
    Text& operator=(Text&& text) noexcept;
    Text();
    ~Text();
    Text(const Text& text);
    Text(Text&& text) noexcept;
    Text(const char* Buffer);
    Text(char* Buffer, const int BufferSize);
};

bool operator<(const Text &Lhs, const Text &Rhs);