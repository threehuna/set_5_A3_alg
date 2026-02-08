//
// Created by Иван Иванов on 08.02.2026.
//

#include "HyperLogLog.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

HyperLogLog::HyperLogLog(int B, Hash32 hasher)
    : B_(B),
      m_(static_cast<std::size_t>(1ULL << B)),
      registers_(m_, 0),
      hasher_(hasher) {}

void HyperLogLog::add(const std::string& value) {
    std::uint32_t x = hasher_(value);

    std::uint32_t idx = 0;
    if (B_ == 0) {
        idx = 0;
    } else {
        idx = x >> (32 - B_);
    }

    std::uint32_t w = 0;
    if (B_ == 32) {
        w = 0;
    } else {
        w = x << B_;
    }

    int rho = rho32(w);
    std::size_t i = static_cast<std::size_t>(idx);

    if (i < registers_.size()) {
        if (registers_[i] < static_cast<std::uint8_t>(rho)) {
            registers_[i] = static_cast<std::uint8_t>(rho);
        }
    }
}

double HyperLogLog::estimate() const {
    double alpha = alphaM();
    double sum = 0.0;
    std::size_t zeros = 0;

    for (std::size_t i = 0; i < m_; ++i) {
        int r = static_cast<int>(registers_[i]);
        sum += std::ldexp(1.0, -r);
        if (r == 0) {
            zeros += 1;
        }
    }

    double m = static_cast<double>(m_);
    double E = alpha * m * m / sum;

    if (E <= 2.5 * m && zeros > 0) {
        double z = static_cast<double>(zeros);
        E = m * std::log(m / z);
    }

    const double two32 = 4294967296.0;
    if (E > (1.0 / 30.0) * two32) {
        double ratio = E / two32;
        if (ratio < 1.0) {
            E = -two32 * std::log(1.0 - ratio);
        }
    }

    return E;
}

int HyperLogLog::B() const {
    return B_;
}

std::size_t HyperLogLog::m() const {
    return m_;
}

int HyperLogLog::rho32(std::uint32_t w) {
    if (w == 0) {
        return 33;
    }
    int lz = 0;
    for (int i = 31; i >= 0; --i) {
        if ((w >> i) & 1U) {
            break;
        }
        lz += 1;
    }
    return lz + 1;
}

double HyperLogLog::alphaM() const {
    if (m_ == 16) return 0.673;
    if (m_ == 32) return 0.697;
    if (m_ == 64) return 0.709;
    double m = static_cast<double>(m_);
    return 0.7213 / (1.0 + 1.079 / m);
}
