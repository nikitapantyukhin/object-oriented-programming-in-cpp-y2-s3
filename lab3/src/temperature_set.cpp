#include "temperature_set.h"

#include <utility>

TemperatureSet::TemperatureSet()
    : data_(nullptr), size_(0), capacity_(0) {}

TemperatureSet::TemperatureSet(const TemperatureSet& other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {

    if (capacity_ > 0) {
        data_ = new Temperature[capacity_];

        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
}

TemperatureSet::TemperatureSet(TemperatureSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

TemperatureSet& TemperatureSet::operator=(const TemperatureSet& other) {
    if (this != &other) {
        TemperatureSet temp(other);
        swap(temp);
    }

    return *this;
}

TemperatureSet& TemperatureSet::operator=(TemperatureSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    return *this;
}

TemperatureSet::~TemperatureSet() {
    delete[] data_;
}

void TemperatureSet::swap(TemperatureSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

void TemperatureSet::grow() {
    std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;

    Temperature* new_data = new Temperature[new_capacity];

    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }

    delete[] data_;

    data_ = new_data;
    capacity_ = new_capacity;
}

bool TemperatureSet::contains(const Temperature& temperature) const {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == temperature) {
            return true;
        }
    }

    return false;
}

bool TemperatureSet::add(const Temperature& temperature) {
    if (contains(temperature)) {
        return false;
    }

    if (size_ == capacity_) {
        grow();
    }

    data_[size_] = temperature;
    ++size_;

    return true;
}

bool TemperatureSet::remove(const Temperature& temperature) {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == temperature) {
            for (std::size_t j = i; j + 1 < size_; ++j) {
                data_[j] = data_[j + 1];
            }

            --size_;
            return true;
        }
    }

    return false;
}

bool TemperatureSet::operator()(const Temperature& temperature) const {
    return contains(temperature);
}

std::size_t TemperatureSet::size() const {
    return size_;
}

bool TemperatureSet::empty() const {
    return size_ == 0;
}

TemperatureSet& TemperatureSet::operator+=(const Temperature& temperature) {
    add(temperature);
    return *this;
}

TemperatureSet& TemperatureSet::operator-=(const Temperature& temperature) {
    remove(temperature);
    return *this;
}

TemperatureSet TemperatureSet::operator|(const TemperatureSet& other) const {
    TemperatureSet result(*this);

    for (std::size_t i = 0; i < other.size_; ++i) {
        result.add(other.data_[i]);
    }

    return result;
}

TemperatureSet TemperatureSet::operator&(const TemperatureSet& other) const {
    TemperatureSet result;

    for (std::size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }

    return result;
}

TemperatureSet TemperatureSet::operator-(const TemperatureSet& other) const {
    TemperatureSet result;

    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }

    return result;
}

bool TemperatureSet::operator==(const TemperatureSet& other) const {
    if (size_ != other.size_) {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            return false;
        }
    }

    return true;
}

bool TemperatureSet::operator!=(const TemperatureSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const TemperatureSet& set) {
    os << "{";

    for (std::size_t i = 0; i < set.size_; ++i) {
        os << (i == 0 ? " " : ", ");
        os << set.data_[i];
    }

    os << " }";

    return os;
}
