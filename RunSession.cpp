#include "RunSession.h"
#include "ModifierFactory.h"
#include <iostream>
#include <sstream>
#include <limits>

RunSession::RunSession()
    : RunSession(std::random_device{}()) {
}

RunSession::RunSession(unsigned int seed)
    : rng(seed) {
    deck.GenerateStandard52();
    deck.Shuffle(rng);
}

void RunSession::PrepareDeckIfNeeded(int needCards) {
    if (deck.Size() >= needCards) {
        return;
    }

    deck.GenerateStandard52();
    deck.Shuffle(rng);
}

void RunSession::DealNewHand() {
    PrepareDeckIfNeeded(HAND_SIZE);

    hand.Clear();
    for (int i = 0; i < HAND_SIZE; ++i) {
        hand.Add(deck.Draw());
    }
}

void RunSession::PrintCurrentHand() const {
    std::cout << "Kartu di tangan:\n";
    const std::vector<Card>& v = hand.Cards();

    for (int i = 0; i < static_cast<int>(v.size()); ++i) {
        std::cout << i << ": " << v[i].ToString() << "\n";
    }
}

std::vector<int> RunSession::ReadPlayIndices() const {
    while (true) {
        std::cout << "Pilih index kartu (maks 5), pisah spasi. Contoh: 0 2 5 7\n";
        std::cout << "Input: ";

        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            std::cout << "Input kosong.\n";
            continue;
        }

        std::istringstream iss(line);
        std::vector<int> idx;
        int x;

        while (iss >> x) {
            idx.push_back(x);
        }

        if (idx.empty()) {
            std::cout << "Tidak ada angka.\n";
            continue;
        }

        if (static_cast<int>(idx.size()) > Hand::MAX_PLAY) {
            std::cout << "Maksimal 5 kartu.\n";
            continue;
        }

        return idx;
    }
}

std::string RunSession::HandTypeToString(ScoringSystem::HandType t) const {
    switch (t) {
    case ScoringSystem::HandType::HighCard: return "HighCard";
    case ScoringSystem::HandType::Pair: return "Pair";
    case ScoringSystem::HandType::TwoPair: return "TwoPair";
    case ScoringSystem::HandType::ThreeKind: return "ThreeKind";
    case ScoringSystem::HandType::Straight: return "Straight";
    case ScoringSystem::HandType::Flush: return "Flush";
    case ScoringSystem::HandType::FullHouse: return "FullHouse";
    case ScoringSystem::HandType::FourKind: return "FourKind";
    case ScoringSystem::HandType::StraightFlush: return "StraightFlush";
    }
    return "Unknown";
}

int RunSession::GetGoldPerRemainingCard() const {
    return levelIndex;
}

int RunSession::CalculateGoldFromRemainingCards() const {
    const std::vector<Card>& remaining = hand.Cards();
    int goldPerCard = GetGoldPerRemainingCard();
    int totalGold = 0;

    for (const Card& c : remaining) {
        (void)c;
        totalGold += goldPerCard;
    }

    return totalGold;
}

int RunSession::GetModifierPriceForCurrentLevel(const ModifierCard& mod) const {
    return mod.price + (levelIndex - 1) * 2;
}

void RunSession::PrintOwnedModifiers() const {
    if (ownedModifiers.empty()) {
        std::cout << "Kamu tidak punya modifier.\n";
        return;
    }

    std::cout << "Modifier yang dimiliki:\n";
    for (int i = 0; i < static_cast<int>(ownedModifiers.size()); ++i) {
        std::cout << i + 1 << ". " << ownedModifiers[i].name << "\n";
    }
}

int RunSession::ReadModifierChoiceToUse() const {
    if (ownedModifiers.empty()) {
        return 0;
    }

    while (true) {
        std::cout << "Pilih modifier yang ingin dipakai (0 = tidak pakai): ";
        std::string line;
        std::getline(std::cin, line);

        std::istringstream iss(line);
        int choice = -1;
        iss >> choice;

        if (!iss.fail() && choice >= 0 && choice <= static_cast<int>(ownedModifiers.size())) {
            return choice;
        }

        std::cout << "Pilihan tidak valid.\n";
    }
}

int RunSession::ReadShopChoice() const {
    ModifierCard doubleIt = ModifierFactory::CreateDoubleIt();
    ModifierCard giveMeMore = ModifierFactory::CreateGiveMeMore();

    int doubleItPrice = GetModifierPriceForCurrentLevel(doubleIt);
    int giveMeMorePrice = GetModifierPriceForCurrentLevel(giveMeMore);

    while (true) {
        std::cout << "Pilih yang ingin dibeli:\n";
        std::cout << "1. Double It (" << doubleItPrice << " gold)\n";
        std::cout << "2. Give Me More (" << giveMeMorePrice << " gold)\n";
        std::cout << "0. Skip shop\n";
        std::cout << "Input: ";

        std::string line;
        std::getline(std::cin, line);

        std::istringstream iss(line);
        int choice = -1;
        iss >> choice;

        if (!iss.fail() && (choice == 0 || choice == 1 || choice == 2)) {
            return choice;
        }

        std::cout << "Pilihan tidak valid.\n";
    }
}

void RunSession::OpenShop() {
    std::cout << "\n=== SHOP ===\n";
    std::cout << "Level saat ini: " << levelIndex << "\n";
    std::cout << "Gold kamu: " << playerGold << "\n";

    int choice = ReadShopChoice();
    if (choice == 0) {
        std::cout << "Kamu melewati shop.\n";
        return;
    }

    ModifierCard mod;
    if (choice == 1) {
        mod = ModifierFactory::CreateDoubleIt();
    }
    else {
        mod = ModifierFactory::CreateGiveMeMore();
    }

    int finalPrice = GetModifierPriceForCurrentLevel(mod);

    if (playerGold < finalPrice) {
        std::cout << "Gold tidak cukup untuk membeli " << mod.name
            << ". Harga saat ini: " << finalPrice << " gold.\n";
        return;
    }

    playerGold -= finalPrice;
    ownedModifiers.push_back(mod);

    std::cout << "Berhasil membeli " << mod.name
        << " seharga " << finalPrice << " gold.\n";
    std::cout << "Sisa gold: " << playerGold << "\n";
}

void RunSession::Start() {
    while (true) {
        std::cout << "\n=== LEVEL " << levelIndex << " ===\n";
        std::cout << "Target score level: " << targetScore << "\n";
        std::cout << "Total gold saat ini: " << playerGold << "\n";

        int levelScore = 0;

        for (int round = 1; round <= ROUNDS_PER_LEVEL; ++round) {
            std::cout << "\n--- ROUND " << round << "/" << ROUNDS_PER_LEVEL << " ---\n";
            std::cout << "Score level saat ini: " << levelScore << "\n";
            std::cout << "Gold per kartu sisa: " << GetGoldPerRemainingCard() << "\n";

            DealNewHand();
            PrintCurrentHand();

            std::vector<int> pick = ReadPlayIndices();

            std::vector<Card> played;
            try {
                played = hand.PlaySelected(pick);
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
                --round;
                continue;
            }

            PrintOwnedModifiers();
            int modifierChoice = ReadModifierChoiceToUse();

            bool useDoubleIt = false;
            bool useGiveMeMore = false;

            if (modifierChoice > 0) {
                ModifierCard selectedMod = ownedModifiers[modifierChoice - 1];

                if (selectedMod.type == ModifierType::DoubleIt) {
                    useDoubleIt = true;
                    std::cout << "Kamu memakai modifier: Double It\n";
                }
                else if (selectedMod.type == ModifierType::GiveMeMore) {
                    useGiveMeMore = true;
                    std::cout << "Kamu memakai modifier: Give Me More\n";
                }

                ownedModifiers.erase(ownedModifiers.begin() + (modifierChoice - 1));
            }

            ScoringSystem::BaseScore bs = scoring.EvaluateBase(played);

            int finalRoundScore = bs.finalScore;
            if (useDoubleIt) {
                finalRoundScore *= 2;
            }

            int goldEarned = CalculateGoldFromRemainingCards();
            if (useGiveMeMore) {
                goldEarned += 4;
            }

            levelScore += finalRoundScore;
            playerGold += goldEarned;

            std::cout << "Combo: " << HandTypeToString(bs.type) << "\n";
            std::cout << "Chips: " << bs.chips << " Mult: " << bs.mult << "\n";
            std::cout << "Score round ini: " << finalRoundScore << "\n";
            std::cout << "Kartu sisa: " << hand.Size() << "\n";
            std::cout << "Gold didapat round ini: " << goldEarned << "\n";
            std::cout << "Total gold sekarang: " << playerGold << "\n";
            std::cout << "Total score level sekarang: " << levelScore << "\n";

            OpenShop();
        }

        std::cout << "\n=== HASIL LEVEL " << levelIndex << " ===\n";
        std::cout << "Total score level: " << levelScore << "\n";
        std::cout << "Target score: " << targetScore << "\n";
        std::cout << "Total gold tersimpan: " << playerGold << "\n";

        if (levelScore < targetScore) {
            std::cout << "GAME OVER. Kamu gagal capai target level.\n";
            break;
        }

        std::cout << "MENANG LEVEL.\n";

        targetScore += 30;
        levelIndex += 1;

        std::cout << "Lanjut level berikutnya? (y/n): ";
        char ans = 'y';
        std::cin >> ans;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!(ans == 'y' || ans == 'Y')) {
            break;
        }
    }
}