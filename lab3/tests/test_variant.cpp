#include <gtest/gtest.h>

#include "temperature.h"
#include "temperature_set.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

using Unit = Temperature::Unit;

// Конструктор по умолчанию
TEST(TemperatureTest, DefaultConstructor) {
    Temperature a;
    Temperature b(0, Unit::Celsius);

    EXPECT_TRUE(a == b);
}

// Канонизация температур
TEST(TemperatureTest, Canonicalization) {
    Temperature a(0, Unit::Celsius);
    Temperature b(32, Unit::Fahrenheit);
    Temperature c(273.15, Unit::Kelvin);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == c);
    EXPECT_TRUE(a == c);
}

// Сравнение разных температур
TEST(TemperatureTest, DifferentValues) {
    Temperature a(20, Unit::Celsius);
    Temperature b(30, Unit::Celsius);

    EXPECT_FALSE(a == b);
}

// Проверка некорректных значений
TEST(TemperatureTest, InvalidValues) {
    EXPECT_THROW(
        Temperature(std::numeric_limits<double>::infinity(),
                    Unit::Celsius),
        std::invalid_argument
    );

    EXPECT_THROW(
        Temperature(std::numeric_limits<double>::quiet_NaN(),
                    Unit::Celsius),
        std::invalid_argument
    );

    EXPECT_THROW(
        Temperature(20, static_cast<Unit>(100)),
        std::invalid_argument
    );
}

// Вывод температуры
TEST(TemperatureTest, Output) {
    Temperature a(20, Unit::Celsius);
    Temperature b(68, Unit::Fahrenheit);
    Temperature c(300, Unit::Kelvin);

    std::ostringstream out;
    out << a << ", " << b << ", " << c;

    EXPECT_EQ(out.str(), "20°C, 68°F, 300 K");
}

// Пустое множество
TEST(TemperatureSetTest, EmptySet) {
    TemperatureSet set;

    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0u);
    EXPECT_FALSE(set.contains(Temperature(10, Unit::Celsius)));
}

// Добавление элементов и дубликатов
TEST(TemperatureSetTest, AddAndDuplicates) {
    TemperatureSet set;

    EXPECT_TRUE(set.add(Temperature(20, Unit::Celsius)));
    EXPECT_FALSE(set.add(Temperature(68, Unit::Fahrenheit)));
    EXPECT_TRUE(set.add(Temperature(30, Unit::Celsius)));

    EXPECT_EQ(set.size(), 2u);
}

// Поиск и operator()
TEST(TemperatureSetTest, ContainsAndCallOperator) {
    TemperatureSet set;

    set.add(Temperature(100, Unit::Celsius));

    EXPECT_TRUE(set.contains(Temperature(212, Unit::Fahrenheit)));
    EXPECT_TRUE(set(Temperature(373.15, Unit::Kelvin)));

    EXPECT_FALSE(set.contains(Temperature(50, Unit::Celsius)));
    EXPECT_FALSE(set(Temperature(50, Unit::Celsius)));
}

// Удаление существующего элемента
TEST(TemperatureSetTest, RemoveExisting) {
    TemperatureSet set;

    set.add(Temperature(10, Unit::Celsius));
    set.add(Temperature(20, Unit::Celsius));
    set.add(Temperature(30, Unit::Celsius));

    EXPECT_TRUE(set.remove(Temperature(68, Unit::Fahrenheit)));

    EXPECT_EQ(set.size(), 2u);
    EXPECT_FALSE(set.contains(Temperature(20, Unit::Celsius)));
}

// Удаление с сохранением порядка
TEST(TemperatureSetTest, RemovePreservesOrder) {
    TemperatureSet set;

    set.add(Temperature(10, Unit::Celsius));
    set.add(Temperature(20, Unit::Celsius));
    set.add(Temperature(30, Unit::Celsius));

    set.remove(Temperature(20, Unit::Celsius));

    std::ostringstream out;
    out << set;

    EXPECT_EQ(out.str(), "{ 10°C, 30°C }");
}

// Удаление отсутствующего элемента
TEST(TemperatureSetTest, RemoveMissing) {
    TemperatureSet set;

    set.add(Temperature(10, Unit::Celsius));

    EXPECT_FALSE(set.remove(Temperature(50, Unit::Celsius)));
    EXPECT_EQ(set.size(), 1u);
}

// Конструктор копирования
TEST(TemperatureSetTest, CopyConstructor) {
    TemperatureSet original;
    original.add(Temperature(10, Unit::Celsius));

    TemperatureSet copy(original);
    copy.add(Temperature(20, Unit::Celsius));

    EXPECT_EQ(original.size(), 1u);
    EXPECT_EQ(copy.size(), 2u);
}

// Копирующее присваивание
TEST(TemperatureSetTest, CopyAssignment) {
    TemperatureSet a;
    TemperatureSet b;

    a.add(Temperature(10, Unit::Celsius));
    b.add(Temperature(20, Unit::Celsius));

    b = a;
    b.add(Temperature(30, Unit::Celsius));

    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(b.size(), 2u);

    a = a;
    EXPECT_EQ(a.size(), 1u);
}

// Перемещающий конструктор
TEST(TemperatureSetTest, MoveConstructor) {
    TemperatureSet original;
    original.add(Temperature(10, Unit::Celsius));

    TemperatureSet moved(std::move(original));

    EXPECT_TRUE(original.empty());
    EXPECT_EQ(original.size(), 0u);

    EXPECT_EQ(moved.size(), 1u);
    EXPECT_TRUE(moved.contains(Temperature(10, Unit::Celsius)));
}

// Перемещающее присваивание
TEST(TemperatureSetTest, MoveAssignment) {
    TemperatureSet a;
    TemperatureSet b;

    a.add(Temperature(10, Unit::Celsius));
    b.add(Temperature(20, Unit::Celsius));

    b = std::move(a);

    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.size(), 1u);
    EXPECT_TRUE(b.contains(Temperature(10, Unit::Celsius)));

    b = std::move(b);
    EXPECT_EQ(b.size(), 1u);
}

// Операторы += и -=
TEST(TemperatureSetTest, AddRemoveOperators) {
    TemperatureSet set;

    set += Temperature(10, Unit::Celsius);
    set += Temperature(20, Unit::Celsius);

    EXPECT_EQ(set.size(), 2u);

    set -= Temperature(10, Unit::Celsius);

    EXPECT_EQ(set.size(), 1u);
    EXPECT_TRUE(set.contains(Temperature(20, Unit::Celsius)));
}

// Объединение множеств
TEST(TemperatureSetTest, Union) {
    TemperatureSet a;
    TemperatureSet b;

    a.add(Temperature(10, Unit::Celsius));
    a.add(Temperature(20, Unit::Celsius));

    b.add(Temperature(68, Unit::Fahrenheit));
    b.add(Temperature(30, Unit::Celsius));

    TemperatureSet result = a | b;

    EXPECT_EQ(result.size(), 3u);
    EXPECT_TRUE(result.contains(Temperature(10, Unit::Celsius)));
    EXPECT_TRUE(result.contains(Temperature(20, Unit::Celsius)));
    EXPECT_TRUE(result.contains(Temperature(30, Unit::Celsius)));

    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 2u);
}

// Пересечение множеств
TEST(TemperatureSetTest, Intersection) {
    TemperatureSet a;
    TemperatureSet b;

    a.add(Temperature(10, Unit::Celsius));
    a.add(Temperature(20, Unit::Celsius));

    b.add(Temperature(68, Unit::Fahrenheit));
    b.add(Temperature(30, Unit::Celsius));

    TemperatureSet result = a & b;

    EXPECT_EQ(result.size(), 1u);
    EXPECT_TRUE(result.contains(Temperature(20, Unit::Celsius)));
}

// Разность множеств
TEST(TemperatureSetTest, Difference) {
    TemperatureSet a;
    TemperatureSet b;

    a.add(Temperature(10, Unit::Celsius));
    a.add(Temperature(20, Unit::Celsius));

    b.add(Temperature(68, Unit::Fahrenheit));
    b.add(Temperature(30, Unit::Celsius));

    TemperatureSet result = a - b;

    EXPECT_EQ(result.size(), 1u);
    EXPECT_TRUE(result.contains(Temperature(10, Unit::Celsius)));
}

// Равенство независимо от порядка
TEST(TemperatureSetTest, Equality) {
    TemperatureSet a;
    TemperatureSet b;

    a.add(Temperature(10, Unit::Celsius));
    a.add(Temperature(20, Unit::Celsius));

    b.add(Temperature(68, Unit::Fahrenheit));
    b.add(Temperature(50, Unit::Fahrenheit));

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b.add(Temperature(30, Unit::Celsius));

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Вывод множества
TEST(TemperatureSetTest, Output) {
    TemperatureSet set;

    set.add(Temperature(10, Unit::Celsius));
    set.add(Temperature(68, Unit::Fahrenheit));

    std::ostringstream out;
    out << set;

    EXPECT_EQ(out.str(), "{ 10°C, 68°F }");
}

// Стресс-тест: 1000 добавлений
TEST(TemperatureSetTest, StressTest) {
    TemperatureSet set;

    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(set.add(Temperature(i, Unit::Celsius)));
    }

    EXPECT_EQ(set.size(), 1000u);

    for (int i = 0; i < 1000; ++i) {
        EXPECT_FALSE(set.add(Temperature(i, Unit::Celsius)));
    }

    EXPECT_EQ(set.size(), 1000u);
}
