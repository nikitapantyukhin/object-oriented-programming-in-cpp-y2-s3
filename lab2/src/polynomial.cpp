#include "polynomial.h"

#include <ostream>
#include <stdexcept>
#include <utility>

Polynomial::Polynomial()
    : coeffs_(nullptr), degree_(-1) {
}

Polynomial::Polynomial(const double* coeffs, int degree)
    : coeffs_(nullptr), degree_(-1) {

    if (degree < -1) {
        throw std::invalid_argument("Invalid degree");
    }

    if (degree == -1) {
        return;
    }

    if (coeffs == nullptr) {
        throw std::invalid_argument("Null coefficients");
    }

    int new_degree = degree;

    while (new_degree >= 0 && coeffs[new_degree] == 0.0) {
        --new_degree;
    }

    if (new_degree == -1) {
        return;
    }

    coeffs_ = new double[new_degree + 1];

    for (int i = 0; i <= new_degree; ++i) {
        coeffs_[i] = coeffs[i];
    }

    degree_ = new_degree;
}

Polynomial::Polynomial(const Polynomial& other)
    : coeffs_(nullptr), degree_(other.degree_) {

    if (degree_ == -1) {
        return;
    }

    coeffs_ = new double[degree_ + 1];

    for (int i = 0; i <= degree_; ++i) {
        coeffs_[i] = other.coeffs_[i];
    }
}

Polynomial::Polynomial(Polynomial&& other) noexcept
    : coeffs_(other.coeffs_), degree_(other.degree_) {

    other.coeffs_ = nullptr;
    other.degree_ = -1;
}

Polynomial& Polynomial::operator=(Polynomial other) {
    swap(other);
    return *this;
}

Polynomial::~Polynomial() {
    delete[] coeffs_;
}

void Polynomial::swap(Polynomial& other) noexcept {
    std::swap(coeffs_, other.coeffs_);
    std::swap(degree_, other.degree_);
}

void Polynomial::trim() {
    int new_degree = degree_;

    while (new_degree >= 0 && coeffs_[new_degree] == 0.0) {
        --new_degree;
    }

    if (new_degree == degree_) {
        return;
    }

    if (new_degree == -1) {
        delete[] coeffs_;
        coeffs_ = nullptr;
        degree_ = -1;
        return;
    }

    double* new_coeffs = new double[new_degree + 1];

    for (int i = 0; i <= new_degree; ++i) {
        new_coeffs[i] = coeffs_[i];
    }

    delete[] coeffs_;

    coeffs_ = new_coeffs;
    degree_ = new_degree;
}

double Polynomial::evaluate(double x) const {
    double result = 0.0;
    double power = 1.0;

    for (int i = 0; i <= degree_; ++i) {
        result += coeffs_[i] * power;
        power *= x;
    }

    return result;
}

void Polynomial::add(const Polynomial& other) {
    int max_degree = degree_;

    if (other.degree_ > max_degree) {
        max_degree = other.degree_;
    }

    if (max_degree == -1) {
        return;
    }

    Polynomial result;

    result.coeffs_ = new double[max_degree + 1]{};
    result.degree_ = max_degree;

    for (int i = 0; i <= degree_; ++i) {
        result.coeffs_[i] += coeffs_[i];
    }

    for (int i = 0; i <= other.degree_; ++i) {
        result.coeffs_[i] += other.coeffs_[i];
    }

    result.trim();
    swap(result);
}

void Polynomial::mul(const Polynomial& other) {
    if (degree_ == -1 || other.degree_ == -1) {
        Polynomial result;
        swap(result);
        return;
    }

    int new_degree = degree_ + other.degree_;

    Polynomial result;

    result.coeffs_ = new double[new_degree + 1]{};
    result.degree_ = new_degree;

    for (int i = 0; i <= degree_; ++i) {
        for (int j = 0; j <= other.degree_; ++j) {
            result.coeffs_[i + j] += coeffs_[i] * other.coeffs_[j];
        }
    }

    result.trim();
    swap(result);
}

Polynomial Polynomial::pow(std::size_t n) const {
    double one[] = {1.0};
    Polynomial result(one, 0);

    for (std::size_t i = 0; i < n; ++i) {
        result.mul(*this);
    }

    return result;
}

void Polynomial::print(std::ostream& out) const {
    if (degree_ == -1) {
        out << 0;
        return;
    }

    bool first = true;

    for (int i = 0; i <= degree_; ++i) {
        double c = coeffs_[i];

        if (c == 0.0) {
            continue;
        }

        if (!first) {
            out << (c < 0 ? " - " : " + ");
        } 
        
        else if (c < 0) {
            out << "-";
        }

        double abs_c = c < 0 ? -c : c;

        if (i == 0 || abs_c != 1.0) {
            out << abs_c;
        }

        if (i > 0) {
            out << "x";

            if (i > 1) {
                out << "^" << i;
            }
        }

        first = false;
    }
}
