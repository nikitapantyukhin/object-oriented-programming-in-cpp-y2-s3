#pragma once

#include "bank_product.h"

#include <cstddef>

class BankCollection {
private:
    BankProduct** items_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

    void reserve(std::size_t newCapacity);
    void swap(BankCollection& other) noexcept;

public:
    BankCollection() = default;
    ~BankCollection();

    BankCollection(const BankCollection& other);
    BankCollection& operator=(const BankCollection& other);

    BankCollection(BankCollection&& other) noexcept;
    BankCollection& operator=(BankCollection&& other) noexcept;

    void add(const BankProduct& product);
    void remove(std::size_t index);

    std::size_t size() const noexcept;
    const BankProduct& at(std::size_t index) const;

    double totalMetric() const;
    const BankProduct* maxMetric() const;
};
