#pragma once
#include <string>

struct Item {
    std::string name;
    double price;

    Item(const std::string& n, double p) : name(n), price(p) {}
};
