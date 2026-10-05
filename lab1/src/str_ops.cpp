#include "str_ops.h"

#include <iostream>

std::size_t str_len(const char* src) {
    if (src == nullptr) {
        return 0;
    }

    std::size_t len = 0;

    while (src[len] != '\0') {
        len++;
    }

    return len;
}

void str_copy(char* dst, const char* src) {
    if (dst == nullptr || src == nullptr) {
        return;
    }

    std::size_t i = 0;

    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';
}

char* str_alloc(const char* src) {
    if (src == nullptr) {
        return nullptr;
    }
    
    const std::size_t len = str_len(src);
    char* result = new char[len + 1];
    str_copy(result, src);

    return result;
}

void str_delete(char*& s) {
    delete[] s;
    s = nullptr;
}

void str_print(const char* s) {
    if (s == nullptr) {
        std::cout << "Строка пустая!\n";
        return;
    }

    std::cout << s << '\n';
}

void str_to_upper(char* s) {
    if (s == nullptr) {
        return;
    }

    std::size_t i = 0;

    while (s[i] != '\0') {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] -= ('a' - 'A');
        }

        i++;
    }
}

std::size_t str_count_char(const char* s, char ch) {
    if (s == nullptr) {
        return 0;
    }

    std::size_t count = 0;
    std::size_t i = 0;

    while (s[i] != '\0') {
        if (s[i] == ch) {
            count++;
        }

        i++;
    }

    return count;
}