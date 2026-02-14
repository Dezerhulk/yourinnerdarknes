#ifndef SCANNER_H
#define SCANNER_H

#include <string>

bool isDangerous(const std::string& line);
void scanDirectory(const char* path);

#endif
