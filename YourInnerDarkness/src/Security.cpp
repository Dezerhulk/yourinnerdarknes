#include "Security.h"
#include "Logger.h"
#include <iostream>

bool Security::validateInput(int choice, int min, int max) {
    if (choice < min || choice > max) {
        Logger::log("Invalid input detected (403)");
        return false;
    }
    return true;
}

bool Security::antiCheat(int hp) {
    constexpr int MAX_ALLOWED_HP = 200;
    if (hp > MAX_ALLOWED_HP) {
        Logger::log("Cheat detected! (500)");
        return true;
    }
    return false;
}
