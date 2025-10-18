#pragma once
#include <vector>
#include <random>

class Agent {
public:
    Agent(double initialMoney, const std::vector<double>& prodCaps);

    double produce(int itemIndex, std::mt19937_64& rng);
    double decide_demand(int itemIndex, double market_price, std::mt19937_64& rng);
    double buy(int itemIndex, double desired, double price);
    bool sell_from_inventory(int itemIndex, double units, double price);
    void receive_payment(double money);
    void consume(int itemIndex);

    double get_money() const { return money; }
    double get_inventory(int itemIndex) const { return inventory[itemIndex]; }

protected:
    double money;
    std::vector<double> production_capacity;
    std::vector<double> consumption_need;
    std::vector<double> inventory;

    std::vector<double> lastProduced;
    std::vector<double> lastDesired;
};
