#pragma once
#include <string>

enum class Suit { Hearts, Diamonds, Clubs, Spades };
enum class Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

struct Card {
    Suit suit{};
    Rank rank{};

    Card() = default;
    Card(Suit s, Rank r) : suit(s), rank(r) {}

    std::string ToString() const;
};