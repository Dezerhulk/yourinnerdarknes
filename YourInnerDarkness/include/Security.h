#pragma once

class Security {
public:
    static bool validateInput(int choice, int min, int max);
    // returns true if a violation (cheat) is detected
    static bool antiCheat(int hp);
};
