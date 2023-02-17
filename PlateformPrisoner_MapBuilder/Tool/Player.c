#include "../Window/window.h"
#include <string.h>
#include "Player.h"

Player_t *initPlayer(char *name, int health, int id, int largeur, int hauteur)
{
    Player_t *player = malloc(sizeof(Player_t));
    strcpy(player->name, name);
    player->health = health;
    player->id = id;
    player->largeur = largeur;
    player->hauteur = hauteur;
    return player;
}

void drawPlayer(window_t *window, int x, int y, int color_head, int color_body)
{
    window_mvaddch_col(window, y, x, color_head, ' ' | A_REVERSE);
    window_mvaddch_col(window, y, x - 1, color_head, '-' | A_REVERSE);
    window_mvaddch_col(window, y + 1, x - 1, color_body, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y + 1, x, color_body, ' ' | ACS_PLUS);
    window_mvaddch_col(window, y + 1, x + 1, color_body, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y + 2, x - 1, color_body, ' ' | ACS_ULCORNER);
    window_mvaddch_col(window, y + 2, x, color_body, ' ' | ACS_BTEE);
    window_mvaddch_col(window, y + 2, x + 1, color_body, ' ' | ACS_URCORNER);
    window_mvaddch_col(window, y + 3, x - 1, color_body, ' ' | ACS_VLINE);
    window_mvaddch_col(window, y + 3, x + 1, color_body, ' ' | ACS_VLINE);
}