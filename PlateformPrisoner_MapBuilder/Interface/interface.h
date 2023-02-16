#ifndef _INTERFACE_
#define _INTERFACE_

#define DELETE 0
#define BLOCK 1
#define LADDER 2
#define TRAP 3
#define GATE 4
#define KEY 5
#define DOOR 6
#define EXIT 7
#define START 8
#define ROBOT 9
#define PROBE 10
#define LIFE 11
#define BOMB 12
#define LEVEL 16
#define CLEAR 18
#define PLAYER 19
#include <stdlib.h>
#include <ncurses.h>
#include "../Window/window.h"


// Structure représentant l'interface de l'application
typedef struct interface
{
    window_t *win_infos;        // La fenêtre d'informations
    window_t *win_level;        // La fenêtre du jeu
    window_t *win_tools;        // La fenêtre des outils
    unsigned int selection;     // La position sélectionnée
    unsigned int current_color; // La couleur du gate sélectionné
} interface_t;

interface_t *interface_create();
void interface_delete(interface_t **interface);
void interface_tools_update(interface_t *interface);
void interface_tools_actions(interface_t *interface, int posX, int posY);
void interface_actions(interface_t *interface, int c);
void outliner(interface_t *interface);
void interface_level_actions(interface_t *interface, int posX, int posY);

#endif
