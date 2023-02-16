#include "Start.h"

Start_t *initStart(int largeur, int hauteur)
{
    Start_t *start = malloc(sizeof(Start_t));
    start->largeur = largeur;
    start->hauteur = hauteur;
    return start;
}
void drawStart(window_t *window, int x, int y, int color)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            window_mvaddch_col(window, y + j, x + i, color, ' ' | A_REVERSE);
        }
    }
}
