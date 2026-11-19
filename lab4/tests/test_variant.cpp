#include "bank_collection.h"
#include "card.h"
#include "credit.h"
#include "deposit.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>

// Метрики банковских продуктов

TEST(DepositTest, WithoutCapitalization) {
    Deposit deposit(100000, 0.05, 2, false);
    EXPECT_DOUBLE_EQ(deposit.metric(), 5000);
    EXPECT_FALSE(deposit.isCumulative());
}

TEST(DepositTest, WithCapitalization) {
    Deposit deposit(100000, 0.05, 2, true);
    EXPECT_NEAR(deposit.metric(), 5125, 1e-6);
    EXPECT_TRUE(deposit.isCumulative());
}

TEST(CreditTest, WithoutAnnuity) {
    Credit credit(200000, 0.10, 3, false);
    EXPECT_DOUBLE_EQ(credit.metric(), 20000);
    EXPECT_FALSE(credit.isAnnuity());
}

TEST(CreditTest, WithAnnuity) {
    Credit credit(120000, 0.12, 1, true);
    EXPECT_NEAR(credit.metric(), 127942.25569681192, 1e-5);
    EXPECT_TRUE(credit.isAnnuity());
}

TEST(CardTest, Cashback) {
    Card card(100000, 0.03);
    EXPECT_DOUBLE_EQ(card.metric(), 3000);
    EXPECT_TRUE(card.hasCashback());
}

TEST(CardTest, NoCashback) {
    Card card(1000, 0);
    EXPECT_DOUBLE_EQ(card.metric(), 0);
    EXPECT_FALSE(card.hasCashback());
}

// Полиморфизм

TEST(PolymorphismTest, VirtualMetric) {
    Deposit deposit(100000, 0.05, 2, false);
    const BankProduct& product = deposit;
    EXPECT_DOUBLE_EQ(product.metric(), 5000);
}

TEST(PolymorphismTest, DynamicCast) {
    Deposit deposit(100000, 0.05, 2, true);
    BankProduct* product = &deposit;

    EXPECT_NE(dynamic_cast<Deposit*>(product), nullptr);
    EXPECT_EQ(dynamic_cast<Credit*>(product), nullptr);
}

TEST(PolymorphismTest, Clone) {
    Deposit deposit(100000, 0.05, 2, true);
    BankProduct* copy = deposit.clone();

    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy, &deposit);
    EXPECT_NE(dynamic_cast<Deposit*>(copy), nullptr);
    EXPECT_NEAR(copy->metric(), deposit.metric(), 1e-6);

    delete copy;
}

// Операторы ввода, вывода и сравнения

TEST(OperatorTest, Output) {
    Deposit deposit(100000, 0.05, 2, true);
    std::ostringstream out;
    out << deposit;

    EXPECT_NE(out.str().find("Вклад"), std::string::npos);
    EXPECT_NE(out.str().find("сумма=100000"), std::string::npos);
}

TEST(OperatorTest, Input) {
    Deposit deposit;
    std::istringstream in("100000 0.05 2 1");
    in >> deposit;

    EXPECT_TRUE(static_cast<bool>(in));
    EXPECT_NEAR(deposit.metric(), 5125, 1e-6);
}

TEST(OperatorTest, OutputAndNumericInput) {
    Card card(100000, 0.03);
    std::ostringstream out;
    out << card;

    EXPECT_NE(out.str().find("Карта"), std::string::npos);
    EXPECT_NE(out.str().find("расходы=100000"), std::string::npos);

    Card copy;
    std::istringstream in("100000 0.03");
    in >> copy;

    EXPECT_TRUE(static_cast<bool>(in));
    EXPECT_DOUBLE_EQ(copy.metric(), 3000);
}

TEST(OperatorTest, EqualByMetric) {
    Deposit deposit(100000, 0.05, 2, false);
    Card same(100000, 0.05);
    Card different(100000, 0.03);

    EXPECT_TRUE(deposit == same);
    EXPECT_FALSE(deposit == different);
}

TEST(OperatorTest, ConvertToDouble) {
    Deposit deposit(100000, 0.05, 2, false);
    EXPECT_DOUBLE_EQ(static_cast<double>(deposit), 5000);
}

// Коллекция и вариант 19

TEST(CollectionTest, Empty) {
    BankCollection bank;
    EXPECT_EQ(bank.size(), 0U);
    EXPECT_DOUBLE_EQ(bank.totalMetric(), 0);
    EXPECT_EQ(bank.maxMetric(), nullptr);
}

TEST(CollectionTest, SumAndMaximum) {
    BankCollection bank;
    bank.add(Deposit(100000, 0.05, 2, false));
    bank.add(Credit(200000, 0.10, 3, false));
    bank.add(Card(100000, 0.03));

    EXPECT_EQ(bank.size(), 3U);
    EXPECT_DOUBLE_EQ(bank.totalMetric(), 28000);
    ASSERT_NE(bank.maxMetric(), nullptr);
    EXPECT_DOUBLE_EQ(bank.maxMetric()->metric(), 20000);
    EXPECT_NE(dynamic_cast<const Credit*>(bank.maxMetric()), nullptr);
}

TEST(CollectionTest, Remove) {
    BankCollection bank;
    bank.add(Card(100000, 0.03));
    bank.add(Deposit(100000, 0.05, 2, false));
    bank.remove(0);

    EXPECT_EQ(bank.size(), 1U);
    EXPECT_DOUBLE_EQ(bank.totalMetric(), 5000);
}

TEST(CollectionTest, Copy) {
    BankCollection first;
    first.add(Card(100000, 0.03));
    BankCollection second(first);
    first.remove(0);

    EXPECT_EQ(first.size(), 0U);
    EXPECT_EQ(second.size(), 1U);
    EXPECT_DOUBLE_EQ(second.totalMetric(), 3000);
}

TEST(CollectionTest, Move) {
    BankCollection first;
    first.add(Card(100000, 0.03));
    BankCollection second(std::move(first));

    EXPECT_EQ(first.size(), 0U);
    EXPECT_DOUBLE_EQ(second.totalMetric(), 3000);
}

// Проверка ошибок

TEST(ValidationTest, InvalidParameters) {
    EXPECT_THROW(Deposit(-100, 0.05, 2, false), std::invalid_argument);
    EXPECT_THROW(Credit(100, 0.1, 0, true), std::invalid_argument);
    EXPECT_THROW(Card(100, -0.03), std::invalid_argument);
}

TEST(ValidationTest, InvalidInput) {
    Deposit deposit(100000, 0.05, 2, false);
    std::istringstream in("-50000 0.05 2 1");
    in >> deposit;

    EXPECT_FALSE(static_cast<bool>(in));
    EXPECT_DOUBLE_EQ(deposit.metric(), 5000);
}
