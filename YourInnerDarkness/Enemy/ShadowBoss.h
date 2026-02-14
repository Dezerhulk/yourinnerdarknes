#pragma once
#include "Enemy.h"

class ShadowBoss : public Enemy {
public:
    ShadowBoss();

    void specialAbility() override;
    int getAttack() override;
};
