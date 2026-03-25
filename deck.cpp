#include "Deck.h"
#include <algorithm>
#include <stdexcept>

void Deck::GenerateStandard52() {
    cards.clear();
    cards.reserve(52);

    for (int s = 0; s < 4; ++s) {
        for (int r = static_cast<int>(Rank::Two); r <= static_cast<int>(Rank::Ace); ++r) {
            cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
        }
    }
}

void Deck::Shuffle(std::mt19937& rng) {
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::Draw() {
    if (cards.empty()) throw std::runtime_error("Deck empty");
    Card c = cards.back();
    cards.pop_back();
    return c;
}

int Deck::Size() const {
    return static_cast<int>(cards.size());
}

bool Deck::Empty() const {
    return cards.empty();
}