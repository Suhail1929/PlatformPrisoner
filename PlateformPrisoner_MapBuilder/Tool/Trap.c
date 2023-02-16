#include "Trap.h"
Trap_t *initTrap(int largeur, int hauteur, int degat)
{
    Trap_t *trap = malloc(sizeof(Trap_t));
    trap->largeur = largeur;
    trap->hauteur = hauteur;
    trap->degat = degat;
    return trap;
}

void drawTrap(window_t *window, int x, int y, int couleur)
{
    window_mvaddch_col(window, y, x, couleur, '#' | A_REVERSE);
}