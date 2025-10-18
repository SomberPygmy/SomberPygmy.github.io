#include "Economy.h"
#include "UserAgent.h"
#include <algorithm>

Economy::Economy(int numItems,
    const std::vector<std::string>& names,
    const std::vector<double>& initialPrices,
    const std::vector<double>& production,
    std::mt19937_64& rngRef)
    : itemNames(names), prices(initialPrices), productionCaps(production), rng(rngRef) {
}

//Simulate one market turn
void Economy::step(bool includePlayer, UserAgent* player) {
    for (size_t i = 0; i < prices.size(); i++) {
        //Simple random price adjustment
        double change = ((double)rng() / rng.max() - 0.5) * 0.2; // -0.1 to +0.1
        prices[i] += change;
        if (prices[i] < 0.1) prices[i] = 0.1;
    }

    
    if (includePlayer && player) {
        
    }
}

//Return current market stats
EconomyStats Economy::collect_stats() const {
    return EconomyStats{ prices, productionCaps };
}

//For individual item price
double Economy::get_price(size_t index) const {
    if (index < prices.size()) return prices[index];
    return 0.0;
}
