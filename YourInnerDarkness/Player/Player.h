#pragma once

class Player {
private:
    int hp = 100;
    int attack = 15;
    int xp = 0;

public:
    void takeDamage(int dmg);
    void addXP(int amount);

    bool isAlive() const;
    int getAttack() const;
};
