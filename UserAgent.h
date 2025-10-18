#pragma once
#include <vector>
#include <algorithm>

class UserAgent {
private:
    double money;
    std::vector<int> inventory;

public:
	//Constructor only for Declaration
    UserAgent(double startingMoney, const std::vector<double>& productionCaps);

    //Money method
    double get_money() const;
    void add_money(double amount);

    //Inventory method
    int get_inventory(size_t idx) const;
    void add_item(size_t idx, int amount = 1);
    void remove_item(size_t idx, int amount = 1);
};
