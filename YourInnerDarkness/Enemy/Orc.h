#pragma once
#include "Enemy.h"

class Orc : public Enemy {
public:
    Orc() : Enemy("Fear", 70, 12, 15) {}
    int getAttack() override { return attack; }
};
