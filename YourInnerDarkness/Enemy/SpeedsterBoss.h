#pragma once
#include "Enemy.h"
#include <cstdlib>

class SpeedsterBoss : public Enemy {
public:
    SpeedsterBoss() : Enemy("Speedster Boss", 120, 12, 30) {}

    int getAttack() override {
        return attack + (rand() % 10);
    }
};
