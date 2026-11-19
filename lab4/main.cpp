#include "src/bank_collection.h"
#include "src/card.h"
#include "src/credit.h"
#include "src/deposit.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>

bool readNumber(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        int number;
        std::string extra;

        if ((input >> number) && !(input >> extra)) {
            value = number;
            return true;
        }

        std::cout << "Ошибка: введите одно корректное целое число.\n";
    }
}

bool readNumber(const std::string& prompt, long long& value) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        long long number;
        std::string extra;

        if ((input >> number) && !(input >> extra)) {
            value = number;
            return true;
        }

        std::cout << "Ошибка: введите одно корректное целое число.\n";
    }
}

bool readNumber(const std::string& prompt, double& value) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        double number;
        std::string extra;

        if ((input >> number) && !(input >> extra)) {
            value = number;
            return true;
        }

        std::cout << "Ошибка: введите одно корректное число.\n";
    }
}

bool readNonNegative(const std::string& prompt, double& value) {
    while (true) {
        if (!readNumber(prompt, value)) {
            return false;
        }
        if (std::isfinite(value) && value >= 0.0) {
            return true;
        }
        std::cout << "Ошибка: число должно быть конечным и неотрицательным.\n";
    }
}

bool readPositiveYears(int& years) {
    while (true) {
        if (!readNumber("Срок в годах (целое положительное число): ", years)) {
            return false;
        }
        if (years > 0) {
            return true;
        }
        std::cout << "Ошибка: срок должен быть больше нуля.\n";
    }
}

bool readFlag(const std::string& prompt, bool& value) {
    int number = 0;
    while (true) {
        if (!readNumber(prompt, number)) {
            return false;
        }
        if (number == 0 || number == 1) {
            value = (number == 1);
            return true;
        }
        std::cout << "Ошибка: введите 0 или 1.\n";
    }
}

bool readRate(double& rate) {
    double percent = 0.0;
    if (!readNonNegative("Годовая ставка в процентах (например, 5): ", percent)) {
        return false;
    }
    rate = percent / 100.0;
    return true;
}

const char* productName(const BankProduct& product) {
    if (dynamic_cast<const Deposit*>(&product) != nullptr) {
        return "Вклад";
    }
    if (dynamic_cast<const Credit*>(&product) != nullptr) {
        return "Кредит";
    }
    if (dynamic_cast<const Card*>(&product) != nullptr) {
        return "Карта";
    }
    return "Неизвестный продукт";
}

void showProduct(const BankProduct& product, std::size_t number) {
    std::cout << number << ". " << productName(product) << " | Финансовый показатель: " << product.metric() << '\n';
}

void showProducts(const BankCollection& collection) {
    if (collection.size() == 0) {
        std::cout << "Список банковских продуктов пуст.\n";
        return;
    }

    std::cout << "\nБанковские продукты:\n";
    for (std::size_t i = 0; i < collection.size(); ++i) {
        showProduct(collection.at(i), i + 1);
    }
}

bool readProductIndex(const BankCollection& collection, std::size_t& index) {
    if (collection.size() == 0) {
        std::cout << "Список банковских продуктов пуст.\n";
        return true;
    }

    showProducts(collection);
    while (true) {
        long long number = 0;
        if (!readNumber("Введите номер продукта: ", number)) {
            return false;
        }
        if (number > 0 &&
            static_cast<unsigned long long>(number) <= collection.size()) {
            index = static_cast<std::size_t>(number - 1);
            return true;
        }
        std::cout << "Ошибка: продукта с таким номером нет.\n";
    }
}

bool addDeposit(BankCollection& collection) {
    double sum = 0.0;
    double rate = 0.0;
    int years = 0;
    bool cumulative = false;

    if (!readNonNegative("Сумма вклада: ", sum) ||
        !readRate(rate) ||
        !readPositiveYears(years) ||
        !readFlag("Капитализация (1 — да, 0 — нет): ", cumulative)) {
        return false;
    }

    collection.add(Deposit(sum, rate, years, cumulative));
    std::cout << "Вклад успешно добавлен.\n";
    return true;
}

bool addCredit(BankCollection& collection) {
    double sum = 0.0;
    double rate = 0.0;
    int years = 0;
    bool annuity = false;

    if (!readNonNegative("Сумма кредита: ", sum) ||
        !readRate(rate) ||
        !readPositiveYears(years) ||
        !readFlag("Аннуитетный расчёт (1 — да, 0 — нет): ", annuity)) {
        return false;
    }

    collection.add(Credit(sum, rate, years, annuity));
    std::cout << "Кредит успешно добавлен.\n";
    return true;
}

bool addCard(BankCollection& collection) {
    double spending = 0.0;
    double rate = 0.0;

    if (!readNonNegative("Сумма расходов по карте: ", spending) ||
        !readNonNegative("Ставка кешбэка в процентах (например, 3): ", rate)) {
        return false;
    }

    collection.add(Card(spending, rate / 100.0));
    std::cout << "Карта успешно добавлена.\n";
    return true;
}

bool removeProduct(BankCollection& collection) {
    if (collection.size() == 0) {
        std::cout << "Удалять нечего: список пуст.\n";
        return true;
    }
    std::size_t index = 0;
    if (!readProductIndex(collection, index)) {
        return false;
    }
    collection.remove(index);
    std::cout << "Продукт удалён.\n";
    return true;
}

bool showSpecialMethod(const BankCollection& collection) {
    if (collection.size() == 0) {
        std::cout << "Список банковских продуктов пуст.\n";
        return true;
    }

    std::size_t index = 0;
    if (!readProductIndex(collection, index)) {
        return false;
    }

    const BankProduct& product = collection.at(index);
    if (const auto* deposit = dynamic_cast<const Deposit*>(&product)) {
        std::cout << "Капитализация: " << (deposit->isCumulative() ? "есть" : "нет") << '\n';
    } else if (const auto* credit = dynamic_cast<const Credit*>(&product)) {
        std::cout << "Аннуитетный расчёт: " << (credit->isAnnuity() ? "да" : "нет") << '\n';
    } else if (const auto* card = dynamic_cast<const Card*>(&product)) {
        std::cout << "Кешбэк: " << (card->hasCashback() ? "есть" : "нет") << '\n';
    }
    return true;
}

bool compareProducts(const BankCollection& collection) {
    if (collection.size() < 2) {
        std::cout << "Для сравнения нужно хотя бы два продукта.\n";
        return true;
    }

    std::size_t first = 0;
    std::size_t second = 0;
    std::cout << "Первый продукт:\n";
    if (!readProductIndex(collection, first)) {
        return false;
    }
    std::cout << "Второй продукт:\n";
    if (!readProductIndex(collection, second)) {
        return false;
    }

    std::cout << (collection.at(first) == collection.at(second) ? "Метрики продуктов равны с заданной точностью.\n"
                      : "Метрики продуктов различаются.\n");
    return true;
}

bool showAsNumber(const BankCollection& collection) {
    if (collection.size() == 0) {
        std::cout << "Список банковских продуктов пуст.\n";
        return true;
    }

    std::size_t index = 0;
    if (!readProductIndex(collection, index)) {
        return false;
    }

    std::cout << "Значение после преобразования в число: " << static_cast<double>(collection.at(index)) << '\n';
    return true;
}

void showMenu() {
    std::cout << "\nМЕНЮ:\n"
              << "1. Добавить вклад\n"
              << "2. Добавить кредит\n"
              << "3. Добавить карту\n"
              << "4. Показать все продукты\n"
              << "5. Рассчитать суммарную метрику\n"
              << "6. Найти продукт с максимальной метрикой\n"
              << "7. Удалить продукт\n"
              << "8. Проверить специальное свойство продукта\n"
              << "9. Сравнить два продукта\n"
              << "10. Преобразовать продукт в число\n"
              << "0. Выход\n";
}

int main() {
    BankCollection collection;
    std::cout << std::setprecision(12);

    while (true) {
        showMenu();

        int choice = 0;
        if (!readNumber("Выберите действие: ", choice)) {
            std::cout << "\nВвод завершён.\n";
            break;
        }

        try {
            bool inputOk = true;

            switch (choice) {
                case 1:
                    inputOk = addDeposit(collection);
                    break;
                case 2:
                    inputOk = addCredit(collection);
                    break;
                case 3:
                    inputOk = addCard(collection);
                    break;
                case 4:
                    showProducts(collection);
                    break;
                case 5:
                    std::cout << "Суммарная метрика: " << collection.totalMetric() << '\n';
                    break;
                case 6: {
                    const BankProduct* best = collection.maxMetric();
                    if (best == nullptr) {
                        std::cout << "Список банковских продуктов пуст.\n";
                    } else {
                        std::cout << "Продукт с максимальной метрикой: " << productName(*best) << '\n'
                                  << "Значение метрики: " << best->metric() << '\n';
                    }
                    break;
                }
                case 7:
                    inputOk = removeProduct(collection);
                    break;
                case 8:
                    inputOk = showSpecialMethod(collection);
                    break;
                case 9:
                    inputOk = compareProducts(collection);
                    break;
                case 10:
                    inputOk = showAsNumber(collection);
                    break;
                case 0:
                    std::cout << "Программа завершена.\n";
                    return 0;
                default:
                    std::cout << "Ошибка: неизвестная команда.\n";
                    break;
            }

            if (!inputOk) {
                std::cout << "\nВвод завершён.\n";
                break;
            }
        } catch (const std::exception&) {
            std::cout << "Не удалось выполнить операцию. Проверьте введённые данные.\n";
        }
    }

    return 0;
}
