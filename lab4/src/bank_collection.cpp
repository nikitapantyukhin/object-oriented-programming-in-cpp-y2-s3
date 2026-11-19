#include "bank_collection.h"

#include <limits>
#include <stdexcept>
#include <utility>

BankCollection::~BankCollection() {
    for (std::size_t i = 0; i < size_; ++i) {
        delete items_[i];
    }
    delete[] items_;
}

void BankCollection::reserve(std::size_t newCapacity) {
    if (newCapacity <= capacity_) {
        return;
    }

    BankProduct** newItems = new BankProduct*[newCapacity];
    for (std::size_t i = 0; i < size_; ++i) {
        newItems[i] = items_[i];
    }

    delete[] items_;
    items_ = newItems;
    capacity_ = newCapacity;
}

void BankCollection::add(const BankProduct& product) {
    if (size_ == capacity_) {
        if (capacity_ > std::numeric_limits<std::size_t>::max() / 2) {
            throw std::length_error("Коллекция банков слишком велика");
        }
        reserve(capacity_ == 0 ? 2 : capacity_ * 2);
    }

    items_[size_] = product.clone();
    ++size_;
}

void BankCollection::remove(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Неверный индек");
    }

    delete items_[index];
    for (std::size_t i = index; i + 1 < size_; ++i) {
        items_[i] = items_[i + 1];
    }
    --size_;
    items_[size_] = nullptr;
}

std::size_t BankCollection::size() const noexcept {
    return size_;
}

const BankProduct& BankCollection::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Неверный индек");
    }
    return *items_[index];
}

double BankCollection::totalMetric() const {
    double total = 0.0;
    for (std::size_t i = 0; i < size_; ++i) {
        total += items_[i]->metric();
    }
    return total;
}

const BankProduct* BankCollection::maxMetric() const {
    if (size_ == 0) {
        return nullptr;
    }

    const BankProduct* result = items_[0];
    for (std::size_t i = 1; i < size_; ++i) {
        if (items_[i]->metric() > result->metric()) {
            result = items_[i];
        }
    }
    return result;
}

void BankCollection::swap(BankCollection& other) noexcept {
    std::swap(items_, other.items_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

BankCollection::BankCollection(const BankCollection& other) {
    BankCollection temp;
    for (std::size_t i = 0; i < other.size_; ++i) {
        temp.add(*other.items_[i]);
    }
    swap(temp);
}

BankCollection& BankCollection::operator=(const BankCollection& other) {
    if (this != &other) {
        BankCollection temp(other);
        swap(temp);
    }
    return *this;
}

BankCollection::BankCollection(BankCollection&& other) noexcept {
    swap(other);
}

BankCollection& BankCollection::operator=(BankCollection&& other) noexcept {
    if (this != &other) {
        BankCollection temp(std::move(other));
        swap(temp);
    }
    return *this;
}
