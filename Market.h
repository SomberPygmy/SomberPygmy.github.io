#pragma once
#include <vector>
#include "Item.h"

class Market {
public:
    Market(const std::vector<std::string>& itemNames, const std::vector<double>& initPrices);

    void step(const std::vector<double>& total_supply,
        const std::vector<double>& total_demand);
    double get_price(int itemIndex) const { return items[itemIndex].price; }
    const std::vector<Item>& get_items() const { return items; }

private:
    std::vector<Item> items;
    double adjustment_speed = 0.05;
};
