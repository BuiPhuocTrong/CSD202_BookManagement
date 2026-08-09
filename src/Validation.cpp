#include "../header/Validation.h"

bool isIntegerBelongRange(const std::string& s, int min, int max) {
    try {
        size_t pos;
        int num = std::stoi(s, &pos);

        // Check if the whole string is an integer
        if (pos != s.length())
            return false;

        return (num >= min && num <= max);
    }
    catch (...) {
        return false;
    }
}