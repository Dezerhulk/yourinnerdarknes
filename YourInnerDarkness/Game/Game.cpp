#include "Game.h"
#include "../Enemy/EnemyFactory.h"
#include <iostream>
#include <limits>

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
        std::cout << "Press 1 to attack, 2 to defend > ";
        int choice = 0;
        
        // Input validation with error checking
        while (!(std::cin >> choice) || choice < 1 || choice > 2) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Press 1 to attack, 2 to defend > ";
        }

        // Apply player action
        if (choice == 1) {
            std::cout << "You attack!\n";
            enemy->takeDamage(player.getAttack());
        } else {
            std::cout << "You defend!\n";
        }

        enemy->specialAbility();

        // Enemy attacks with reduced damage if player defended
        if (enemy->isAlive()) {
            int damage = enemy->getAttack();
            if (choice == 2) {
                damage = damage / 2;
                std::cout << "(Reduced damage from defense)\n";
            }
            player.takeDamage(damage);
        }
    }

    if (player.isAlive())
    {
        player.addXP(enemy->getXP());
        std::cout << "Enemy defeated!\n";
    }

    delete enemy;
}
