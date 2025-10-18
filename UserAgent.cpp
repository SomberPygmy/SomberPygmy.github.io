#include "UserAgent.h"

//Constructor
UserAgent::UserAgent(double startingMoney, const std::vector<double>& productionCaps)
    : money(startingMoney), inventory(productionCaps.size(), 0) {
}

//Money method
double UserAgent::get_money() const { return money; }
void UserAgent::add_money(double amount) { money += amount; }

//Inventory method
int UserAgent::get_inventory(size_t idx) const { return inventory[idx]; }

void UserAgent::add_item(size_t idx, int amount) {
    if (idx < inventory.size())
        inventory[idx] += amount;
}

void UserAgent::remove_item(size_t idx, int amount) {
    if (idx < inventory.size())
        inventory[idx] = std::max(0, inventory[idx] - amount);
}
