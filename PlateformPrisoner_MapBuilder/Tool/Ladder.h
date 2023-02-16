#ifndef LADDER_H
#define LADDER_H

#include "../Window/window.h"

typedef struct Ladder
{
    int largeur;
    int hauteur;
} Ladder_t;
Ladder_t *initLadder(int largeur, int hauteur);
void drawLadder(window_t *window, int x, int y, int color);

#endif