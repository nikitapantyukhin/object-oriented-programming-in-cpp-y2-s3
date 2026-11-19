#pragma once

#include <iosfwd>

class BankProduct {
public:
    virtual ~BankProduct() = default;

    virtual double metric() const = 0;
    virtual void print(std::ostream& out) const = 0;
    virtual void read(std::istream& in) = 0;
    virtual BankProduct* clone() const = 0;

    explicit operator double() const;
};

std::ostream& operator<<(std::ostream& out, const BankProduct& product);
std::istream& operator>>(std::istream& in, BankProduct& product);
bool operator==(const BankProduct& lhs, const BankProduct& rhs);
