#include "card.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

Card::Card(double spending, double rate)
    : spending_(spending), rate_(rate) {
    if (spending_ < 0.0) {
        throw std::invalid_argument("Неверная сумма трат");
    }
    if (rate_ < 0.0) {
        throw std::invalid_argument("Неверный процент кэшбэка");
    }
}

double Card::metric() const {
    return spending_ * rate_;
}

void Card::print(std::ostream& out) const {
    out << spending_ << ' ' << rate_;
}

void Card::read(std::istream& in) {
    double spending, rate;

    if (!(in >> spending >> rate)) {
        return;
    }

    if (spending < 0.0 || rate < 0.0) {
        in.setstate(std::ios::failbit);
        return;
    }

    spending_ = spending;
    rate_ = rate;
}

BankProduct* Card::clone() const {
    return new Card(*this);
}

bool Card::hasCashback() const {
    return rate_ > 0.0;
}
