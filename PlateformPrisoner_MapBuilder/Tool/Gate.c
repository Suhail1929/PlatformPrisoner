#include "Gate.h"

Gate_t *initGate(int largeur, int hauteur)
{
    Gate_t *gate = malloc(sizeof(Gate_t));
    gate->largeur = largeur;
    gate->hauteur = hauteur;
    return gate;
}
void drawGate(window_t *window, int x, int y, int color)
{
    for (size_t i = 0; i < 4; i++)
    {
        window_mvaddch_col(window, y + i, x, color, ' ' | ACS_PLUS);
    }
}