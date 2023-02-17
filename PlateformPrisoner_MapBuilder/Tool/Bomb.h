#ifndef BOMB_H
#define BOMB_H

typedef struct Bomb
{
    int largeur;
    int hauteur;
    int degat;
} Bomb_t;

Bomb_t *initBomb(int hauteur,int largeur,int degat);
void drawBomb(window_t *window, int x, int y, int couleur);
#endif // BOMB_H