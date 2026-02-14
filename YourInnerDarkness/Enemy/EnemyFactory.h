#pragma once
#include "Enemy.h"

class EnemyFactory {
public:
    static Enemy* createRandomEnemy();
};
