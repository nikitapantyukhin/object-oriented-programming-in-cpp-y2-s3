#include "src/temperature_set.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

bool readInt(const char* prompt, int& result) {
    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        int value;
        char extra;

        if ((input >> value) && !(input >> extra)) {
            result = value;
            return true;
        }

        std::cout << "Ошибка! Введите целое число.\n";
    }
}

bool readDouble(const char* prompt, double& result) {
    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        double value;
        char extra;

        if ((input >> value) && !(input >> extra) &&
            std::isfinite(value)) {
            result = value;
            return true;
        }

        std::cout << "Ошибка! Введите корректное число.\n";
    }
}

bool readTemperature(Temperature& result) {
    double value;

    if (!readDouble("Введите значение: ", value)) {
        return false;
    }

    std::cout << "\nВыберите единицу измерения:\n";
    std::cout << "1. Цельсий (°C)\n";
    std::cout << "2. Фаренгейт (°F)\n";
    std::cout << "3. Кельвин (K)\n";

    while (true) {
        int choice;

        if (!readInt("Ваш выбор: ", choice)) {
            return false;
        }

        Temperature::Unit unit;

        switch (choice) {
            case 1:
                unit = Temperature::Unit::Celsius;
                break;

            case 2:
                unit = Temperature::Unit::Fahrenheit;
                break;

            case 3:
                unit = Temperature::Unit::Kelvin;
                break;

            default:
                std::cout << "Ошибка! Выберите 1, 2 или 3.\n";
                continue;
        }

        result = Temperature(value, unit);
        return true;
    }
}

void printMenu() {
    std::cout << "\nМЕНЮ\n";
    std::cout << "1.  Добавить температуру в A\n";
    std::cout << "2.  Добавить температуру в B\n";
    std::cout << "3.  Удалить температуру из A\n";
    std::cout << "4.  Удалить температуру из B\n";
    std::cout << "5.  Проверить наличие температуры в A\n";
    std::cout << "6.  Проверить наличие температуры в B\n";
    std::cout << "7.  Показать множества A и B\n";
    std::cout << "8.  Объединение A | B\n";
    std::cout << "9.  Пересечение A & B\n";
    std::cout << "10. Разность A - B\n";
    std::cout << "11. Разность B - A\n";
    std::cout << "12. Сравнить множества A и B\n";
    std::cout << "0.  Выход\n\n";
}

int main() {
    TemperatureSet A;
    TemperatureSet B;

    while (true) {
        printMenu();

        int choice;

        if (!readInt("Ваш выбор: ", choice)) {
            std::cout << "\nВвод завершён.\n";
            break;
        }

        if (choice == 0) {
            std::cout << "Завершение программы.\n";
            break;
        }

        try {
            if (choice >= 1 && choice <= 6) {
                Temperature temperature;

                if (!readTemperature(temperature)) {
                    std::cout << "\nВвод завершён.\n";
                    break;
                }

                TemperatureSet& set =(choice % 2 == 1) ? A : B;

                if (choice <= 2) {
                    if (set.add(temperature)) {
                        std::cout << "Температура добавлена.\n";
                    } else {
                        std::cout << "Такая температура уже есть.\n";
                    }
                }
                else if (choice <= 4) {
                    if (set.remove(temperature)) {
                        std::cout << "Температура удалена.\n";
                    } else {
                        std::cout << "Температура не найдена.\n";
                    }
                }
                else {
                    if (set.contains(temperature)) {
                        std::cout << "Температура найдена.\n";
                    } else {
                        std::cout << "Температура не найдена.\n";
                    }
                }
            }
            else {
                switch (choice) {
                    case 7:
                        std::cout << "A = " << A << " (size = " << A.size() << ")\n";
                        std::cout << "B = " << B << " (size = " << B.size() << ")\n";
                        break;

                    case 8:
                        std::cout << "A | B = " << (A | B) << '\n';
                        break;

                    case 9:
                        std::cout << "A & B = " << (A & B) << '\n';
                        break;

                    case 10:
                        std::cout << "A - B = " << (A - B) << '\n';
                        break;

                    case 11:
                        std::cout << "B - A = " << (B - A) << '\n';
                        break;

                    case 12:
                        std::cout << "A == B: " << (A == B ? "true" : "false") << '\n';

                        std::cout << "A != B: " << (A != B ? "true" : "false") << '\n';
                        break;

                    default:
                        std::cout << "Неверный пункт меню.\n";
                        break;
                }
            }
        }
        catch (const std::exception& error) {
            std::cout << "Ошибка: " << error.what() << '\n';
        }
    }

    return 0;
}
