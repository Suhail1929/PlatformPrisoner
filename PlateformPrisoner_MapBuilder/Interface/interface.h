// author: HAYAT RAHIM & MTARFI Souhail
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
#define LEVEL 15
#define CLEAR 17
#define SAVE 19

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

/*
 * Function that create an interface
 * @param level : the level
 */
interface_t *interface_create(level_t *level);
/*
 * Function that update the level window
 * @param interface : the interface
 * @param level : the level
 */
void update_win_level(interface_t *interface, level_t *level);
/*
 * Function that update the tools window
 * @param interface : the interface
 */
void interface_delete(interface_t **interface);
/*
 * Function that update the tools window
 * @param interface : the interface
 */
void interface_tools_update(interface_t *interface);
/*
 * Function that update the infos window
 * @param interface : the interface
 */
void interface_tools_actions(int fd, level_t *level, interface_t *interface, int posX, int posY);
/*
 * Function that implement the actions of the interface
 * @param interface : the interface
 * @param c : the key pressed
 * @param fd : the file descriptor
 * @param level : the level
 */
void interface_actions(int fd, level_t *level, interface_t *interface, int c);
/*
 * Function that implement the actions of the interface in level
 * @param interface : the interface
 * @param posX : the position X
 * @param posY : the position Y
 * @param restore : the restore
 */
void interface_level_actions(interface_t *interface, int posX, int posY, int restore);
/*
 * Function that draw the outliner
 * @param interface : the interface
 */
void outliner(interface_t *interface);
/*
 * Function insert an entity ID in the map and return the entity ID
 * @param posX : the position X
 * @param posY : the position Y
 * @param largeur : the width
 * @param hauteur : the height
 * @param entityID : the entity ID
 */
int insertEntityID(int posX, int posY, int largeur, int hauteur, int entityID);
/*
 * Function update an entity ID in the map and return the entity ID
 * @param posX : the position X
 * @param posY : the position Y
 * @param action : the action if == 0 delete the entity else update the entity
 */
int updateEntity(interface_t *interface, int posX, int posY, int action);
/*
 * Function that return the head of the entity
 * @param posX : the position X
 * @param posY : the position Y
 * @param largeur : the width
 * @param hauteur : the height
 */
void getHeadEntity(int *posX, int *posY, int bloc_width, int bloc_height);
/*
 *Function that clear the map ID
 */
void clearMapID();
/*
 *Function that display the map ID
 */
void displayMapID();
/*
 *Function that clear the interface
 */
void clearInerface(interface_t *interface);
#endif
