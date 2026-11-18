#pragma once

#include "temperature.h"

#include <cstddef>
#include <iostream>

class TemperatureSet {
public:
    TemperatureSet();
    TemperatureSet(const TemperatureSet& other);
    TemperatureSet(TemperatureSet&& other) noexcept;

    TemperatureSet& operator=(const TemperatureSet& other);
    TemperatureSet& operator=(TemperatureSet&& other) noexcept;

    ~TemperatureSet();

    bool add(const Temperature& temperature);
    bool remove(const Temperature& temperature);
    bool contains(const Temperature& temperature) const;

    bool operator()(const Temperature& temperature) const;

    std::size_t size() const;
    bool empty() const;

    TemperatureSet& operator+=(const Temperature& temperature);
    TemperatureSet& operator-=(const Temperature& temperature);

    TemperatureSet operator|(const TemperatureSet& other) const;
    TemperatureSet operator&(const TemperatureSet& other) const;
    TemperatureSet operator-(const TemperatureSet& other) const;

    bool operator==(const TemperatureSet& other) const;
    bool operator!=(const TemperatureSet& other) const;

    friend std::ostream& operator<<(std::ostream& os, const TemperatureSet& set);

private:
    Temperature* data_;
    std::size_t size_;
    std::size_t capacity_;

    void grow();
    void swap(TemperatureSet& other) noexcept;
};
