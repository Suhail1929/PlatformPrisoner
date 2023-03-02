#include <ncurses.h>
#include <stdlib.h>
#include "../Couleur/couleur.h"
#include "../Window/window.h"
#include "../Data/data_table.h"
#include "../Interface/interface.h"
#include "entity.h"
/*
 * function modify the entity in the map to use it for restore the map
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
    switch (tab[posY][posX])
    {
    case ID_DELETE:
        interface->selection = 500;
        break;
    case ID_BLOCK:
        *bloc_width = 1, *bloc_height = 1;
        interface->selection = Block;
        break;
    case ID_LADDER:
        *bloc_width = 3, *bloc_height = 1;
        interface->selection = Ladder;
        break;
    case ID_TRAP:
        *bloc_width = 1, *bloc_height = 1;
        interface->selection = Trap;
        break;
    case ID_EXIT:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Exit;
        break;
    case ID_START:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Start;
        break;
    case ID_ROBOT:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Robot;
        break;
    case ID_PROBE:
        *bloc_width = 2, *bloc_height = 3;
        interface->selection = Probe;
        break;
    case ID_LIFE:
        *bloc_width = 1, *bloc_height = 1;
        interface->selection = Life;
        break;
    case ID_BOMB:
        *bloc_width = 1, *bloc_height = 1;
        interface->selection = Bomb;
        break;
    case ID_PURPLE_GATE:
        *bloc_width = 1, *bloc_height = 4;
        interface->selection = Gate;
        interface->current_color = MAGENTA;
        break;
    case ID_GREEN_GATE:
        *bloc_width = 1, *bloc_height = 4;
        interface->selection = Gate;
        interface->current_color = GREEN;
        break;
    case ID_BLUE_GATE:
        *bloc_width = 1, *bloc_height = 4;
        interface->selection = Gate;
        interface->current_color = BLUE;
        break;
    case ID_YELLOW_GATE:
        *bloc_width = 1, *bloc_height = 4;
        interface->selection = Gate;
        interface->current_color = YELLOW;
        break;
    case ID_PURPLE_KEY:
        *bloc_width = 1, *bloc_height = 2;
        interface->selection = Key;
        interface->current_color = MAGENTA;
        break;
    case ID_GREEN_KEY:
        *bloc_width = 1, *bloc_height = 2;
        interface->selection = Key;
        interface->current_color = GREEN;
        break;
    case ID_BLUE_KEY:
        *bloc_width = 1, *bloc_height = 2;
        interface->selection = Key;
        interface->current_color = BLUE;
        break;
    case ID_YELLOW_KEY:
        *bloc_width = 1, *bloc_height = 2;
        interface->selection = Key;
        interface->current_color = YELLOW;
        break;
    case ID_PURPLE_DOOR1:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 1;
        break;
    case ID_PURPLE_DOOR2:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 2;
        break;
    case ID_PURPLE_DOOR3:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 3;
        break;
    case ID_PURPLE_DOOR4:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 4;
        break;
    case ID_PURPLE_DOOR5:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 5;
        break;
    case ID_PURPLE_DOOR6:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 6;
        break;
    case ID_PURPLE_DOOR7:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 7;
        break;
    case ID_PURPLE_DOOR8:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 8;
        break;
    case ID_PURPLE_DOOR9:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = MAGENTA;
        *nb_door = 9;
        break;
    case ID_GREEN_DOOR1:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 1;
        break;
    case ID_GREEN_DOOR2:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 2;
        break;
    case ID_GREEN_DOOR3:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 3;
        break;
    case ID_GREEN_DOOR4:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 4;
        break;
    case ID_GREEN_DOOR5:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 5;
        break;
    case ID_GREEN_DOOR6:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 6;
        break;
    case ID_GREEN_DOOR7:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 7;
        break;
    case ID_GREEN_DOOR8:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 8;
        break;
    case ID_GREEN_DOOR9:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = GREEN;
        *nb_door = 9;
        break;
    case ID_BLUE_DOOR1:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 1;
        break;
    case ID_BLUE_DOOR2:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 2;
        break;
    case ID_BLUE_DOOR3:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 3;
        break;
    case ID_BLUE_DOOR4:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 4;
        break;
    case ID_BLUE_DOOR5:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 5;
        break;
    case ID_BLUE_DOOR6:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 6;
        break;
    case ID_BLUE_DOOR7:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 7;
        break;
    case ID_BLUE_DOOR8:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 8;
        break;
    case ID_BLUE_DOOR9:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = BLUE;
        *nb_door = 9;
        break;
    case ID_YELLOW_DOOR1:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 1;
        break;
    case ID_YELLOW_DOOR2:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 2;
        break;
    case ID_YELLOW_DOOR3:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 3;
        break;
    case ID_YELLOW_DOOR4:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 4;
        break;
    case ID_YELLOW_DOOR5:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 5;
        break;
    case ID_YELLOW_DOOR6:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 6;
        break;
    case ID_YELLOW_DOOR7:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 7;
        break;
    case ID_YELLOW_DOOR8:
        *bloc_width = 8, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 2;
        break;
    case ID_YELLOW_DOOR9:
        *bloc_width = 3, *bloc_height = 4;
        interface->selection = Door;
        interface->current_color = YELLOW;
        *nb_door = 9;
        break;
    default:
        // traitement par défaut
        break;
    }
    return (previous_selection == ID_DELETE) ? 0 : -1;
}
