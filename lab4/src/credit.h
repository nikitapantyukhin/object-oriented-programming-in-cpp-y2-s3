#pragma once

#include "bank_product.h"

class Credit : public BankProduct {
private:
    double sum_ = 0.0;
    double rate_ = 0.0;
    int years_ = 1;
    bool annuity_ = false;

public:
    Credit() = default;
    Credit(double sum, double rate, int years, bool annuity);

    double metric() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    BankProduct* clone() const override;

    bool isAnnuity() const;
};
