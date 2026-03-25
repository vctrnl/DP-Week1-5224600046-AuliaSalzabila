#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Card.h"

class Hand {
public:
    static const int MAX_PLAY = 5;

    void Clear();
    void Add(const Card& c);

    int Size() const;
    const std::vector<Card>& Cards() const;

    // Ambil maksimal 5 kartu dari tangan berdasarkan index.
    // Index pakai 0..Size-1.
    // Menghapus kartu yang dimainkan dari tangan, lalu mengembalikan kartu yang dimainkan.
    std::vector<Card> PlaySelected(const std::vector<int>& indices);

private:
    std::vector<Card> cards;

    void ValidatePlay(const std::vector<int>& indices) const;
};