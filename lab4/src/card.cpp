#include "card.h"

#include <cmath>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>

Card::Card(double spending, double rate)
    : spending_(spending), rate_(rate) {
    if (!std::isfinite(spending_) || spending_ < 0.0) {
        throw std::invalid_argument("Неверная сумма трат");
    }
    if (!std::isfinite(rate_) || rate_ < 0.0) {
        throw std::invalid_argument("Неверный процент кэшбэка");
    }
}

double Card::metric() const {
    return spending_ * rate_;
}

void Card::print(std::ostream& out) const {
    const std::streamsize oldPrecision = out.precision(15);
    out << "Карта: расходы=" << spending_ << " ставка=" << rate_ << " метрика=" << metric();
    out.precision(oldPrecision);

}

void Card::read(std::istream& in) {
    double spending, rate;

    in >> std::ws;
    if (!in) {
        return;
    }

    if (in.peek() == 'C') {
        double printedMetric;
        if (!readLiteral(in, "Карта: расходы=") || !(in >> spending) ||
            !readLiteral(in, " ставка=") || !(in >> rate) ||
            !readLiteral(in, " метрика=") || !(in >> printedMetric)) {
            return;
        }

        if (!std::isfinite(printedMetric)) {
            in.setstate(std::ios::failbit);
            return;
        }
    } else if (!(in >> spending >> rate)) {
        return;
    }

    if (!std::isfinite(spending) || spending < 0.0 || !std::isfinite(rate) || rate < 0.0) {
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
