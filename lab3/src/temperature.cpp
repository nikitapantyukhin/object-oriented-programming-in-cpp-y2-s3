#include "temperature.h"

#include <cmath>
#include <stdexcept>

Temperature::Temperature()
    : value_(0.0), unit_(Unit::Celsius) {}

Temperature::Temperature(double value, Unit unit)
    : value_(value), unit_(unit) {

    if (!std::isfinite(value)) {
        throw std::invalid_argument("Invalid temperature value");
    }

    if (unit != Unit::Celsius && unit != Unit::Fahrenheit && unit != Unit::Kelvin) {
        throw std::invalid_argument("Invalid temperature unit");
    }
}

long double Temperature::canonicalKey() const {
    long double celsius = static_cast<long double>(value_);

    switch (unit_) {
        case Unit::Celsius:
            break;

        case Unit::Fahrenheit:
            celsius = (celsius - 32.0L) * 5.0L / 9.0L;
            break;

        case Unit::Kelvin:
            celsius -= 273.15L;
            break;
    }

    return std::round(celsius * 1000000.0L);
}

bool Temperature::operator==(const Temperature& other) const {
    return canonicalKey() == other.canonicalKey();
}

std::ostream& operator<<(std::ostream& os, const Temperature& temperature) {
    os << temperature.value_;

    switch (temperature.unit_) {
        case Temperature::Unit::Celsius:
            os << "°C";
            break;

        case Temperature::Unit::Fahrenheit:
            os << "°F";
            break;

        case Temperature::Unit::Kelvin:
            os << " K";
            break;
    }

    return os;
}
