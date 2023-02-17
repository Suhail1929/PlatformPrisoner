#ifndef PLAYER_H
#define PLAYER_H


typedef struct Player
{
    char name[20];
    int id;
    int hauteur;
    int largeur;
    int health;
} Player_t;
Player_t *initPlayer(char *name, int health, int id, int largeur, int hauteur);
void drawPlayer(window_t *window, int x, int y, int color_head, int color_body);
#endif