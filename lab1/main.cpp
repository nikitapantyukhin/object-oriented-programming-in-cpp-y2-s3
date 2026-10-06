#include "str_ops.h"

#include <cstddef>
#include <iostream>
#include <limits>

namespace {
    constexpr std::size_t INITIAL_BUFFER_CAPACITY = 64;
    constexpr std::size_t BUFFER_GROWTH_FACTOR = 2;
}

char* read_line() {
    std::size_t capacity = INITIAL_BUFFER_CAPACITY;
    std::size_t length = 0;

    char* buffer = new char[capacity];
    char ch;

    while (std::cin.get(ch) && ch != '\n') {
        if (length + 1 >= capacity) {
            const std::size_t new_capacity = capacity * BUFFER_GROWTH_FACTOR;
            char* new_buffer = new char[new_capacity];

            for (std::size_t i = 0; i < length; i++) {
                new_buffer[i] = buffer[i];
            }

            delete[] buffer;

            buffer = new_buffer;
            capacity = new_capacity ;
        }

        buffer[length] = ch;
        length++;
    }

    buffer[length] = '\0';

    char* result = str_alloc(buffer);
    delete[] buffer;

    return result;
}

bool ensure_string_exists(const char* text) {
    if (text == nullptr) {
        std::cout << "Строка пустая!\n";
        return false;
    }

    return true;
}

void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool read_char(char& ch) {
    std::cout << "Введите символ: ";

    if (!std::cin.get(ch)) {
        return false;
    }

    if (ch == '\n') {
        std::cout << "Ошибка: символ не введен\n";
        return false;
    }

    char next;

    if (!std::cin.get(next)) {
        return false;
    }

    if (next != '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Введите только один символ!\n";
        return false;
    }

    return true;
}

bool read_menu_choice(int& choice) {
    std::cout << "Ваш выбор: ";
    
    if (!(std::cin >> choice)) {
        clear_input();
        std::cout << "Введите целое число\n";
        return false;
    }

    char next;

    if (!std::cin.get(next)) {
        return false;
    }

    if (next != '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Введите только одно целое число\n";
        return false;
    }

    return true;
}

void print_menu() {
    std::cout << '\n';
    std::cout << "1. Ввести строку\n";
    std::cout << "2. Напечатать строку\n";
    std::cout << "3. Узнать длину строки\n";
    std::cout << "4. Скопировать строку и напечатать копию\n";
    std::cout << "5. Перевести строку в верхний регистр\n";
    std::cout << "6. Подсчитать количество символов\n";
    std::cout << "0. Выход\n";
}

int main() {
    char* text = nullptr;

    while (true) {
        print_menu();

        int choice = 0;

        if (!read_menu_choice(choice)) {
            continue;
        }

        if (choice == 0) {
            str_delete(text);
            return 0;
        }

        else if (choice == 1) {
            str_delete(text);
            std::cout << "Введите строку: ";
            text = read_line();
            std::cout << "Строка сохранена\n";
        }

        else if (choice == 2) {
            str_print(text);
        }

        else if (choice == 3) {
            if (!ensure_string_exists(text)) {
                continue;
            }

            const std::size_t length = str_len(text);
            std::cout << "Длина строки: " << length << '\n';
        }

        else if (choice == 4) {
            if (!ensure_string_exists(text)) {
                continue;
            }

            char* temp = str_alloc(text);
            str_print(temp);
            str_delete(temp);
        }

        else if (choice == 5) {
            if (!ensure_string_exists(text)) {
                continue;
            }

            str_to_upper(text);
            std::cout << "Строка в верхнем регистре: ";
            str_print(text);
        }

        else if (choice == 6) {
            if (!ensure_string_exists(text)) {
                continue;
            }

            char ch;

            if (!read_char(ch)) {
                continue;
            }

            const std::size_t count = str_count_char(text, ch);

            std::cout << "Количество символов '" << ch << "' : " << count << '\n';
        }

        else {
            std::cout << "Такого выбора в меню нет!\n";
        }
    }
}