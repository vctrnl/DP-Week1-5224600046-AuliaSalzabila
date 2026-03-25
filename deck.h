#pragma once
#include <vector>
#include <random>
#include "Card.h"

class Deck {
public:
    void GenerateStandard52();
    void Shuffle(std::mt19937& rng);

    Card Draw();
    int Size() const;
    bool Empty() const;

private:
    std::vector<Card> cards;
};