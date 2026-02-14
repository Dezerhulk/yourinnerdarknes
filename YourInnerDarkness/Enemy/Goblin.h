#pragma once
#include "Enemy.h"

class Goblin : public Enemy {
public:
    Goblin() : Enemy("Doubt", 40, 8, 10) {}
    int getAttack() override { return attack; }
};
