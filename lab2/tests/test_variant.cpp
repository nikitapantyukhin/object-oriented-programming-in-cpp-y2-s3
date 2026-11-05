#include "polynomial.h"
#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>
#include <utility>

void expectPolynomial(const Polynomial& p, const char* expected) {
    std::ostringstream out;
    p.print(out);
    EXPECT_EQ(out.str(), expected);
}

// Конструкторы

TEST(PolynomialConstructorTest, DefaultCreatesZeroPolynomial) {
    Polynomial p;

    expectPolynomial(p, "0");
    EXPECT_DOUBLE_EQ(p.evaluate(5.0), 0.0);
}

TEST(PolynomialConstructorTest, CreatesPolynomialFromCoefficients) {
    double coeffs[] = {1.0, 2.0, 3.0};
    Polynomial p(coeffs, 2);

    expectPolynomial(p, "1 + 2x + 3x^2");
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 17.0);
}

TEST(PolynomialConstructorTest, RemovesTrailingZeroCoefficients) {
    double coeffs[] = {4.0, 3.0, 0.0, 0.0};
    Polynomial p(coeffs, 3);

    expectPolynomial(p, "4 + 3x");
}

TEST(PolynomialConstructorTest, AllZeroCoefficientsCreateZeroPolynomial) {
    double coeffs[] = {0.0, 0.0, 0.0};
    Polynomial p(coeffs, 2);

    expectPolynomial(p, "0");
}

TEST(PolynomialConstructorTest, InvalidNegativeDegreeThrowsException) {
    double coeffs[] = {1.0};

    EXPECT_THROW(Polynomial(coeffs, -2), std::invalid_argument);
}

TEST(PolynomialConstructorTest, NullCoefficientsThrowException) {
    EXPECT_THROW(Polynomial(nullptr, 2), std::invalid_argument);
}

// Копирование

TEST(PolynomialCopyTest, CopyConstructorCreatesIndependentCopy) {
    double coeffs[] = {1.0, 2.0};
    double extra[] = {5.0};
    Polynomial original(coeffs, 1);
    Polynomial copy(original);
    Polynomial addend(extra, 0);

    original.add(addend);

    expectPolynomial(original, "6 + 2x");
    expectPolynomial(copy, "1 + 2x");
}

TEST(PolynomialCopyTest, CopyConstructorHandlesZeroPolynomial) {
    Polynomial original;
    Polynomial copy(original);

    expectPolynomial(original, "0");
    expectPolynomial(copy, "0");
}

// Перемещение

TEST(PolynomialMoveTest, MoveConstructorTransfersCoefficients) {
    double coeffs[] = {1.0, 2.0, 3.0};
    Polynomial original(coeffs, 2);

    Polynomial moved(std::move(original));

    expectPolynomial(moved, "1 + 2x + 3x^2");
    expectPolynomial(original, "0");
}

// Присваивание

TEST(PolynomialAssignmentTest, CopyAssignmentReplacesOldCoefficients) {
    double first[] = {1.0, 2.0};
    double second[] = {5.0, 6.0, 7.0};
    Polynomial a(first, 1);
    Polynomial b(second, 2);

    b = a;

    expectPolynomial(a, "1 + 2x");
    expectPolynomial(b, "1 + 2x");
}

TEST(PolynomialAssignmentTest, SelfAssignmentKeepsPolynomialUnchanged) {
    double coeffs[] = {3.0, -2.0, 1.0};
    Polynomial p(coeffs, 2);

    p = p;

    expectPolynomial(p, "3 - 2x + x^2");
}

TEST(PolynomialAssignmentTest, MoveAssignmentTransfersCoefficients) {
    double first[] = {1.0, 2.0};
    double second[] = {5.0};
    Polynomial a(first, 1);
    Polynomial b(second, 0);

    b = std::move(a);

    expectPolynomial(b, "1 + 2x");
    expectPolynomial(a, "0");
}

// Вычисление значения

TEST(PolynomialEvaluateTest, EvaluatesPolynomialAtNegativePoint) {
    double coeffs[] = {1.0, 0.0, 2.0};
    Polynomial p(coeffs, 2);

    EXPECT_DOUBLE_EQ(p.evaluate(-2.0), 9.0);
}

// Сложение

TEST(PolynomialAddTest, AddsPolynomialsWithDifferentDegrees) {
    double first[] = {1.0, 2.0, 3.0};
    double second[] = {5.0, -2.0, 0.0, 4.0};
    Polynomial a(first, 2);
    Polynomial b(second, 3);

    a.add(b);

    expectPolynomial(a, "6 + 3x^2 + 4x^3");
    expectPolynomial(b, "5 - 2x + 4x^3");
}

TEST(PolynomialAddTest, CancelsHighestDegreeCoefficients) {
    double first[] = {2.0, 3.0, 4.0};
    double second[] = {5.0, -3.0, -4.0};
    Polynomial a(first, 2);
    Polynomial b(second, 2);

    a.add(b);

    expectPolynomial(a, "7");
}

// Умножение

TEST(PolynomialMulTest, MultipliesPolynomialsWithDifferentDegrees) {
    double first[] = {1.0, 2.0};
    double second[] = {1.0, 0.0, 3.0};
    Polynomial a(first, 1);
    Polynomial b(second, 2);

    a.mul(b);

    expectPolynomial(a, "1 + 2x + 3x^2 + 6x^3");
    expectPolynomial(b, "1 + 3x^2");
}

TEST(PolynomialMulTest, MultiplicationByZeroProducesZeroPolynomial) {
    double coeffs[] = {1.0, 2.0};
    Polynomial p(coeffs, 1);
    Polynomial zero;

    p.mul(zero);

    expectPolynomial(p, "0");
    expectPolynomial(zero, "0");
}

TEST(PolynomialMulTest, CanMultiplyPolynomialByItself) {
    double coeffs[] = {1.0, 1.0};
    Polynomial p(coeffs, 1);

    p.mul(p);

    expectPolynomial(p, "1 + 2x + x^2");
}

// Возведение в степень

TEST(PolynomialPowTest, RaisesPolynomialToThirdPower) {
    double coeffs[] = {1.0, 1.0};
    Polynomial p(coeffs, 1);

    Polynomial result = p.pow(3);

    expectPolynomial(result, "1 + 3x + 3x^2 + x^3");
    expectPolynomial(p, "1 + x");
}

TEST(PolynomialPowTest, ZeroPowerReturnsOne) {
    double coeffs[] = {2.0, 1.0};
    Polynomial p(coeffs, 1);
    Polynomial zero;

    expectPolynomial(p.pow(0), "1");
    expectPolynomial(zero.pow(0), "1");
}
