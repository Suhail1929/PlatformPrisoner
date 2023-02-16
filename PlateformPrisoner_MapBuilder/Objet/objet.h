#ifndef OBJET_H
#define OBJET_H

#include "../Window/window.h"
#include "../Couleur/couleur.h"
#include "../Tool/Block.h"
#include "../Tool/Start.h"
#include "../Tool/Exit.h"
#include "../Tool/Gate.h"
#include "../Tool/Key.h"
#include "../Tool/Ladder.h"
#include "../Tool/Life.h"
#include "../Tool/Probe.h"
#include "../Tool/Robot.h"
#include "../Tool/Bomb.h"
#include "../Tool/Door.h"
#include "../Tool/Player.h"
#include "../Tool/Trap.h"
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
#endif