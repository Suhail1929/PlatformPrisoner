#ifndef _ITEM_H_
#define _ITEM_H_

typedef struct
{
    unsigned int color;
} color_t; // structure qui utilise les couleurs

typedef struct
{
    color_t color;
    unsigned int nb_door;
} door_t; // door

typedef struct
{
    unsigned int nb_key;
    unsigned int nb_life;
    unsigned int nb_bomb;
    color_t color;
} player_t; // player

typedef union
{
    color_t color;
    door_t door;
    player_t player;
} properties_t; // item's properties

typedef struct
{
    properties_t properties;
    unsigned int id;
    int x;
    int y;
    unsigned int width;
    unsigned int height;
} item_t; // item_t

item_t *init_item(unsigned int id, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void display_item(window_t *window, item_t item, int posX, int posY);

#endif // _ITEM_H_
