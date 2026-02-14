#include "Player.h"
#include <algorithm>

Player::Player() : hp(100), attack(15), confidence(0) {}

void Player::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

void Player::heal(int value) {
    hp = std::min(hp + value, MAX_HP);
}

void Player::increaseConfidence(int value) {
    confidence += value;
    attack += value / 2;
}

int Player::getHP() const { return hp; }
int Player::getAttack() const { return attack; }
int Player::getConfidence() const { return confidence; }

bool Player::isAlive() const {
    return hp > 0;
}

void Player::setHP(int newHp) {
    if (newHp < 0) newHp = 0;
    if (newHp > MAX_HP) newHp = MAX_HP;
    hp = newHp;
}
