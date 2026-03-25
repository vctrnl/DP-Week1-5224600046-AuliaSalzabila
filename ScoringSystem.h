#pragma once
#include <vector>
#include "Card.h"

class ScoringSystem {
public:
    enum class HandType {
        HighCard,
        Pair,
        TwoPair,
        ThreeKind,
        Straight,
        Flush,
        FullHouse,
        FourKind,
        StraightFlush
    };

    struct BaseScore {
        HandType type = HandType::HighCard;
        int chips = 0;
        int mult = 1;
        int finalScore = 0;
    };

    // cardsPlayed maksimal 5 kartu. Idealnya tepat 5 untuk evaluasi combo poker.
    BaseScore EvaluateBase(const std::vector<Card>& cardsPlayed) const;

private:
    HandType EvaluateHandType5(const std::vector<Card>& v) const;

    bool IsFlush5(const std::vector<Card>& v) const;
    bool IsStraight5(const std::vector<Card>& v) const;

    void BuildRankCounts5(const std::vector<Card>& v, int counts[15]) const;

    int SumRanksAsChips(const std::vector<Card>& v) const;

    int CountPairs(const int counts[15]) const;
    bool HasNOfAKind(const int counts[15], int n) const;
    bool HasFullHouse(const int counts[15]) const;

    void ApplyComboBase(HandType type, int& chips, int& mult) const;
};