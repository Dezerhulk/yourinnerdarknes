#pragma once
#include "Enemy.h"

class Orc : public Enemy {
public:
    Orc() : Enemy("Fear", 75, 16, 10) {}
    int getAttack() override { return attack; }
};
