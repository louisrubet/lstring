
// SPDX-License-Identifier: MIT

#pragma once
#include <cstring>

class lstring {
   public:
    lstring();
    virtual ~lstring();

    /// @brief create a string from constant litteral string
    template <size_t N> lstring(const char (&str)[N]) { this->str = (char*)str; this->cap = N; this->len = N - 1; }

    lstring(char* str, size_t cap); /// @brief create a new clear string (borrow a buffer)
    lstring(char* str, size_t cap, size_t len); /// @brief create a string from existing char* (borrow a char*)
    lstring(const char* str, size_t cap, size_t len); /// @brief create a string from existing char* (borrow a const char*)
    lstring& operator=(const lstring &other); /// @brief borrow an existing lstring

    lstring& allocateCopy(const lstring& other); /// @brief allocate shortest and copy
    lstring& allocateEmpty(); /// @brief allocate an empty string (len=1, char '\0')

    bool allocated() const; /// @brief whether it's allocated (set) or not
    bool empty() const; /// @brief whether it's allocated (set) OR empty (len = 0)

    lstring& drop(); /// @brief remove last char
    lstring& clear(); /// @brief empty the string

    // concatenate
    lstring& cat(const lstring& other);
    lstring& cat(char c);
    template <size_t N> lstring& cat(const char (&other)[N]) { return cat(lstring(other, N, N - 1)); }

    // copy
    lstring& cpy(const char* src, size_t src_len);
    lstring& cpy(const lstring& other);

    // remove space chars (cf isspace)
    lstring& trimLeft();
    lstring& trimRight();
    lstring& trim();

    // printf-like format
    lstring& sprintf(const char* format, ...);

    // comparison
    template <size_t N> bool startsWith(const char (&other)[N]) { return strncmp(str, other, N - 1) == 0; }
    template <size_t N> bool operator==(const char (&other)[N]) { if (len != (N - 1)) return false; return strncmp(str, other, N) == 0; }
    template <size_t N> bool operator==(const char (&other)[N]) const { if (len != (N - 1)) return false; return strncmp(str, other, N) == 0; }
    bool operator==(const lstring& other);
    bool operator==(const lstring& other) const;

    operator const char*() const;

    // conversions to standard types
    int toInt() const;
    double toDouble() const;

    // conversions to lstring
    static lstring toString(bool b, char (&buf)[16]);
    static lstring toString(float f, char (&buf)[16]);
    static lstring toString(int i, char (&buf)[16]);
    static lstring toString(short i, char (&buf)[16]);
    static lstring toString(char i, char (&buf)[16]);
    static lstring toString(unsigned int i, char (&buf)[16]);
    static lstring toString(unsigned short i, char (&buf)[16]);
    static lstring toString(unsigned char i, char (&buf)[16]);

   public:
    char* str = nullptr;
    size_t cap = 0;
    size_t len = 0;
};
