#include "deposit.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

Deposit::Deposit(double sum, double rate, int years, bool cumulative)
    : sum_(sum), rate_(rate), years_(years), cumulative_(cumulative) {
    if (sum_ < 0.0) {
        throw std::invalid_argument("Недопустимая сумма вклада");
    }
    if (rate_ < 0.0) {
        throw std::invalid_argument("Недопустимая процентная ставка");
    }
    if (years_ <= 0) {
        throw std::invalid_argument("Недопустимый срок вклада");
    }
}

double Deposit::metric() const {
    if (!cumulative_) {
        return sum_ * rate_;
    }
    return sum_ * (std::pow(1.0 + rate_, years_) - 1.0) / years_;
}

void Deposit::print(std::ostream& out) const {
    out << sum_ << ' ' << rate_ << ' ' << years_ << ' ' << cumulative_;
}

void Deposit::read(std::istream& in) {
    double sum, rate;
    int years, cumulative;

    if (!(in >> sum >> rate >> years >> cumulative)) {
        return;
    }

    if (sum < 0.0 || rate < 0.0 || years <= 0 || (cumulative != 0 && cumulative != 1)) {
        in.setstate(std::ios::failbit);
        return;
    }

    sum_ = sum;
    rate_ = rate;
    years_ = years;
    cumulative_ = (cumulative == 1);
}

BankProduct* Deposit::clone() const {
    return new Deposit(*this);
}

bool Deposit::isCumulative() const {
    return cumulative_;
}
