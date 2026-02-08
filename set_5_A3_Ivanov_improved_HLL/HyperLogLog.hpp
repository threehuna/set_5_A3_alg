//
// Created by Иван Иванов on 08.02.2026.
//

#pragma once

#include "HashFuncGen.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

class HyperLogLog {
public:
    HyperLogLog(int B, Hash64 hasher);

    void add(const std::string& value);
    double estimate() const;

    int B() const;
    std::size_t m() const;

private:
    static int rho64(std::uint64_t w, int maxRho);
    double alphaM() const;

private:
    int B_;
    std::size_t m_;
    std::vector<std::uint64_t> regsPacked_;
    Hash64 hasher_;
};

