#include "bank_product.h"

#include <cmath>
#include <istream>
#include <ostream>

bool BankProduct::readLiteral(std::istream& in, const char* literal) {
    for (const char* p = literal; *p != '\0'; ++p) {
        char actual;
        if (!in.get(actual) || actual != *p) {
            in.setstate(std::ios::failbit);
            return false;
        }
    }
    return true;
}

BankProduct::operator double() const {
    return metric();
}

std::ostream& operator<<(std::ostream& out, const BankProduct& product) {
    product.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, BankProduct& product) {
    product.read(in);
    return in;
}

bool operator==(const BankProduct& lhs, const BankProduct& rhs) {
    constexpr double EPS = 1e-6;
    return std::fabs(lhs.metric() - rhs.metric()) < EPS;
}
