#pragma once

#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class Game {
private:
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;

    void fight(Enemy& enemy);

public:
    Game();
    void run();
};
