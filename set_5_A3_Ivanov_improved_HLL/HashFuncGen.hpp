//
// Created by Иван Иванов on 08.02.2026.
//
#pragma once

#include <cstdint>
#include <random>
#include <string>

struct Hash32 {
    std::uint32_t seed;
    std::uint32_t operator()(const std::string& s);
};

struct Hash64 {
    std::uint64_t seed;
    std::uint64_t operator()(const std::string& s);
};

class HashFuncGen {
public:
    explicit HashFuncGen(std::uint64_t seed);

    Hash32 make();
    Hash64 make64();

private:
    std::mt19937_64 rng_;
};

