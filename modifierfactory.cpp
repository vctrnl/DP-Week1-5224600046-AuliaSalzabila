#include "ModifierFactory.h"

ModifierCard ModifierFactory::CreateDoubleIt() {
    return ModifierCard(ModifierType::DoubleIt, "Double It", 15);
}

ModifierCard ModifierFactory::CreateGiveMeMore() {
    return ModifierCard(ModifierType::GiveMeMore, "Give Me More", 12);
}