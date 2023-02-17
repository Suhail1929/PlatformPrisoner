#ifndef START_H
#define START_H



typedef struct Start
{
    int largeur;
    int hauteur;
} Start_t;

Start_t *initStart(int largeur, int hauteur);

void drawStart(window_t *window, int x, int y, int color);
#endif