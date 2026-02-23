#pragma once

class Player {
private:
    int hp = 150;
    int attack = 20;
    int xp = 0;

public:
    void takeDamage(int dmg);
    void addXP(int amount);

    bool isAlive() const;
    int getAttack() const;
};
