#ifndef PROBE_H
#define PROBE_H

#include "../Window/window.h"

typedef struct Probe
{
    int largeur;
    int hauteur;
} Probe_t;

Probe_t *initProbe(int largeur, int hauteur);
void drawProbe(window_t *window, int x, int y, int color);

#endif