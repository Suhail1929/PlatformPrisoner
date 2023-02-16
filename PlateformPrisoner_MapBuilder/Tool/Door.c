#include "Door.h"
int nb_door = 1;
Door_t *initDoor(int largeur, int hauteur)
{
    Door_t *door = malloc(sizeof(Door_t));
    door->largeur = largeur;
    door->hauteur = hauteur;
    door->id_door = nb_door;
    door->key = NULL;
    return door;
}
void drawDoor(window_t *window, int x, int y, int color)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            window_mvaddch_col(window, y + j, x + i, color, ' ' | A_REVERSE);
        }
    }
    window_mvaddch(window, y, x, '0');
    window_mvaddch(window, y, x + 1, "123456789"[nb_door - 1]);
}

void incrementDoor()
{
    if (nb_door < 9)
    {
        nb_door++;
    }
}

void decrementDoor()
{
    if (nb_door > 1)
    {
        nb_door--;
    }
}
