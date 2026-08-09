#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include "Subjects.h"

#include <string>

void inputIntegerInRange(int& target, int min, int max, const std::string& prompt);
std::string inputString(const std::string& prompt);
Book inputBook ();
void waitEnter();

#endif