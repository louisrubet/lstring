// SPDX-License-Identifier: MIT

#include "lstring.h"

#include <ctype.h>

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>

lstring::lstring() {}
lstring::~lstring() {}

lstring::lstring(char* str, size_t cap) {
    this->str = str;
    this->cap = cap;
    clear();
}

lstring::lstring(char* str, size_t cap, size_t len) {
    this->str = str;
    this->cap = cap;
    this->len = len;
}

lstring::lstring(const char* str, size_t cap, size_t len) {
    this->str = (char*)str;
    this->cap = cap;
    this->len = len;
}

lstring& lstring::operator=(const lstring& other) {
    str = other.str;
    cap = other.cap;
    len = other.len;
    return *this;
}

lstring& lstring::allocateCopy(const lstring& other) {
    if (other.str != nullptr && ((str = (char*)malloc(other.len + 1)) != nullptr)) {
        len = other.len;
        cap = len + 1;
        return cpy(other.str, len);
    }
    return *this;
}

lstring& lstring::allocateEmpty() { return allocateCopy(lstring("")); }

bool lstring::allocated() const { return str != nullptr; }
bool lstring::empty() const { return str == nullptr || len == 0; }

lstring& lstring::drop() {
    if (str != nullptr && cap > 0 && len > 0) str[--len] = 0;
    return *this;
}

lstring& lstring::clear() {
    if (str != nullptr && cap > 0) {
        str[0] = 0;
        len = 0;
    }
    return *this;
}

lstring& lstring::cat(const lstring& other) {
    if (other.str == nullptr || cap < len + 1) return *this;

    size_t max_copy = cap - len - 1;
    size_t copy_len = (other.len < max_copy) ? other.len : max_copy;
    if (copy_len == 0) return *this;

    memcpy(str + len, other.str, copy_len);
    len += copy_len;
    str[len] = '\0';

    return *this;
}

lstring& lstring::cat(char c) {
    if (str != nullptr && len < cap - 1) {
        str[len++] = c;
        str[len] = '\0';
    }
    return *this;
}

lstring& lstring::cpy(const char* src, size_t src_len) {
    if (src != nullptr && str != nullptr) {
        len = (src_len < cap - 1) ? src_len : cap - 1;
        memcpy(this->str, src, len);
        this->str[len] = 0;
    }
    return *this;
}

lstring& lstring::cpy(const lstring& other) { return cpy(other.str, other.len); }

lstring& lstring::trimLeft() {
    if (str != nullptr && len > 0) {
        int spaces = 0;
        while (spaces < len && isspace(str[spaces])) spaces++;
        for (int i = 0; i < len - spaces; i++) str[i] = str[i + spaces];
        len -= spaces;
        str[len] = 0;
    }
    return *this;
}

lstring& lstring::trimRight() {
    if (str != nullptr) {
        while (len > 0 && isspace(str[len - 1])) len--;
        str[len] = 0;
    }
    return *this;
}

lstring& lstring::trim() { return trimRight().trimLeft(); }

lstring& lstring::sprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    len = (size_t)vsnprintf(str, cap, format, args);
    va_end(args);
    return *this;
}

bool lstring::operator==(const lstring& other) {
    if (len != other.len) return false;
    return strncmp(str, other.str, len) == 0;
}

bool lstring::operator==(const lstring& other) const {
    if (len != other.len) return false;
    return strncmp(str, other.str, len) == 0;
}

lstring::operator const char*() const { return str; }

lstring lstring::toString(bool b, char (&buf)[16]) { return b ? lstring("true") : lstring("false"); }
lstring lstring::toString(float f, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%.02f", f); }
lstring lstring::toString(int i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%d", i); }
lstring lstring::toString(short i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hd", i); }
lstring lstring::toString(char i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hhd", i); }
lstring lstring::toString(unsigned int i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%u", i); }
lstring lstring::toString(unsigned short i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hu", i); }
lstring lstring::toString(unsigned char i, char (&buf)[16]) { return lstring(buf, sizeof(buf)).sprintf("%hhu", i); }

int lstring::toInt() const {
    int res = 0;
    if (str != nullptr && len > 0) (void)sscanf(str, "%d", &res);
    return res;
}

double lstring::toDouble() const {
    double res = 0;
    if (str != nullptr && len > 0) (void)sscanf(str, "%lf", &res);
    return res;
}
