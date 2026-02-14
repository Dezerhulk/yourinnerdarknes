#include "Enemy.h"
#include <iostream>

Enemy::Enemy(std::string n, int h, int a, int xp)
    : name(n), hp(h), attack(a), xpReward(xp) {}

void Enemy::specialAbility() {}

int Enemy::getAttack()
{
    return attack;
}

void Enemy::takeDamage(int dmg)
{
    hp -= dmg;
}

bool Enemy::isAlive() const
{
    return hp > 0;
}

int Enemy::getXP() const
{
    return xpReward;
}

void Enemy::printStats() const
{
    std::cout << name << " | HP: " << hp << "\n";
}
