#include "src/bank_collection.h"
#include "src/deposit.h"
#include "src/credit.h"
#include "src/card.h"

#include <iostream>
#include <stdexcept>

int readInt(const char* message) {
    int number;
    std::cout << message;
    if (!(std::cin >> number)) {
        throw std::invalid_argument("Нужно ввести целое число");
    }
    return number;
}

double readDouble(const char* message) {
    double number;
    std::cout << message;
    if (!(std::cin >> number) || number < 0) {
        throw std::invalid_argument("Нужно ввести неотрицательное число");
    }
    return number;
}

const char* productName(const BankProduct& product) {
    if (dynamic_cast<const Deposit*>(&product)) return "Вклад";
    if (dynamic_cast<const Credit*>(&product)) return "Кредит";
    if (dynamic_cast<const Card*>(&product)) return "Карта";
    return "Неизвестный продукт";
}

void showProducts(const BankCollection& bank) {
    if (bank.size() == 0) {
        std::cout << "Список продуктов пуст.\n";
        return;
    }

    for (std::size_t i = 0; i < bank.size(); ++i) {
        const BankProduct& product = bank.at(i);
        std::cout << i + 1 << ". " << productName(product)
                  << " | Данные: " << product
                  << " | Метрика: " << product.metric() << '\n';
    }
}

std::size_t readIndex(const BankCollection& bank) {
    if (bank.size() == 0) {
        throw std::out_of_range("Список продуктов пуст");
    }

    showProducts(bank);
    int number = readInt("Введите номер продукта: ");
    if (number < 1 || static_cast<std::size_t>(number) > bank.size()) {
        throw std::out_of_range("Продукта с таким номером нет");
    }
    return static_cast<std::size_t>(number - 1);
}

void showMenu() {
    std::cout << "\nМЕНЮ\n"
              << "1. Добавить вклад\n"
              << "2. Добавить кредит\n"
              << "3. Добавить карту\n"
              << "4. Показать все продукты\n"
              << "5. Посчитать сумму метрик\n"
              << "6. Найти максимальную метрику\n"
              << "7. Удалить продукт\n"
              << "8. Показать свойство продукта\n"
              << "9. Сравнить два продукта\n"
              << "10. Преобразовать продукт в число\n"
              << "0. Выход\n";
}

int main() {
    BankCollection bank;

    while (true) {
        showMenu();

        try {
            int choice = readInt("Выберите действие: ");

            switch (choice) {
                case 1: {
                    double sum = readDouble("Сумма вклада: ");
                    double rate = readDouble("Ставка в процентах: ") / 100.0;
                    int years = readInt("Срок в годах: ");
                    int flag = readInt("Капитализация (1 - да, 0 - нет): ");

                    if (years <= 0 || (flag != 0 && flag != 1)) {
                        throw std::invalid_argument("Неверный срок или капитализация");
                    }

                    bank.add(Deposit(sum, rate, years, flag == 1));
                    std::cout << "Вклад добавлен.\n";
                    break;
                }
                case 2: {
                    double sum = readDouble("Сумма кредита: ");
                    double rate = readDouble("Ставка в процентах: ") / 100.0;
                    int years = readInt("Срок в годах: ");
                    int flag = readInt("Аннуитет (1 - да, 0 - нет): ");

                    if (years <= 0 || (flag != 0 && flag != 1)) {
                        throw std::invalid_argument("Неверный срок или аннуитет");
                    }

                    bank.add(Credit(sum, rate, years, flag == 1));
                    std::cout << "Кредит добавлен.\n";
                    break;
                }
                case 3: {
                    double spending = readDouble("Расходы по карте: ");
                    double rate = readDouble("Кешбэк в процентах: ") / 100.0;

                    bank.add(Card(spending, rate));
                    std::cout << "Карта добавлена.\n";
                    break;
                }
                case 4:
                    showProducts(bank);
                    break;

                case 5:
                    std::cout << "Сумма метрик: " << bank.totalMetric() << '\n';
                    break;

                case 6: {
                    const BankProduct* best = bank.maxMetric();
                    if (best == nullptr) {
                        std::cout << "Список продуктов пуст.\n";
                    } else {
                        std::cout << "Максимум: " << productName(*best) << " | Метрика: " << best->metric() << '\n';
                    }
                    break;
                }
                case 7: {
                    std::size_t index = readIndex(bank);
                    bank.remove(index);
                    std::cout << "Продукт удалён.\n";
                    break;
                }
                case 8: {
                    const BankProduct& product = bank.at(readIndex(bank));

                    if (const Deposit* d = dynamic_cast<const Deposit*>(&product)) {
                        std::cout << "Капитализация: " << (d->isCumulative() ? "да" : "нет") << '\n';
                    } else if (const Credit* c = dynamic_cast<const Credit*>(&product)) {
                        std::cout << "Аннуитет: " << (c->isAnnuity() ? "да" : "нет") << '\n';
                    } else if (const Card* c = dynamic_cast<const Card*>(&product)) {
                        std::cout << "Кешбэк: " << (c->hasCashback() ? "есть" : "нет") << '\n';
                    }
                    break;
                }
                case 9: {
                    std::cout << "Первый продукт:\n";
                    std::size_t first = readIndex(bank);
                    std::cout << "Второй продукт:\n";
                    std::size_t second = readIndex(bank);

                    if (bank.at(first) == bank.at(second)) {
                        std::cout << "Метрики равны.\n";
                    } else {
                        std::cout << "Метрики различаются.\n";
                    }
                    break;
                }
                case 10: {
                    const BankProduct& product = bank.at(readIndex(bank));
                    std::cout << "Значение: " << static_cast<double>(product) << '\n';
                    break;
                }
                case 0:
                    std::cout << "Программа завершена.\n";
                    return 0;

                default:
                    std::cout << "Неизвестная команда.\n";
            }
        } catch (const std::exception& e) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "Ошибка: " << e.what() << '\n';
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    return 0;
}
