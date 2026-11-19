#include "deposit.h"

#include <cmath>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>

Deposit::Deposit(double sum, double rate, int years, bool cumulative)
    : sum_(sum), rate_(rate), years_(years), cumulative_(cumulative) {
    if (!std::isfinite(sum_) || sum_ < 0.0) {
        throw std::invalid_argument("Недопустимая сумма вклада");
    }
    if (!std::isfinite(rate_) || rate_ < 0.0) {
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
    const std::streamsize oldPrecision = out.precision(15);
    out << "Вклад: сумма=" << sum_
        << " ставка=" << rate_
        << " лет=" << years_
        << " капитализация=" << static_cast<int>(cumulative_)
        << " метрика=" << metric();
    out.precision(oldPrecision);
}

void Deposit::read(std::istream& in) {
    double sum, rate;
    int years, cumulative;

    in >> std::ws;
    if (!in) {
        return;
    }

    if (in.peek() == 'D') {
        double printedMetric;
        if (!readLiteral(in, "Вклад: сумма=") || !(in >> sum) ||
            !readLiteral(in, " ставка=") || !(in >> rate) ||
            !readLiteral(in, " лет=") || !(in >> years) ||
            !readLiteral(in, " капитализация=") || !(in >> cumulative) ||
            !readLiteral(in, " метрика=") || !(in >> printedMetric)) {
            return;
        }
        if (!std::isfinite(printedMetric)) {
            in.setstate(std::ios::failbit);
            return;
        }
    } else if (!(in >> sum >> rate >> years >> cumulative)) {
        return;
    }

    if (!std::isfinite(sum) || sum < 0.0 || !std::isfinite(rate) || rate < 0.0 ||
        years <= 0 || (cumulative != 0 && cumulative != 1)) {
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
