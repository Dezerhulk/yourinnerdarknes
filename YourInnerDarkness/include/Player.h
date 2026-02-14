#pragma once

class Player {
private:
    static constexpr int MAX_HP = 200;
    int hp;
    int attack;
    int confidence;

public:
    Player();

    void takeDamage(int dmg);
    void heal(int value);
    void increaseConfidence(int value);

    int getHP() const;
    int getAttack() const;
    int getConfidence() const;
    bool isAlive() const;

    // For handling security-related forced changes
    void setHP(int newHp);
};
