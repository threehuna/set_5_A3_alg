#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <string>

class RandomStreamGen {
public:
    RandomStreamGen(std::uint64_t seed, std::size_t totalSize);

    bool hasNext() const;
    std::string next();

    std::size_t totalSize() const;

private:
    static const std::string& alphabet();

private:
    std::mt19937_64 rng_;
    std::size_t totalSize_;
    std::size_t produced_;
    std::uniform_int_distribution<int> lenDist_;
    std::uniform_int_distribution<int> charDist_;
};
