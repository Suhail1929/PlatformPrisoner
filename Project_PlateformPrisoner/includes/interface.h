// author: HAYAT RAHIM & MTARFI Souhail
#ifndef _INTERFACE_
#define _INTERFACE_

#define ID_DELETE 0
#define ID_BLOCK 1
#define ID_LIFE 2
#define ID_BOMB 3
#define ID_TRAP 4
#define ID_GATE 10
#define ID_LADDER 5
#define ID_KEY 20
#define ID_PROBE 6
#define ID_ROBOT 7
#define ID_DOOR 3000
#define ID_EXIT 8
#define ID_START 9
#define ID_PLAYER 40

#define LEVEL 30 - 15
#define CLEAR 31 - 14
#define SAVE 32 - 13

#define HEIGHT 20
#define WIDTH 60

extern int tab[HEIGHT][WIDTH];

// Structure représentant l'interface de l'application
typedef struct
{
    window_t *win_infos;           // La fenêtre d'informations ou HUD (Heads Up Display)
    window_t *win_level;           // La fenêtre du jeu
    window_t *win_tools;           // La fenêtre des outils
    window_t *win_debug;           // La fenêtre de debug
    unsigned int selection;        // L'outil sélectionnée
    unsigned int current_color;    // La couleur sélectionné
    liste tab_item[HEIGHT][WIDTH]; // Tableau de Liste de pointeur vers un items (pour l'instant aucun pointeur)
    liste global_item;             // Les pointeurs pointent vers les items de cette liste (contiendra les items original de la map)
    liste tab_player;              // Tableau de joueurs
} interface_t;

/**
 * Function that create an interface
 * @param level : the level
 */
interface_t *interface_create(level_t *level);

/**
 * Function that update the level window
 * @param interface : the interface
 * @param level : the level
 */
void update_win_level(interface_t *interface, level_t *level);

/*
 * Function that update the tools window  ###
 * @param interface : the interface
 */
void interface_delete(interface_t **interface);

/*
 * Function that update the tools window
 * @param interface : the interface
 */
void interface_tools_update(interface_t *interface);

/*
 * Function that update the infos window ###
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
 * Function insert an entity ID in the map and return 0 if success or 1 if error
 * @param posX : the position X
 * @param posY : the position Y
 * @param largeur : the width
 * @param hauteur : the height
 * @param entityID : the entity ID
 */
int insertEntityID(int posX, int posY, int largeur, int hauteur, int entityID);

/*
 * Function update an entity ID in the map and return a width to jump or -1 if error
 * @param posX : the position X
 * @param posY : the position Y
 * @param action : the action if == 0 delete the entity else update the entity
 */
int updateEntity(interface_t *interface, int posX, int posY, int action);

/*
 * Function that return the head of the entity ### Function?
 * @param posX : the position X
 * @param posY : the position Y
 * @param largeur : the width
 * @param hauteur : the height
 */
void getHeadEntity(int tab[20][60], int *posX, int *posY, int bloc_width, int bloc_height);

/*
 *Function that clear the map ID ### Function?
 */
void clearMapID();

/*
 *Function that display the map ID ### Function?
 */
void displayMapID();

/*
 *Function that clear the interface ### Function?
 */
void clearInterface(interface_t *interface);

void creer_partie();
char *afficher_salons();
void convertToItem(interface_t *interface, level_t *level);
interface_t *interface_create_game(char *path);
void interface_game_actions(interface_t *interface, int c);

void interface_game_update(interface_t *interface, int c);
/**
 * Update Head-Up Display window
 * @param[in,out] interface the interface
 */
void interface_hud_update(interface_t *interface);
void interface_debug(interface_t *interface, int posX, int posY);

int is_obstacle(interface_t *interface, item_t item, int new_y, int new_x);
void init_player(interface_t *interface, int x, int y);
void find_start(interface_t *interface);
#endif
