#pragma once
#include "Enemy.h"

class TimeBoss : public Enemy {
public:
    TimeBoss() : Enemy("Time Boss", 130, 15, 40) {}

    int getAttack() override {
        return attack * 2;
    }
};
