#include "EnemyFactory.h"
#include "ShadowBoss.h"
#include "Goblin.h"
#include "Orc.h"
#include "RageBoss.h"
#include "TimeBoss.h"
#include "SpeedsterBoss.h"
#include <cstdlib>

Enemy* EnemyFactory::createRandomEnemy()
{
    int r = rand() % 6;

    switch (r) {
    case 0: return new Goblin();
    case 1: return new Orc();
    case 2: return new RageBoss();
    case 3: return new TimeBoss();
    case 4: return new SpeedsterBoss();
    default: return new ShadowBoss();
    }
}
