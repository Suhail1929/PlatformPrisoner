#ifndef TRAP_H
#define TRAP_H



typedef struct Trap
{
    int largeur;
    int hauteur;
    int degat;
} Trap_t;

Trap_t *initTrap(int largeur, int hauteur, int degat);
void drawTrap(window_t *window, int x, int y, int couleur);

#endif