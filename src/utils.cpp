#include <math.h>

#include "utils.hpp"

#include "Entity.hpp"

int l1(int x1, int y1, int x2, int y2)
{
    return abs(x1-x2) + abs(y1-y2);
}

float l2(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

Entity * get_entities_at_location(
        std::vector<Entity *> entities, int destination_x, int destination_y, bool blocking)
{
    for (int i=0; i<(int)entities.size(); i++)
    {
        if (entities[i]->blocks() == blocking &&
            entities[i]->x == destination_x &&
            entities[i]->y == destination_y)

            return entities[i];
    }

    return nullptr;
}
