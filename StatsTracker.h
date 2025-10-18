#pragma once
#include <vector>
#include "Economy.h" //Uses the existing EconomyStats

class StatsTracker {
private:
    std::vector<std::vector<double>> priceHistory;
public:
    StatsTracker(size_t numItems);

    void record(const EconomyStats& stats);

    //For Graph
    size_t getHistorySize(size_t itemIndex) const {
        return priceHistory[itemIndex].size();
    }

    double getPrice(size_t itemIndex, size_t turn) const {
        return priceHistory[itemIndex][turn];
    }

    double getMaxPrice(size_t itemIndex) const {
        double maxVal = priceHistory[itemIndex][0];
        for (double p : priceHistory[itemIndex]) if (p > maxVal) maxVal = p;
        return maxVal;
    }

    double getMinPrice(size_t itemIndex) const {
        double minVal = priceHistory[itemIndex][0];
        for (double p : priceHistory[itemIndex]) if (p < minVal) minVal = p;
        return minVal;
    }
};
