#pragma once

#include "bank_product.h"

class Deposit : public BankProduct {
private:
    double sum_ = 0.0;
    double rate_ = 0.0;
    int years_ = 1;
    bool cumulative_ = false;

public:
    Deposit() = default;
    Deposit(double sum, double rate, int years, bool cumulative);

    double metric() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    BankProduct* clone() const override;

    bool isCumulative() const;
};
