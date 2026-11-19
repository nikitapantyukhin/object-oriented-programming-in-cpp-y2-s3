#pragma once

#include "bank_product.h"

class Card : public BankProduct {
private:
    double spending_ = 0.0;
    double rate_ = 0.0;

public:
    Card() = default;
    Card(double spending, double rate);

    double metric() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    BankProduct* clone() const override;

    bool hasCashback() const;
};
