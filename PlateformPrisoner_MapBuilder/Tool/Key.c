#include "Key.h"

Key_t *initKey(int largeur, int hauteur)
{
    Key_t *key = malloc(sizeof(Key_t));
    key->hauteur = hauteur;
    key->largeur = largeur;
    return key;
}

void drawKey(window_t *window, int x, int y, int color)
{
    window_mvaddch_col(window, y, x, color, ' ' | A_REVERSE);
    window_mvaddch_col(window, y + 1, x, color, ' ' | ACS_LLCORNER);
}