#ifndef OBJET_H
#define OBJET_H

union type
{
    Block_t *block;
    Bomb_t *bomb;
    Door_t *door;
    Exit_t *exit;
    Player_t *player;
    Key_t *key;
    Gate_t *gate;
    Ladder_t *ladder;
    Life_t *life;
    Probe_t *probe;
    Robot_t *robot;
    Start_t *start;
    Trap_t *trap;
};

typedef struct Objet
{
    int largeur;
    int hauteur;
    int couleur;
    int id_type;
    union type type;

} Objet_t;

Objet_t *initObjet(int largeur, int hauteur, int couleur, int id_type);
void drawObjet(Objet_t objet, window_t *window, int x, int y);
void deleteObjet(Objet_t *objet);
#endif