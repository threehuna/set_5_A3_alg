//
// Created by Иван Иванов on 08.02.2026.
//

#include "Checkpoints.hpp"
#include <cstddef>
#include <vector>

std::vector<std::size_t> buildCheckpoints(std::size_t total, int stepPercent) {
    std::vector<std::size_t> pts;
    if (total == 0) {
        return pts;
    }
    if (stepPercent <= 0) {
        pts.push_back(total);
        return pts;
    }

    std::size_t last = 0;
    for (int p = stepPercent; p <= 100; p += stepPercent) {
        std::size_t t = (static_cast<std::size_t>(p) * total) / 100;
        if (t < 1) t = 1;
        if (t > total) t = total;
        if (t != last) {
            pts.push_back(t);
            last = t;
        }
    }

    if (pts.empty() || pts.back() != total) {
        pts.push_back(total);
    }

    return pts;
}
