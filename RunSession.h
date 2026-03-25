#pragma once
#include <random>
#include <vector>
#include <string>
#include "Deck.h"
#include "Hand.h"
#include "ScoringSystem.h"
#include "ModifierCard.h"

class RunSession {
public:
    RunSession();
    explicit RunSession(unsigned int seed);

    void Start();

private:
    static const int HAND_SIZE = 8;
    static const int ROUNDS_PER_LEVEL = 4;

    void PrepareDeckIfNeeded(int needCards);
    void DealNewHand();
    std::vector<int> ReadPlayIndices() const;
    void PrintCurrentHand() const;
    std::string HandTypeToString(ScoringSystem::HandType t) const;

    int GetGoldPerRemainingCard() const;
    int CalculateGoldFromRemainingCards() const;

    int GetModifierPriceForCurrentLevel(const ModifierCard& mod) const;

    void PrintOwnedModifiers() const;
    int ReadModifierChoiceToUse() const;

    void OpenShop();
    int ReadShopChoice() const;

private:
    std::mt19937 rng;
    Deck deck;
    Hand hand;
    ScoringSystem scoring;

    int targetScore = 120;
    int levelIndex = 1;
    int playerGold = 0;

    std::vector<ModifierCard> ownedModifiers;
};