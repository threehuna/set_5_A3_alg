//
// Created by Иван Иванов on 08.02.2026.
//
#include "HashFuncGen.hpp"

#include <cstdint>
#include <random>
#include <string>

static std::uint32_t fmix32(std::uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

std::uint32_t Hash32::operator()(const std::string& s) {
    std::uint32_t h = 2166136261U ^ seed;
    for (unsigned char c : s) {
        h ^= static_cast<std::uint32_t>(c);
        h *= 16777619U;
    }
    return fmix32(h);
}

HashFuncGen::HashFuncGen(std::uint64_t seed) : rng_(seed) {}

Hash32 HashFuncGen::make() {
    std::uint32_t s = static_cast<std::uint32_t>(rng_());
    if (s == 0) {
        s = 1;
    }
    Hash32 h;
    h.seed = s;
    return h;
}

