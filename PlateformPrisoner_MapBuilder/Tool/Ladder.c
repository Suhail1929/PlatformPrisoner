#include "../Window/window.h"
#include "Ladder.h"

Ladder_t *initLadder(int largeur, int hauteur)
{
    Ladder_t *ladder = malloc(sizeof(Ladder_t));
    ladder->largeur = largeur;
    ladder->hauteur = hauteur;
    return ladder;
}

void drawLadder(window_t *window, int x, int y, int color)
{
    window_mvaddch_col(window, y, x, color, ' ' | ACS_LTEE);
    window_mvaddch_col(window, y, x + 1, color, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y, x + 2, color, ' ' | ACS_RTEE);
}