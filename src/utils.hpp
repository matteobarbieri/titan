#ifndef ROGUE_20177_UTILS
#define ROGUE_20177_UTILS

#include <vector>

class Entity;

/**
 * Computes the l2 norm (Euclidean distance) of two points
 */
float l2(int x1, int y1, int x2, int y2);

/**
 * Computes the l1 norm (Manhattan distance) of two points
 */
int l1(int x1, int y1, int x2, int y2);

Entity * get_blocking_entities_at_location(
        std::vector<Entity *> entities, int destination_x, int destination_y);

#endif
