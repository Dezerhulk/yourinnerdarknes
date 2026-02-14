#include "Player.h"
#include "../Security/Security.h"
#include <iostream>

void Player::takeDamage(int dmg)
{
    if (!Security::validRange(dmg, 0, 200))
        return;

    hp -= dmg;

    if (hp < 0)
        hp = 0;
}

void Player::addXP(int amount)
{
    if (!Security::validRange(amount, 0, 500))
        return;

    xp += amount;

    std::cout << "XP: " << xp << "\n";
}

bool Player::isAlive() const
{
    return hp > 0;
}

int Player::getAttack() const
{
    return attack;
}
