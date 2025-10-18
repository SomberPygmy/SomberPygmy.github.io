#include "Market.h"
#include <algorithm>

Market::Market(const std::vector<std::string>& itemNames, const std::vector<double>& initPrices) {
    for (size_t i = 0; i < itemNames.size(); ++i) {
        items.emplace_back(itemNames[i], initPrices[i]);
    }
}

void Market::step(const std::vector<double>& total_supply,
    const std::vector<double>& total_demand) {
    for (size_t i = 0; i < items.size(); ++i) {
        double supply = total_supply[i];
        double demand = total_demand[i];
        if (supply + demand == 0) continue;

        double imbalance = (demand - supply) / (supply + demand);
        double delta = 1.0 + adjustment_speed * imbalance;
        if (delta <= 0.0) delta = 0.01;

        items[i].price *= delta;

        if (items[i].price < 0.01) items[i].price = 0.01;
        if (items[i].price > 1000.0) items[i].price = 1000.0;
    }
}
