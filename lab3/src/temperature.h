#pragma once

#include <iostream>

class Temperature {
public:
    enum class Unit {
        Celsius,
        Fahrenheit,
        Kelvin
    };

    Temperature();
    Temperature(double value, Unit unit);

    bool operator==(const Temperature& other) const;

    friend std::ostream& operator<<(
        std::ostream& os,
        const Temperature& temperature
    );

private:
    double value_;
    Unit unit_;

    long double canonicalKey() const;
};
