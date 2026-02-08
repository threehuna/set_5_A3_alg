//
// Created by Иван Иванов on 08.02.2026.
//

#include "RandomStreamGen.hpp"

#include <cstddef>
#include <cstdint>
#include <random>
#include <string>

RandomStreamGen::RandomStreamGen(std::uint64_t seed, std::size_t totalSize)
    : rng_(seed),
      totalSize_(totalSize),
      produced_(0),
      lenDist_(1, 30),
      charDist_(0, static_cast<int>(alphabet().size() - 1)) {}

bool RandomStreamGen::hasNext() const {
    return produced_ < totalSize_;
}

std::string RandomStreamGen::next() {
    int len = lenDist_(rng_);
    std::string s;
    s.reserve(static_cast<std::size_t>(len));

    const std::string& alpha = alphabet();
    for (int i = 0; i < len; ++i) {
        int idx = charDist_(rng_);
        s.push_back(alpha[static_cast<std::size_t>(idx)]);
    }

    produced_ += 1;
    return s;
}

std::size_t RandomStreamGen::totalSize() const {
    return totalSize_;
}

const std::string& RandomStreamGen::alphabet() {
    static const std::string alpha =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "-";
    return alpha;
}
