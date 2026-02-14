#pragma once
#include <string>

class Enemy {
protected:
    std::string name;
    int hp;
    int attack;
    int xpReward;

public:
    Enemy(std::string n, int h, int a, int xp);

    virtual void specialAbility();
    virtual int getAttack();

    void takeDamage(int dmg);
    bool isAlive() const;
    int getXP() const;

    virtual void printStats() const;

    virtual ~Enemy() = default;
};
