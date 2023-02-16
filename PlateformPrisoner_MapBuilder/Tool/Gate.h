#ifndef GATE_H
#define GATE_H

#include "../Window/window.h"

typedef struct Gate
{
    int largeur;
    int hauteur;
} Gate_t;

Gate_t *initGate(int largeur, int hauteur);
void drawGate(window_t *window, int x, int y, int color);

#endif // GATE_H