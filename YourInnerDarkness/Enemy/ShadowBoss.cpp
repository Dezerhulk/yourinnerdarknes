#include "ShadowBoss.h"
#include <iostream>
#include <cstdlib>

ShadowBoss::ShadowBoss()
    : Enemy("Shadow of Fear", 200, 20, 150) {}

void ShadowBoss::specialAbility()
{
    hp += 5;
    std::cout << "Shadow absorbs darkness and heals!\n";
}

int ShadowBoss::getAttack()
{
    return attack + rand() % 10;
}
