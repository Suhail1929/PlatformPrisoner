#ifndef _INTERFACE_
#define _INTERFACE_

#define Delete 0
#define Block 1
#define Ladder 2
#define Trap 3
#define Gate 4
#define Key 5
#define Door 6
#define Exit 7
#define Start 8
#define Robot 9
#define Probe 10
#define Life 11
#define Bomb 12
#define LEVEL 16
#define CLEAR 18

#define HEIGHT 20
#define WIDTH 60

// Structure représentant l'interface de l'application
typedef struct
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
void interface_level_actions(interface_t *interface, int posX, int posY, int restore);
void outliner(interface_t *interface);
int insertEntityID(int posX, int posY, int largeur, int hauteur, int entityID);
int updateEntity(interface_t *interface, int posX, int posY, int action);
void getHeadEntity(int *posX, int *posY, int bloc_width, int bloc_height);
void clearMapID();
void displayMapID();
#endif
