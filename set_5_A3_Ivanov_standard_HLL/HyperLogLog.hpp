//
// Created by Иван Иванов on 08.02.2026.
//

#pragma once

#include "HashFuncGen.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class HyperLogLog {
public:
    HyperLogLog(int B, Hash32 hasher);

    void add(const std::string& value);
    double estimate() const;

    int B() const;
    std::size_t m() const;

private:
    static int rho32(std::uint32_t w);
    double alphaM() const;

private:
    int B_;
    std::size_t m_;
    std::vector<std::uint8_t> registers_;
    Hash32 hasher_;
};

