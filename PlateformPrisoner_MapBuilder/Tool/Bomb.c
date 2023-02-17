#include <ncurses.h>
#include <stdlib.h>
#include "../Window/window.h"
#include "Bomb.h"

Bomb_t *initBomb(int hauteur, int largeur, int degat)
{
    Bomb_t *bomb = malloc(sizeof(Bomb_t));
    bomb->largeur = largeur;
    bomb->hauteur = hauteur;
    bomb->degat = degat;
    return bomb;
}

void drawBomb(window_t *window, int x, int y, int couleur)
{
    window_mvaddch_col(window, y, x, couleur, 'o');
}