//
// Created by Иван Иванов on 08.02.2026.
//

#include "Experiment.hpp"
#include <iostream>

int main() {
    ExperimentConfig cfg;
    cfg.B = 12;
    cfg.streamSize = 100000;
    cfg.runs = 20;
    cfg.stepPercent = 5;
    cfg.baseSeed = 12345ULL;
    cfg.hashSeedBase = 999ULL;

    ExperimentRunner runner;
    runner.run(cfg, "points.csv", "stats.csv");

    std::cout << "Done. Files: points.csv, stats.csv\n";
    return 0;
}
