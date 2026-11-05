#include "polynomial.h"

#include <iostream>
#include <limits>
#include <stdexcept>

Polynomial inputPolynomial(char name) {
    int degree;

    std::cout << "Введите степень многочлена " << name << ": ";
    if (!(std::cin >> degree) || degree < -1) {
        throw std::invalid_argument("Некорректная степень");
    }

    if (degree == -1) {
        return Polynomial();
    }

    double* coeffs = new double[degree + 1];

    try {
        for (int i = 0; i <= degree; ++i) {
            std::cout << "Введите коэффициент при x^" << i << ": ";

            if (!(std::cin >> coeffs[i])) {
                throw std::invalid_argument("Некорректный коэффициент");
            }
        }

        Polynomial result(coeffs, degree);
        delete[] coeffs;

        return result;
    } catch (...) {
        delete[] coeffs;
        throw;
    }
}


int main() {
    Polynomial a;
    Polynomial b;

    int choice;

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Ввести многочлен A\n";
        std::cout << "2. Ввести многочлен B\n";
        std::cout << "3. Показать многочлены\n";
        std::cout << "4. Сложить A + B\n";
        std::cout << "5. Умножить A * B\n";
        std::cout << "6. Возвести A в степень n\n";
        std::cout << "7. Возвести B в степень n\n";
        std::cout << "8. Вычислить значение A(x)\n";
        std::cout << "9. Вычислить значение B(x)\n";
        std::cout << "10. Скопировать A в B\n";
        std::cout << "11. Переместить A в B\n";
        std::cout << "0. Выход\n\n";
        std::cout << "Выберите действие: ";

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Некорректный ввод\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    a = inputPolynomial('A');
                    std::cout << "A(x) = ";
                    a.print(std::cout);
                    std::cout << '\n';
                    break;
                }

                case 2: {
                    b = inputPolynomial('B');
                    std::cout << "B(x) = ";
                    b.print(std::cout);
                    std::cout << '\n';
                    break;
                }

                case 3: {
                    std::cout << "A(x) = ";
                    a.print(std::cout);
                    std::cout << "\nB(x) = ";
                    b.print(std::cout);
                    std::cout << '\n';
                    break;
                }

                case 4: {
                    Polynomial result(a);
                    result.add(b);

                    std::cout << "A(x) + B(x) = ";
                    result.print(std::cout);
                    std::cout << '\n';
                    break;
                }

                case 5: {
                    Polynomial result(a);
                    result.mul(b);

                    std::cout << "A(x) * B(x) = ";
                    result.print(std::cout);
                    std::cout << '\n';
                    break;
                }

                case 6:
                case 7: {
                    int n;
                    std::cout << "Введите степень n: ";

                    if (!(std::cin >> n) || n < 0) {
                        throw std::invalid_argument("Некорректная степень");
                    }

                    const Polynomial& p = (choice == 6) ? a : b;
                    char name = (choice == 6) ? 'A' : 'B';

                    Polynomial result = p.pow(n);

                    std::cout << name << "(x)^" << n << " = ";
                    result.print(std::cout);
                    std::cout << '\n';
                    break;
                }

                case 8:
                case 9: {
                    double x;
                    std::cout << "Введите x: ";

                    if (!(std::cin >> x)) {
                        throw std::invalid_argument("Некорректное значение x");
                    }

                    const Polynomial& p = (choice == 8) ? a : b;
                    char name = (choice == 8) ? 'A' : 'B';

                    std::cout << name << "(" << x << ") = "
                              << p.evaluate(x) << '\n';
                    break;
                }
                
                case 10: {
                    b = a;

                    std::cout << "Многочлен A скопирован в B\n";

                    std::cout << "A(x) = ";
                    a.print(std::cout);

                    std::cout << "\nB(x) = ";
                    b.print(std::cout);

                    std::cout << '\n';
                    break;
                }

                case 11: {
                    b = std::move(a);

                    std::cout << "Многочлен A перемещён в B\n";

                    std::cout << "A(x) = ";
                    a.print(std::cout);

                    std::cout << "\nB(x) = ";
                    b.print(std::cout);

                    std::cout << '\n';
                    break;
                }

                case 0:
                    return 0;

                default:
                    std::cout << "Неизвестное действие\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << '\n';

            if (std::cin.eof()) {
                break;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
