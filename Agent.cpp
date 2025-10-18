#include "Agent.h"
#include <algorithm>
#include <random>

Agent::Agent(double initialMoney, const std::vector<double>& prodCaps)
    : money(initialMoney),
    production_capacity(prodCaps),
    consumption_need(prodCaps.size(), 1.0),
    inventory(prodCaps.size(), 0.0),
    lastProduced(prodCaps.size(), 0.0),
    lastDesired(prodCaps.size(), 0.0) {
}

double Agent::produce(int itemIndex, std::mt19937_64& rng) {
    std::uniform_real_distribution<double> var(0.85, 1.15);
    lastProduced[itemIndex] = production_capacity[itemIndex] * var(rng);
    inventory[itemIndex] += lastProduced[itemIndex];
    return lastProduced[itemIndex];
}

double Agent::decide_demand(int itemIndex, double market_price, std::mt19937_64& rng) {
    std::uniform_real_distribution<double> var(0.9, 1.1);
    double buffer_target = consumption_need[itemIndex] * 1.5 * var(rng);
    double desire = std::max(0.0, buffer_target - inventory[itemIndex]);
    double price_factor = 1.0 / (1.0 + market_price);
    lastDesired[itemIndex] = desire * price_factor;
    return lastDesired[itemIndex];
}

double Agent::buy(int itemIndex, double desired, double price) {
    double affordable = money / price;
    double actually_buy = std::min(desired, affordable);
    money -= actually_buy * price;
    inventory[itemIndex] += actually_buy;
    return actually_buy;
}

bool Agent::sell_from_inventory(int itemIndex, double units, double price) {
    double toSell = std::min(units, inventory[itemIndex]);
    if (toSell > 0.0) {
        inventory[itemIndex] -= toSell;
        money += toSell * price;
        return true;
    }
    return false;
}

void Agent::receive_payment(double m) {
    money += m;
}

void Agent::consume(int itemIndex) {
    double consumed = std::min(inventory[itemIndex], consumption_need[itemIndex]);
    inventory[itemIndex] -= consumed;
}
