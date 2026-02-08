//
// Created by Иван Иванов on 08.02.2026.
//

#include "HyperLogLog.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

static std::uint8_t get6(const std::vector<std::uint64_t>& a, std::size_t i) {
    std::size_t bit = i * 6;
    std::size_t w = bit / 64;
    std::size_t s = bit % 64;

    std::uint64_t v = a[w] >> s;
    if (s > 58) {
        if (w + 1 < a.size()) {
            v |= (a[w + 1] << (64 - s));
        }
    }
    return static_cast<std::uint8_t>(v & 63ULL);
}

static void set6(std::vector<std::uint64_t>& a, std::size_t i, std::uint8_t val) {
    std::size_t bit = i * 6;
    std::size_t w = bit / 64;
    std::size_t s = bit % 64;

    std::uint64_t v = static_cast<std::uint64_t>(val & 63U);

    std::uint64_t mask = 63ULL << s;
    a[w] = (a[w] & ~mask) | (v << s);

    if (s > 58) {
        if (w + 1 < a.size()) {
            int spill = static_cast<int>(s) - 58;
            std::uint64_t mask2 = (spill >= 64) ? ~0ULL : ((1ULL << spill) - 1ULL);
            std::uint64_t part = (v >> (64 - s)) & mask2;
            a[w + 1] = (a[w + 1] & ~mask2) | part;
        }
    }
}

HyperLogLog::HyperLogLog(int B, Hash64 hasher)
    : B_(B),
      m_(static_cast<std::size_t>(1ULL << B)),
      regsPacked_(),
      hasher_(hasher) {
    std::size_t bits = m_ * 6;
    std::size_t words = (bits + 63) / 64;
    regsPacked_.assign(words, 0ULL);
}

void HyperLogLog::add(const std::string& value) {
    std::uint64_t x = hasher_(value);

    std::uint64_t idx = 0;
    if (B_ == 0) {
        idx = 0;
    } else {
        idx = x >> (64 - B_);
    }

    std::uint64_t w = 0;
    if (B_ == 64) {
        w = 0;
    } else {
        w = x << B_;
    }

    int maxRho = (64 - B_) + 1;
    int rho = rho64(w, maxRho);

    std::size_t i = static_cast<std::size_t>(idx);
    if (i < m_) {
        std::uint8_t cur = get6(regsPacked_, i);
        std::uint8_t r = static_cast<std::uint8_t>(rho);
        if (cur < r) {
            set6(regsPacked_, i, r);
        }
    }
}

double HyperLogLog::estimate() const {
    double alpha = alphaM();
    double sum = 0.0;
    std::size_t zeros = 0;

    for (std::size_t i = 0; i < m_; ++i) {
        int r = static_cast<int>(get6(regsPacked_, i));
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

int HyperLogLog::rho64(std::uint64_t w, int maxRho) {
    if (w == 0) {
        return maxRho;
    }


    int lz = 0;
    for (int i = 63; i >= 0; --i) {
        if ((w >> i) & 1ULL) {
            break;
        }
        lz += 1;
    }
    int r = lz + 1;
    if (r > maxRho) {
        r = maxRho;
    }
    return r;
}

double HyperLogLog::alphaM() const {
    if (m_ == 16) return 0.673;
    if (m_ == 32) return 0.697;
    if (m_ == 64) return 0.709;
    double m = static_cast<double>(m_);
    return 0.7213 / (1.0 + 1.079 / m);
}
