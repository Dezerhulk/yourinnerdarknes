#include "Game.h"
#include "Security.h"
#include "Logger.h"
#include "EnemyFactory.h"
#include <iostream>
#include <limits>

Game::Game() {
    // create a few polymorphic enemies via factory
    enemies.emplace_back(EnemyFactory::createRandomEnemy());
    enemies.emplace_back(EnemyFactory::createRandomEnemy());
    enemies.emplace_back(EnemyFactory::createRandomEnemy());
}

void Game::fight(Enemy& enemy) {
    std::cout << "\nYou face: ";
    enemy.printStats();

    while (enemy.isAlive() && player.isAlive()) {
        std::cout << "1. Attack\n2. Heal\nChoice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (!Security::validateInput(choice, 1, 2))
            continue;

        if (choice == 1) {
            enemy.takeDamage(player.getAttack());
            Logger::log("Player attacked");
        }
        else {
            player.heal(15);
            Logger::log("Player healed");
        }

        if (enemy.isAlive())
            player.takeDamage(enemy.getAttack());

        if (Security::antiCheat(player.getHP())) {
            std::cout << "Security violation. Game closed.\n";
            player.setHP(0);
            return;
        }

        std::cout << "HP: " << player.getHP() << "\n";
    }

    if (player.isAlive()) {
        player.increaseConfidence(10);
        std::cout << "Enemy defeated! Confidence increased!\n";
    }
}

void Game::run() {
    for (auto& e : enemies) {
        if (!e) continue;
        fight(*e);
        if (!player.isAlive()) break;
    }

    if (player.isAlive())
        std::cout << "\nYou conquered your inner darkness!\n";
    else
        std::cout << "\nYou lost...\n";
}
