#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "lstring.h"

std::string fail() { return "\033[31mFAIL\033[0m"; }
std::string ok() { return "\033[32mok\033[0m"; }
std::string title(const std::string& text) { return "\n\033[1m" + text + "\033[0m"; }

bool checkString(const std::string& text, const std::string& value, const std::string& expected) {
    std::cout << "Check " << text << " = " << expected;
    if (value != expected)
        std::cout << ", is " << value << " .. " << fail() << std::endl;
    else
        std::cout << " .. " << ok() << std::endl;
    return value == expected;
}

static bool checkBool(const std::string& text, bool value, bool expected) {
    std::cout << "Check " << text << " = " << expected;
    if (value != expected)
        std::cout << ", is " << value << " .. " << fail() << std::endl;
    else
        std::cout << " .. " << ok() << std::endl;
    return value == expected;
}

static bool checkInt(const std::string& text, int value, int expected) {
    std::cout << "Check " << text << " = " << expected;
    if (value != expected)
        std::cout << ", is " << value << " .. " << fail() << std::endl;
    else
        std::cout << " .. " << ok() << std::endl;
    return value == expected;
}

bool testCtor() {
    bool ret = true;
    std::cout << title("lstring::lstring()") << std::endl;

    {
        lstring s;
        ret &= checkBool("lstring - str null", s.str == nullptr, true);
        ret &= checkInt("lstring - capa 0", s.cap, 0);
        ret &= checkInt("lstring - len 0", s.len, 0);
    }

    {
        lstring s("1a2s3d4f5g6h");
        ret &= checkString("lstring(const char (&str)[N]) - str", s.str, "1a2s3d4f5g6h");
        ret &= checkInt("lstring(const char (&str)[N]) - capa", s.cap, 13);
        ret &= checkInt("lstring(const char (&str)[N]) - len", s.len, 12);
    }

    {
        char buf[12] = "abc";
        lstring s(buf, sizeof(buf));
        ret &= checkString("lstring(char* str, size_t capacity) - str", s.str, "");
        ret &= checkInt("lstring(char* str, size_t capacity) - capa", s.cap, 12);
        ret &= checkInt("lstring(char* str, size_t capacity) - len", s.len, 0);
    }

    {
        char* buf = (char*)new char[4];
        strcpy(buf, "abc");
        lstring s(buf, 4, strlen(buf));
        ret &= checkString("lstring(char* str, size_t capacity, size_t length) - 2 - str", s.str, "abc");
        ret &= checkInt("lstring(char* str, size_t capacity, size_t length) - 2 - capa", s.cap, 4);
        ret &= checkInt("lstring(char* str, size_t capacity, size_t length) - 2 - len", s.len, 3);
        delete buf;
    }

    {
        char buf[12] = "abc";
        lstring s(buf, sizeof(buf), strlen(buf));
        ret &= checkString("lstring(char* str, size_t capacity, size_t length) - str", s.str, "abc");
        ret &= checkInt("lstring(char* str, size_t capacity, size_t length) - capa", s.cap, 12);
        ret &= checkInt("lstring(char* str, size_t capacity, size_t length) - len", s.len, 3);
    }

    return ret;
}

bool testAllocateClearDrop() {
    bool ret = true;
    std::cout << title("lstring::allocate clear drop") << std::endl;

    {
        lstring s("1a2s3d4f5g6h");
        lstring n;
        n.allocateCopy(s);
        ret &= checkString("allocateCopy - str", n.str, "1a2s3d4f5g6h");
        ret &= checkInt("allocateCopy - capacity", n.cap, 13);
        ret &= checkInt("allocateCopy - length", n.len, 12);
        ret &= checkBool("allocateCopy != src", s.str != n.str, true);
    }

    {
        lstring s;
        ret &= checkBool("not allocated", s.allocated(), false);
        s.allocateEmpty();
        ret &= checkBool("allocated", s.allocated(), true);
        ret &= checkBool("allocateEmpty - str", s.str != nullptr, true);
        ret &= checkInt("allocateEmpty - length", s.len, 0);
        ret &= checkInt("allocateEmpty - capacity", s.cap, 1);
        ret &= checkBool("empty", s.empty(), true);
    }

    {
        lstring s;
        s.allocateCopy(lstring("hey"));
        s.drop();
        ret &= checkString("drop", s.str, "he");
        s.clear();
        ret &= checkString("clear - str", s.str, "");
        ret &= checkInt("clear - length", s.len, 0);
        ret &= checkInt("clear - capacity", s.cap, 4);
    }

    return ret;
}

bool testCatCpy() {
    bool ret = true;
    std::cout << title("lstring::cat, lstring::cpy, operator=") << std::endl;

    {
        char buf[12] = "ab";
        char buf2[12] = "CDEF";
        lstring a(buf, sizeof(buf), strlen(buf));
        lstring b(buf2, sizeof(buf2), strlen(buf2));
        a.cat(b);
        ret &= checkString("lstring& cat(lstring& other) - str", a.str, "abCDEF");
        ret &= checkInt("lstring& cat(lstring& other) - capa", a.cap, 12);
        ret &= checkInt("lstring& cat(lstring& other) - len", a.len, 6);
    }

    {
        char buf[12] = "ab";
        lstring a(buf, sizeof(buf), strlen(buf));
        a.cat("CDEF");
        ret &= checkString("lstring& cat(const char (&other)[N] - str", a.str, "abCDEF");
        ret &= checkInt("lstring& cat(const char (&other)[N] - capa", a.cap, 12);
        ret &= checkInt("lstring& cat(const char (&other)[N] - len", a.len, 6);
    }

    {
        char buf[12] = "ab";
        lstring a(buf, sizeof(buf), strlen(buf));
        a.cat('9');
        ret &= checkString("lstring& cat(char c) - str", a.str, "ab9");
        a.cat('A');
        ret &= checkString("lstring& cat(char c) - str", a.str, "ab9A");
        ret &= checkInt("lstring& cat(char c) - capa", a.cap, 12);
        ret &= checkInt("lstring& cat(char c) - len", a.len, 4);
    }

    {
        char buf[12] = "ab";
        lstring a(buf, sizeof(buf), strlen(buf));
        a.cpy("CDEF");
        ret &= checkString("lstring& cpy(lstring& other) - str", a.str, "CDEF");
        ret &= checkInt("lstring& cpy(lstring& other) - capa", a.cap, 12);
        ret &= checkInt("lstring& cpy(lstring& other) - len", a.len, 4);
    }

    {
        char buf[12] = "ab";
        char buf2[12] = "CDEF";
        lstring a(buf, sizeof(buf), strlen(buf));
        lstring b(buf2, sizeof(buf2), strlen(buf2));
        a.cpy(b);
        ret &= checkString("lstring& cpy(lstring& other) - 2 - str", a.str, "CDEF");
        ret &= checkInt("lstring& cpy(lstring& other) - 2 - capa", a.cap, 12);
        ret &= checkInt("lstring& cpy(lstring& other) - 2 - len", a.len, 4);
    }

    {
        char buf[12] = "ab";
        lstring a(buf, sizeof(buf), strlen(buf));
        lstring b;
        b = a;
        ret &= checkString("lstring& operator= - str", a.str, b.str);
        ret &= checkInt("lstring& operator= - capa", (int)a.cap, (int)b.cap);
        ret &= checkInt("lstring& operator= - len", (int)a.len, (int)a.len);
    }

    {
        char buf[12] = "ab";
        lstring a(buf, sizeof(buf), strlen(buf));
        lstring b = a;
        ret &= checkString("lstring b = a (copy without operator=) - str", a.str, b.str);
        ret &= checkInt("lstring b = a (copy without operator=) - capa", (int)a.cap, (int)b.cap);
        ret &= checkInt("lstring b = a (copy without operator=) - len", (int)a.len, (int)a.len);
    }

    return ret;
}

bool testTrim() {
    bool ret = true;
    std::cout << title("lstring::trimLeft, lstring::trimRight, lstring::trim") << std::endl;

    {
        char buf[4][12] = {"abc  ", "   abc  ", "\t \t abc  ", "    abc  "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trimLeft();
            ret &= checkString("trimLeft - str", a.str, "abc  ");
            ret &= checkInt("trimLeft - length", a.len, 5);
        }
    }

    {
        char buf[4][12] = {"       ", "", " ", "\t  "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trimLeft();
            ret &= checkString("trimLeft - str", a.str, "");
            ret &= checkInt("trimLeft - length", a.len, 0);
        }
    }

    {
        char buf[4][12] = {"a b c", "  a b c", "\ta b c", "\t\t a b c"};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trimLeft();
            ret &= checkString("trimLeft - str", a.str, "a b c");
            ret &= checkInt("trimLeft - length", a.len, 5);
        }
    }

    {
        char buf[4][12] = {"  abc      ", "  abc", "  abc\t\t \t", "  abc "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trimRight();
            ret &= checkString("trimRight - str", a.str, "  abc");
            ret &= checkInt("trimRight - length", a.len, 5);
        }
    }

    {
        char buf[4][12] = {"       ", "", " ", "\t  "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trimRight();
            ret &= checkString("trimRight - str", a.str, "");
            ret &= checkInt("trimRight - length", a.len, 0);
        }
    }

    {
        char buf[4][12] = {"a b c", "a b c  ", "a b c\t", "a b c \t\t  "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trimRight();
            ret &= checkString("trimRight - str", a.str, "a b c");
            ret &= checkInt("trimRight - length", a.len, 5);
        }
    }

    {
        char buf[4][12] = {"abc       ", "   abc", "abc", "    abc "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trim();
            ret &= checkString("trim - str", a.str, "abc");
            ret &= checkInt("trim - length", a.len, 3);
        }
    }

    {
        char buf[4][12] = {"       ", "", " ", "\t  "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trim();
            ret &= checkString("trim - str", a.str, "");
            ret &= checkInt("trim - length", a.len, 0);
        }
    }

    {
        char buf[4][12] = {"a b c", "  a b c", "a b c  ", " a b c   "};
        for (int i = 0; i < 4; i++) {
            lstring a(buf[i], sizeof(buf[i]), strlen(buf[i]));
            a.trim();
            ret &= checkString("trim - str", a.str, "a b c");
            ret &= checkInt("trim - length", a.len, 5);
        }
    }

    return ret;
}

bool testEqualStartWith() {
    bool ret = true;

    {
        lstring a("abcDE");
        ret &= checkBool("operator==(const char (&other)[N])", a == "abcDE", true);
        ret &= checkBool("operator==(const char (&other)[N])", a == "ab", false);
    }

    {
        lstring a("abc");
        lstring b("abc");
        lstring c("abDEF");
        ret &= checkBool("operator==(lstring& other)", a == b, true);
        ret &= checkBool("operator==(lstring& other)", a == c, false);
    }

    {
        lstring a, b;
        ret &= checkBool("operator==(lstring& other) default", a == b, true);
        a.allocateEmpty();
        ret &= checkBool("operator==(lstring& other) default different", a != b, true);
        b.allocateEmpty();
        ret &= checkBool("operator==(lstring& other) empty", a == b, true);
    }

    {
        lstring a("abcDE");
        ret &= checkBool("operator==(const char (&other)[N])", a.startsWith("abcDE"), true);
        ret &= checkBool("operator==(const char (&other)[N])", a.startsWith("abc"), true);
        ret &= checkBool("operator==(const char (&other)[N])", a.startsWith("abcDEF"), false);
    }

    return ret;
}

bool testToStringIntDouble() {
    bool ret = true;
    char buf[16];

    ret &= checkString("toString(bool b..)", lstring::toString(true, buf).str, "true");
    ret &= checkString("toString(bool b..)", lstring::toString(false, buf).str, "false");
    ret &= checkString("toString(float f..)", lstring::toString(3.14159f, buf).str, "3.14");
    ret &= checkString("toString(float f..)", lstring::toString(-3.14159e-22f, buf).str, "-0.00");
    ret &= checkString("toString(float f..)", lstring::toString(3.14159e-22f, buf).str, "0.00");
    ret &= checkString("toString(float f..)", lstring::toString(1.f / 0.f, buf).str, "inf");
    ret &= checkString("toString(float f..)", lstring::toString(1.f / -0.f, buf).str, "-inf");
    ret &= checkString("toString(int i..)", lstring::toString(12345, buf).str, "12345");
    ret &= checkString("toString(uint32_t i..)", lstring::toString(12345, buf).str, "12345");
    ret &= checkString("toString(uint8_t i..)", lstring::toString(254, buf).str, "254");

    ret &= checkInt("toInt(0)", lstring(buf, sizeof(buf)).sprintf("%d", 0).toInt(), 0);
    ret &= checkInt("toInt(123)", lstring(buf, sizeof(buf)).sprintf("%d", 123).toInt(), 123);
    ret &= checkInt("toInt(some thing)", lstring(buf, sizeof(buf)).cat("some thing").toInt(), 0);

    ret &= checkInt("toDouble(0)", lstring(buf, sizeof(buf)).sprintf("%d", 0).toDouble(), 0.0);
    ret &= checkInt("toDouble(123)", lstring(buf, sizeof(buf)).sprintf("%d", 123).toDouble(), 123.0);
    ret &= checkInt("toDouble()", lstring(buf, sizeof(buf)).cat("some thing").toDouble(), 0.0);

    return ret;
}

bool testStream() {
    bool ret = true;

    char buf[80];
    char buf2[40];
    char num[16];
    lstring a(buf, sizeof(buf));
    int i = 12;
    char first[] = "first";
    ret &= checkString("Streaming expr",
                       lstring(buf, 80)
                           .sprintf("input %d", i)
                           .cat(" is ")
                           .cat(lstring(buf2, 40).sprintf("the %s", first))
                           .cat(" one.")
                           .str,
                       "input 12 is the first one.");

    return ret;
}

int main(int argc, char* argv[]) {
    bool result = true;
    result &= testCtor();
    result &= testAllocateClearDrop();
    result &= testCatCpy();
    result &= testTrim();
    result &= testEqualStartWith();
    result &= testToStringIntDouble();
    result &= testStream();
    return result ? 0 : 1;
}
