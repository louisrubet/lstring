// SPDX-License-Identifier: MIT

#pragma once

#include <ctype.h>

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class lstring {
   public:
    lstring() : str(nullptr), cap(0), len(0) {}

    // clang-format off
    /// @brief create a string from constant litteral string
    template <size_t N>
    lstring(const char (&str)[N]) { this->str = (char *)str; this->cap = N; this->len = N - 1; }

    /// @brief create a new clear string
    lstring(char *str, size_t cap) { this->str = str; this->cap = cap; clear(); }

    /// @brief create a string from existing char*
    lstring(char *str, size_t cap, size_t len) { this->str = str; this->cap = cap; this->len = len; }

    /// @brief borrow an existing string
    lstring(const char *str, size_t cap, size_t len) { this->str = (char *)str; this->cap = cap; this->len = len; }

    /// @brief borrow an existing string
    lstring& operator=(const lstring &other) { str = other.str; cap = other.cap; len = other.len; return *this; }
    // clang-format on

    /// @brief allocate shortest and copy
    lstring& allocateCopy(const lstring& other) {
        if (other.str == nullptr || ((str = (char*)malloc(other.len + 1)) == nullptr)) {
            cap = 0;
            len = 0;
            return *this;
        }
        len = other.len;
        cap = len + 1;
        return cpy(other.str, len);
    }

    lstring& allocateEmpty() { return allocateCopy(lstring("")); }

    bool allocated() const { return str != nullptr; }
    bool empty() const { return str == nullptr || len == 0; }

    lstring& drop() {
        if (str != nullptr && cap > 0 && len > 0) str[--len] = 0;
        return *this;
    }

    lstring& clear() {
        if (str != nullptr && cap > 0) {
            str[0] = 0;
            len = 0;
        }
        return *this;
    }

   public:  // concatenation, copy
    lstring& cat(const lstring& other) {
        if (other.str == nullptr || cap < len + 1) return *this;

        size_t max_copy = cap - len - 1;
        size_t copy_len = (other.len < max_copy) ? other.len : max_copy;
        if (copy_len == 0) return *this;

        memcpy(str + len, other.str, copy_len);
        len += copy_len;
        str[len] = '\0';

        return *this;
    }

    template <size_t N>
    lstring& cat(const char (&other)[N]) {
        return cat(lstring(other, N, N - 1));
    }

    lstring& cat(char c) {
        if (str == nullptr) return *this;
        if (len < cap - 1) {
            str[len++] = c;
            str[len] = '\0';
        }
        return *this;
    }

    lstring& cpy(const char* src, size_t src_len) {
        if (src != nullptr && str != nullptr) {
            len = (src_len < cap - 1) ? src_len : cap - 1;
            memcpy(this->str, src, len);
            this->str[len] = 0;
        }
        return *this;
    }

    lstring& cpy(const lstring& other) { return cpy(other.str, other.len); }

   public:  // trimming
    lstring& trim() {
        if (len > 0 && str != nullptr) {
            // left: count spaces and shift
            int spaces = 0;
            while (spaces < len && isspace(str[spaces])) spaces++;
            for (int i = 0; i < len - spaces; i++) str[i] = str[i + spaces];
            len -= spaces;
            // right: decrease len
            for (int i = len - 1; i > 0 && isspace(str[i]); i--) len--;
            str[len] = 0;
        }
        return *this;
    }

   public:  // formatting
    lstring& sprintf(const char* format, ...) {
        va_list args;
        va_start(args, format);
        len = (size_t)vsnprintf(str, cap, format, args);
        va_end(args);
        return *this;
    }

   public:  // comparing
    template <size_t N>
    bool startsWith(const char (&other)[N]) {
        return strncmp(str, other, N - 1) == 0;
    }

    template <size_t N>
    bool operator==(const char (&other)[N]) {
        if (len != (N - 1)) return false;
        return strncmp(str, other, N) == 0;
    }

    template <size_t N>
    bool operator==(const char (&other)[N]) const {
        if (len != (N - 1)) return false;
        return strncmp(str, other, N) == 0;
    }

    bool operator==(const lstring& other) {
        if (len != other.len) return false;
        return strncmp(str, other.str, len) == 0;
    }

    bool operator==(const lstring& other) const {
        if (len != other.len) return false;
        return strncmp(str, other.str, len) == 0;
    }

    operator const char*() const { return str; }

   public:
    static lstring toString(bool b, char (&buf)[16]) { return b ? lstring("true") : lstring("false"); }
    static lstring toString(float f, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%.02f", f); }
    static lstring toString(int i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%d", i); }
    static lstring toString(short i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hd", i); }
    static lstring toString(char i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hhd", i); }
    static lstring toString(unsigned int i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%u", i); }
    static lstring toString(unsigned short i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hu", i); }
    static lstring toString(unsigned char i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hhu", i); }

   public:
    int toInt() const {
        int res = 0;
        if (str != nullptr && len > 0) (void)sscanf(str, "%d", &res);
        return res;
    }

    double toDouble() const {
        double res = 0;
        if (str != nullptr && len > 0) (void)sscanf(str, "%lf", &res);
        return res;
    }

   public:
    char* str = nullptr;
    size_t cap = 0;
    size_t len = 0;
};
