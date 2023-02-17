#include "../Window/window.h"
#include "Exit.h"
Exit_t *initExit(int largeur, int hauteur)
{
    Exit_t *exit = malloc(sizeof(Exit_t));
    exit->largeur = largeur;
    exit->hauteur = hauteur;
    return exit;
}
void drawExit(window_t *window, int x, int y, int color)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            window_mvaddch_col(window, y + j, x + i, color, ' ' | A_REVERSE);
        }
    }
}