#include <stdlib.h>
#include <ncurses.h>
#include "item.h"

item_t init_item(unsigned int id, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    item_t item;
    item.id = id;
    item.x = x;
    item.y = y;
    item.width = width;
    item.height = height;
    return item;
}