#pragma once
#include <vector>
#include <string>
#include <random>

struct EconomyStats {
    std::vector<double> prices;
    std::vector<double> production;
};

class UserAgent; //Forward

class Economy {
public:
    Economy(int numItems,
        const std::vector<std::string>& names,
        const std::vector<double>& initialPrices,
        const std::vector<double>& production,
        std::mt19937_64& rngRef);

    void step(bool includePlayer = false, UserAgent* player = nullptr);

    EconomyStats collect_stats() const;

    double get_price(size_t index) const;

private:
    std::vector<std::string> itemNames;
    std::vector<double> prices;
    std::vector<double> productionCaps;
    std::mt19937_64& rng;
};
