#include "ScoringSystem.h"
#include <algorithm>

int ScoringSystem::SumRanksAsChips(const std::vector<Card>& v) const {
    int chips = 0;
    for (const auto& c : v) {
        chips += static_cast<int>(c.rank);
    }
    return chips;
}

void ScoringSystem::BuildRankCounts5(const std::vector<Card>& v, int counts[15]) const {
    for (int i = 0; i < 15; ++i) {
        counts[i] = 0;
    }

    for (const auto& c : v) {
        int r = static_cast<int>(c.rank);
        if (r >= 2 && r <= 14) {
            counts[r] += 1;
        }
    }
}

int ScoringSystem::CountPairs(const int counts[15]) const {
    int pairs = 0;
    for (int r = 2; r <= 14; ++r) {
        if (counts[r] == 2) {
            pairs += 1;
        }
    }
    return pairs;
}

bool ScoringSystem::HasNOfAKind(const int counts[15], int n) const {
    for (int r = 2; r <= 14; ++r) {
        if (counts[r] == n) return true;
    }
    return false;
}

bool ScoringSystem::HasFullHouse(const int counts[15]) const {
    bool has3 = false;
    bool has2 = false;

    for (int r = 2; r <= 14; ++r) {
        if (counts[r] == 3) has3 = true;
        if (counts[r] == 2) has2 = true;
    }

    return has3 && has2;
}

bool ScoringSystem::IsFlush5(const std::vector<Card>& v) const {
    if (v.size() != 5) return false;

    Suit s = v[0].suit;
    for (int i = 1; i < 5; ++i) {
        if (v[i].suit != s) return false;
    }
    return true;
}

bool ScoringSystem::IsStraight5(const std::vector<Card>& v) const {
    if (v.size() != 5) return false;

    int ranks[5];
    for (int i = 0; i < 5; ++i) {
        ranks[i] = static_cast<int>(v[i].rank);
    }

    std::sort(ranks, ranks + 5);

    // A2345
    bool wheel = (ranks[0] == 2 && ranks[1] == 3 && ranks[2] == 4 && ranks[3] == 5 && ranks[4] == 14);
    if (wheel) return true;

    for (int i = 1; i < 5; ++i) {
        if (ranks[i] != ranks[i - 1] + 1) return false;
    }
    return true;
}

ScoringSystem::HandType ScoringSystem::EvaluateHandType5(const std::vector<Card>& v) const {
    if (v.size() != 5) return HandType::HighCard;

    bool flush = IsFlush5(v);
    bool straight = IsStraight5(v);

    int counts[15];
    BuildRankCounts5(v, counts);

    if (straight && flush) return HandType::StraightFlush;
    if (HasNOfAKind(counts, 4)) return HandType::FourKind;
    if (HasFullHouse(counts)) return HandType::FullHouse;
    if (flush) return HandType::Flush;
    if (straight) return HandType::Straight;
    if (HasNOfAKind(counts, 3)) return HandType::ThreeKind;

    int pairs = CountPairs(counts);
    if (pairs == 2) return HandType::TwoPair;
    if (pairs == 1) return HandType::Pair;

    return HandType::HighCard;
}

void ScoringSystem::ApplyComboBase(HandType type, int& chips, int& mult) const {
    // mult dasar dari combo
    switch (type) {
    case HandType::HighCard:      mult = 1; break;
    case HandType::Pair:          mult = 2; break;
    case HandType::TwoPair:       mult = 3; break;
    case HandType::ThreeKind:     mult = 4; break;
    case HandType::Straight:      mult = 5; break;
    case HandType::Flush:         mult = 6; break;
    case HandType::FullHouse:     mult = 7; break;
    case HandType::FourKind:      mult = 8; break;
    case HandType::StraightFlush: mult = 10; break;
    }

    // bonus chips dari combo
    switch (type) {
    case HandType::HighCard:      chips += 0;   break;
    case HandType::Pair:          chips += 10;  break;
    case HandType::TwoPair:       chips += 20;  break;
    case HandType::ThreeKind:     chips += 30;  break;
    case HandType::Straight:      chips += 40;  break;
    case HandType::Flush:         chips += 50;  break;
    case HandType::FullHouse:     chips += 60;  break;
    case HandType::FourKind:      chips += 80;  break;
    case HandType::StraightFlush: chips += 100; break;
    }
}

ScoringSystem::BaseScore ScoringSystem::EvaluateBase(const std::vector<Card>& cardsPlayed) const {
    BaseScore bs;

    // kalau belum 5 kartu, tetap kasih skor dasar dari rank saja
    bs.chips = SumRanksAsChips(cardsPlayed);
    bs.mult = 1;

    if (cardsPlayed.size() == 5) {
        bs.type = EvaluateHandType5(cardsPlayed);
        ApplyComboBase(bs.type, bs.chips, bs.mult);
    }
    else {
        bs.type = HandType::HighCard;
    }

    bs.finalScore = bs.chips * bs.mult;
    return bs;
}