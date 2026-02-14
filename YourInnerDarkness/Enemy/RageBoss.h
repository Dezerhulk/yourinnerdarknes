#pragma once
#include "Enemy.h"

class RageBoss : public Enemy {
public:
    RageBoss() : Enemy("Rage Boss", 150, 20, 50) {}

    int getAttack() override {
        return attack + 10;
    }
};
