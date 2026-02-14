#include "Game.h"
#include "../Enemy/EnemyFactory.h"
#include <iostream>

void Game::run()
{
    std::cout << "=== Your Inner Darkness ===\n";

    while (player.isAlive())
    {
        fight();
    }

    std::cout << "\nGame Over\n";
}

void Game::fight()
{
    Enemy* enemy = EnemyFactory::createRandomEnemy();

    std::cout << "\nEnemy appears!\n";
    enemy->printStats();

    while (enemy->isAlive() && player.isAlive())
    {
        std::cout << "Press 1 to attack > ";
        int choice;
        std::cin >> choice;

        enemy->takeDamage(player.getAttack());

        enemy->specialAbility();

        if (enemy->isAlive())
            player.takeDamage(enemy->getAttack());
    }

    if (player.isAlive())
    {
        player.addXP(enemy->getXP());
        std::cout << "Enemy defeated!\n";
    }

    delete enemy;
}
