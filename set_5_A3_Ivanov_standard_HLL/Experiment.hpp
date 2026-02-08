//
// Created by Иван Иванов on 08.02.2026.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

struct ExperimentConfig {
    int B;
    std::size_t streamSize;
    int runs;
    int stepPercent;

    std::uint64_t baseSeed;
    std::uint64_t hashSeedBase;
};

class ExperimentRunner {
public:
    void run(const ExperimentConfig& cfg,
             const std::string& pointsPath,
             const std::string& statsPath);
};

