#include <ncurses.h>
#include <stdlib.h>
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "item.h"
#include "interface.h"
#include "entity.h"

/**
 * @brief : Get the detail of the entity, like the width, the height and the number of doors by reference
 * @param interface: the interface of the application
 * @param tab : the map of the level
 * @param posX : the position X of the entity
 * @param posY : the position Y of the entity
 * @param bloc_width : the width of the entity
 * @param bloc_height : the height of the entity
 * @param nb_door : the number of door of the entity
 */
int getEntityDetail(interface_t *interface, int tab[20][60], int posX, int posY, int *bloc_width, int *bloc_height, int *nb_door)
{
    *nb_door = 1;
    int previous_selection = interface->selection;

    interface->selection = tab[posY][posX];
    switch (interface->selection)
    {
    case 11 ... 14:                                           // ID_GATE (en 1x1)
        interface->current_color = interface->selection % 10; // 2eme chiffre : couleur
    case 1 ... 4:                                             // ID_BLOCK, ID_LIFE, ID_BOMB, ID_TRAP
        *bloc_width = 1, *bloc_height = 1;
        break;
    case 5: // ID_LADDER
        *bloc_width = 3, *bloc_height = 1;
        break;
    case 6: // ID_PROBE
        *bloc_width = 2, *bloc_height = 3;
        break;
    case 3101 ... 3499:                                       // ID_DOOR(3) + color(1-4) + nb_door(01-99)
        *nb_door = interface->selection % 100;                // 2 derniers chiffres : nb_door
    case 41 ... 44:                                           // ID_PLAYER(4) + color(1-4)
        interface->current_color = interface->selection % 10; // 2eme chiffre : couleur pour ID_PLAYER
        if (interface->selection > 3000)
            interface->current_color = (interface->selection / 100) % 10; // 2eme chiffre : couleur pour ID_DOOR
        else
            interface->current_color = interface->selection % 10; // 2eme chiffre : couleur pour ID_PLAYER
    case 7 ... 9:                                                 // ID_ROBOT, ID_EXIT, ID_START
        *bloc_width = 3, *bloc_height = 4;
        break;
    case 21 ... 24:                                           // ID_KEY
        interface->current_color = interface->selection % 10; // 2eme chiffre : couleur
        *bloc_width = 1,
        *bloc_height = 2;
        break;
    default: // ID_DELETE
        interface->selection = ID_DELETE;
        break;
    }
    return (previous_selection == ID_DELETE) ? 0 : -1;
}
