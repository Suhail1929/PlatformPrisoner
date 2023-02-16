#ifndef EXIT_H
#define EXIT_H

#include "../Window/window.h"

typedef struct Exit
{
    int largeur;
    int hauteur;
} Exit_t;

Exit_t *initExit(int largeur, int hauteur);
void drawExit(window_t *window, int x, int y, int color);

#endif // EXIT_H