#pragma once

#include <cstddef>
#include <iosfwd>

class Polynomial {
private:
    double* coeffs_;
    int degree_;

    void trim();

public:
    Polynomial();
    Polynomial(const double* coeffs, int degree);
    Polynomial(const Polynomial& other);
    Polynomial(Polynomial&& other) noexcept;

    Polynomial& operator=(Polynomial other);

    ~Polynomial();

    void swap(Polynomial& other) noexcept;

    double evaluate(double x) const;
    void add(const Polynomial& other);
    void mul(const Polynomial& other);

    Polynomial pow(std::size_t n) const;

    void print(std::ostream& out) const;
};
