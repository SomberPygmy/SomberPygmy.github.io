#include "StatsTracker.h"

StatsTracker::StatsTracker(size_t numItems) {
    priceHistory.resize(numItems);
}

void StatsTracker::record(const EconomyStats& stats) {
    for (size_t i = 0; i < stats.prices.size(); ++i) {
        priceHistory[i].push_back(stats.prices[i]);
    }
}
