#include <math.h>

#include "utils.hpp"


float l2(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
