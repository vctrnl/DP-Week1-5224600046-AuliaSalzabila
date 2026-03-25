#include "Hand.h"

void Hand::Clear() {
    cards.clear();
}

void Hand::Add(const Card& c) {
    cards.push_back(c);
}

int Hand::Size() const {
    return static_cast<int>(cards.size());
}

const std::vector<Card>& Hand::Cards() const {
    return cards;
}

void Hand::ValidatePlay(const std::vector<int>& indices) const {
    if (indices.empty()) throw std::invalid_argument("Tidak ada kartu yang dipilih");
    if (static_cast<int>(indices.size()) > MAX_PLAY) throw std::invalid_argument("Maksimal 5 kartu dimainkan");
    if (indices.size() > cards.size()) throw std::invalid_argument("Pilihan melebihi jumlah kartu di tangan");

    std::vector<int> temp = indices;
    std::sort(temp.begin(), temp.end());

    for (std::size_t i = 0; i < temp.size(); ++i) {
        if (temp[i] < 0 || temp[i] >= static_cast<int>(cards.size())) {
            throw std::out_of_range("Index kartu tidak valid");
        }
        if (i > 0 && temp[i] == temp[i - 1]) {
            throw std::invalid_argument("Index kartu tidak boleh duplikat");
        }
    }
}

std::vector<Card> Hand::PlaySelected(const std::vector<int>& indices) {
    ValidatePlay(indices);

    std::vector<int> temp = indices;
    std::sort(temp.begin(), temp.end(), std::greater<int>());

    std::vector<Card> played;
    played.reserve(temp.size());

    for (int idx : temp) {
        played.push_back(cards[idx]);
        cards.erase(cards.begin() + idx);
    }

    std::reverse(played.begin(), played.end());
    return played;
}