#ifndef LIFE_H
#define LIFE_H

#include "../Window/window.h"

typedef struct Life
{
    int point_de_vie;
    int largeur;
    int hauteur;
} Life_t;

Life_t *initLife(int largeur, int hauteur, int point_de_vie);
void drawLife(window_t *window, int x, int y, int color);

#endif
