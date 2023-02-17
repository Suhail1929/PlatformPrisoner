#include "../Window/window.h"
#include "Life.h"
Life_t *initLife(int largeur, int hauteur, int point_de_vie)
{
    Life_t *life = malloc(sizeof(Life_t));
    life->largeur = largeur;
    life->hauteur = hauteur;
    life->point_de_vie = point_de_vie;
    return life;
}

void drawLife(window_t *window, int x, int y, int color)
{
    window_mvaddch_col(window, y, x, color, 'V');
}