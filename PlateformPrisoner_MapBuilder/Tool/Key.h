#ifndef KEY_H
#define KEY_H

#include "../Window/window.h"

typedef struct Key
{
    int hauteur;
    int largeur;
} Key_t;
Key_t *initKey(int largeur, int hauteur);
void drawKey(window_t *window, int x, int y, int color);

#endif