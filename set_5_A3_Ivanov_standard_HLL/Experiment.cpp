//
// Created by Иван Иванов on 08.02.2026.
//

#include "Experiment.hpp"

#include "Checkpoints.hpp"
#include "HashFuncGen.hpp"
#include "HyperLogLog.hpp"
#include "RandomStreamGen.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

void ExperimentRunner::run(const ExperimentConfig& cfg,
                           const std::string& pointsPath,
                           const std::string& statsPath) {
    std::vector<std::size_t> checkpoints = buildCheckpoints(cfg.streamSize, cfg.stepPercent);
    int steps = static_cast<int>(checkpoints.size());

    std::ofstream points(pointsPath);
    if (!points.is_open()) {
        throw std::runtime_error("cannot open points csv");
    }

    std::ofstream stats(statsPath);
    if (!stats.is_open()) {
        throw std::runtime_error("cannot open stats csv");
    }

    points << "run,step,seen,F0,Nt\n";
    stats << "step,seen,meanNt,stdNt\n";

    std::vector<double> sumNt(static_cast<std::size_t>(steps), 0.0);
    std::vector<double> sumSqNt(static_cast<std::size_t>(steps), 0.0);

    for (int run = 0; run < cfg.runs; ++run) {
        RandomStreamGen gen(cfg.baseSeed + static_cast<std::uint64_t>(run), cfg.streamSize);

        HashFuncGen hgen(cfg.hashSeedBase + static_cast<std::uint64_t>(run));
        Hash32 hasher = hgen.make();

        HyperLogLog hll(cfg.B, hasher);

        std::unordered_set<std::string> exact;
        exact.reserve(cfg.streamSize);

        std::size_t seen = 0;
        int step = 0;

        while (gen.hasNext()) {
            std::string s = gen.next();
            seen += 1;

            exact.insert(s);
            hll.add(s);

            if (step < steps && seen == checkpoints[static_cast<std::size_t>(step)]) {
                std::size_t f0 = exact.size();
                double nt = hll.estimate();

                points << run << "," << step << "," << seen << "," << f0 << "," << nt << "\n";

                sumNt[static_cast<std::size_t>(step)] += nt;
                sumSqNt[static_cast<std::size_t>(step)] += nt * nt;

                step += 1;
            }
        }
    }

    for (int step = 0; step < steps; ++step) {
        double mean = sumNt[static_cast<std::size_t>(step)] / static_cast<double>(cfg.runs);
        double meanSq = sumSqNt[static_cast<std::size_t>(step)] / static_cast<double>(cfg.runs);
        double var = meanSq - mean * mean;
        if (var < 0.0) {
            var = 0.0;
        }
        double sd = std::sqrt(var);

        stats << step << "," << checkpoints[static_cast<std::size_t>(step)] << "," << mean << "," << sd << "\n";
    }
}

