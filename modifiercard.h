#pragma once
#include <string>

enum class ModifierType {
    DoubleIt,
    GiveMeMore
};

struct ModifierCard {
    ModifierType type{};
    std::string name;
    int price = 0;

    ModifierCard() = default;

    ModifierCard(ModifierType t, const std::string& n, int p)
        : type(t), name(n), price(p) {
    }
};