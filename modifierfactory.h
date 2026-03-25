#pragma once
#include "ModifierCard.h"

class ModifierFactory {
public:
    static ModifierCard CreateDoubleIt();
    static ModifierCard CreateGiveMeMore();
};