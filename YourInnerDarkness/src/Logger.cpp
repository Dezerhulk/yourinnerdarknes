#include "Logger.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>

void Logger::log(const std::string& message) {
    std::ofstream file("game.log", std::ios::app);
    if (!file) {
        std::cerr << "Logger: unable to open log file\n";
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    file << std::put_time(std::localtime(&t), "%F %T") << " - " << message << std::endl;
}
