#include "credit.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

Credit::Credit(double sum, double rate, int years, bool annuity)
    : sum_(sum), rate_(rate), years_(years), annuity_(annuity) {
    if (sum_ < 0.0) {
        throw std::invalid_argument("Недопустимая сумма кредита");
    }
    if (rate_ < 0.0) {
        throw std::invalid_argument("Недопустимая процентная ставка");
    }
    if (years_ <= 0) {
        throw std::invalid_argument("Недопустимый срок кредита");
    }
}

double Credit::metric() const {
    if (!annuity_) {
        return sum_ * rate_;
    }

    const double r = rate_ / 12.0;
    const double n = static_cast<double>(years_) * 12.0;

    if (r == 0.0) {
        return 12.0 * sum_ / n;
    }

    const double payment = sum_ * r / (1.0 - std::pow(1.0 + r, -n));
    return 12.0 * payment;
}

void Credit::print(std::ostream& out) const {
    const std::streamsize oldPrecision = out.precision(15);
    out << sum_ << ' ' << rate_ << ' ' << years_ << ' ' << annuity_;
    out.precision(oldPrecision);
}

void Credit::read(std::istream& in) {
    double sum, rate;
    int years, annuity;

    if (!(in >> sum >> rate >> years >> annuity)) {
        return;
    }

    if (sum < 0.0 || rate < 0.0 ||
        years <= 0 || (annuity != 0 && annuity != 1)) {
        in.setstate(std::ios::failbit);
        return;
    }

    sum_ = sum;
    rate_ = rate;
    years_ = years;
    annuity_ = (annuity == 1);
}

BankProduct* Credit::clone() const {
    return new Credit(*this);
}

bool Credit::isAnnuity() const {
    return annuity_;
}
