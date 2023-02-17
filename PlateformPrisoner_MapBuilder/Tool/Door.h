#ifndef Door_H
#define Door_H


extern int nb_door;
typedef struct Door
{
    int largeur;
    int hauteur;
    int id_door;

} Door_t;

Door_t *initDoor(int largeur, int hauteur);
void drawDoor(window_t *window, int x, int y, int color);
void incrementDoor();
void decrementDoor();
#endif