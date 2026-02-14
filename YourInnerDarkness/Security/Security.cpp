#include "Security.h"

bool Security::validRange(int value, int min, int max)
{
    return value >= min && value <= max;
}
