#include <stdlib.h>
#include <ncurses.h>
#include "../Window/window.h"
#include "../Level/enum.h"
#include "../Fonction/fonction.h"
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
#include "../Objet/objet.h"
#include "../Level/niveau.h"
#include "interface.h"

char level[15];

int level_nb = 1;

interface_t *interface_create(niveau_t *niveau)
{
    interface_t *result;
    // Initialisation du niveau

    // Structure allocation
    if ((result = malloc(sizeof(interface_t))) == NULL)
    {
        ncurses_stop();
        perror("Erreur allocation interface");
        exit(EXIT_FAILURE);
    }

    // fenetre informations
    result->win_infos = window_create(0, 22, 77, 5, "Informations", TRUE);
    window_printw_col(result->win_infos, RED, "Entrez sur 'ECHAP' pour quitter\n");
    window_refresh(result->win_infos);

    // fenetre coordonnées
    result->win_level = window_create(0, 0, 62, 22, "Level", FALSE);

    // player
    niveau->objet[5][25] = ID_PLAYER;
    drawPlayer(result->win_level, 25, 5, RED, BLUE);

    // outliner
    outliner(result);
    window_refresh(result->win_level);
    // fenetre tools
    result->win_tools = window_create(62, 0, 15, 22, "Tools", FALSE);

    result->current_color = MAGENTA; // default color
    result->selection = BLOCK;       // default selection
    interface_tools_update(result);
    window_refresh(result->win_tools);

    return result;
}

void outliner(interface_t *interface)
{
    for (int i = 0; i <= 59; i++)
    {
        window_mvaddch_col(interface->win_level, 0, i, CYAN, ' ' | A_REVERSE);
        window_mvaddch_col(interface->win_level, 19, i, CYAN, ' ' | A_REVERSE);
    }
    for (int i = 0; i <= 19; i++)
    {
        window_mvaddch_col(interface->win_level, i, 0, CYAN, ' ' | A_REVERSE);
        window_mvaddch_col(interface->win_level, i, 59, CYAN, ' ' | A_REVERSE);
    }
}

void interface_delete(interface_t **interface)
{
    window_delete(&(*interface)->win_infos);
    window_delete(&(*interface)->win_level);
    window_delete(&(*interface)->win_tools);

    free(*interface);
    interface = NULL;
}

/**
 * Update palette window
 * @param[in,out] interface the interface
 */
void interface_tools_update(interface_t *interface)
{
    window_erase(interface->win_tools);

    if (interface->selection == DELETE)
    {
        window_printw_col(interface->win_tools, WHITE, ">");
        window_color(interface->win_tools, WHITE);
    }
    else
    {
        window_printw_col(interface->win_tools, WHITE, " ");
        window_color(interface->win_tools, RED);
    }

    window_printw(interface->win_tools, " Delete\n");

    if (interface->selection == BLOCK)
    {
        window_printw_col(interface->win_tools, WHITE, ">");
        window_color(interface->win_tools, WHITE);
    }
    else
    {
        window_printw_col(interface->win_tools, WHITE, " ");
        window_color(interface->win_tools, RED);
    }

    window_printw(interface->win_tools, " Block\n");

    if (interface->selection == LADDER)
    {
        window_printw_col(interface->win_tools, WHITE, ">");
        window_color(interface->win_tools, WHITE);
    }
    else
    {
        window_printw_col(interface->win_tools, WHITE, " ");
        window_color(interface->win_tools, RED);
    }

    window_printw(interface->win_tools, " Ladder\n");

    if (interface->selection == TRAP)
    {
        window_printw_col(interface->win_tools, WHITE, ">");
        window_color(interface->win_tools, WHITE);
    }
    else
    {
        window_printw_col(interface->win_tools, WHITE, " ");
        window_color(interface->win_tools, RED);
    }

    window_printw(interface->win_tools, " Trap\n");

    if (interface->selection == GATE)
    {
        window_printw_col(interface->win_tools, WHITE, ">");
        window_printw(interface->win_tools, " Gate\n");
        window_color(interface->win_tools, WHITE);
    }
    else
    {
        window_printw_col(interface->win_tools, WHITE, " ");
        window_color(interface->win_tools, RED);
        window_printw(interface->win_tools, " Gate\n");
    }
    window_mvprintw_col(interface->win_tools, 4, 8, FD_MAGENTA, " ");
    window_mvprintw_col(interface->win_tools, 4, 9, FD_GREEN, " ");
    window_mvprintw_col(interface->win_tools, 4, 10, FD_YELLOW, " ");
    window_mvprintw_col(interface->win_tools, 4, 11, FD_BLUE, " \n");

    if (interface->selection == KEY)
    {
        window_printw_col(interface->win_tools, WHITE, ">");
        window_color(interface->win_tools, WHITE);
    }
    else
    {
        window_printw_col(interface->win_tools, WHITE, " ");
        window_color(interface->win_tools, RED);
    }
    window_printw(interface->win_tools, " Key\n");

    switch (interface->current_color)
    {
    case MAGENTA:
        window_mvprintw_col(interface->win_tools, 5, 8, WHITE, "^   \n");
        break;
    case GREEN:
        window_mvprintw_col(interface->win_tools, 5, 8, WHITE, " ^  \n");
        break;
    case YELLOW:
        window_mvprintw_col(interface->win_tools, 5, 8, WHITE, "  ^ \n");
        break;
    case BLUE:
        window_mvprintw_col(interface->win_tools, 5, 8, WHITE, "   ^\n");
        break;
    default:
        break;
    }

    window_printw_col(interface->win_tools, WHITE, (interface->selection == DOOR) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == DOOR) ? WHITE : RED);

    // char Door_num[12];
    // sprintf(Door_num, "  DOOR  %d\n", nb_door);
    window_printw(interface->win_tools, " Door  ");
    if (nb_door < 10)
    {
        window_printw_col(interface->win_tools, WHITE, "<0%d>\n", nb_door);
    }

    window_printw_col(interface->win_tools, WHITE, (interface->selection == EXIT) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == EXIT) ? WHITE : RED);

    window_printw(interface->win_tools, " Exit\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == START) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == START) ? WHITE : RED);

    window_printw(interface->win_tools, " Start\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ROBOT) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ROBOT) ? WHITE : RED);

    window_printw(interface->win_tools, " Robot\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == PROBE) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == PROBE) ? WHITE : RED);

    window_printw(interface->win_tools, " Probe\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == LIFE) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == LIFE) ? WHITE : RED);

    window_printw(interface->win_tools, " Life\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == BOMB) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == BOMB) ? WHITE : RED);

    window_printw(interface->win_tools, " Bomb\n");

    window_mvprintw_col(interface->win_tools, 14, 0, (interface->selection == LEVEL) ? YELLOW : RED, "Current level");
    if (level_nb < 10)
    {
        sprintf(level, "< 00%d >", level_nb);
    }
    else
    {
        sprintf(level, "< 0%d >", level_nb);
    }
    window_mvprintw_col(interface->win_tools, 16, 3, (interface->selection == LEVEL) ? YELLOW : WHITE, level);
    window_mvprintw_col(interface->win_tools, 18, 2, (interface->selection == CLEAR) ? YELLOW : RED, "  CLEAR");

    window_refresh(interface->win_tools);
}

/**
 * Manage actions in the palette window.
 * @param[in,out] interface the interface
 * @param[in] posX X position of the click in the window
 * @param[in] posY Y position of the click in the window
 */
void interface_tools_actions(interface_t *interface, int posX, int posY)
{
    window_mvprintw_col(interface->win_infos, 1, 0, WHITE, "X : %d Y : %d\n", posX, posY);
    window_refresh(interface->win_tools);
    window_refresh(interface->win_infos);

    if ((posY >= 0) && (posY <= 18))
    {
        switch (posY)
        {
        case DELETE:
            interface->selection = DELETE;
            break;
        case BLOCK:
            interface->selection = BLOCK;
            break;
        case LADDER:
            interface->selection = LADDER;
            break;
        case TRAP:
            interface->selection = TRAP;
            break;
        case GATE:
            switch (posX)
            {
                // add color to struct for each case
            case 8:
                interface->current_color = MAGENTA;
                break;
            case 9:
                interface->current_color = GREEN;
                break;
            case 10:
                interface->current_color = YELLOW;
                break;
            case 11:
                interface->current_color = BLUE;
                break;
            }
            if (posX < 8)
            {
                interface->selection = GATE;
            }
            break;
        case KEY:
            interface->selection = KEY;
            break;
        case DOOR:
            interface->selection = DOOR;
            if (posX == 8)
            {
                decrementDoor();
            }
            else if (posX == 11)
            {
                incrementDoor();
            }
            break;
        case EXIT:
            interface->selection = EXIT;
            break;
        case START:
            interface->selection = START;
            break;
        case ROBOT:
            interface->selection = ROBOT;
            break;
        case PROBE:
            interface->selection = PROBE;
            break;
        case LIFE:
            interface->selection = LIFE;
            break;
        case BOMB:
            interface->selection = BOMB;
            break;
        case LEVEL:
            interface->selection = LEVEL;
            if ((posX >= 3 && posX <= 4) && level_nb > 1)
            {
                level_nb--;
            }
            else if ((posX >= 8 && posX <= 9) && level_nb < 99)
            {
                level_nb++;
            }
            break;
        case CLEAR:
            interface->selection = CLEAR;
            window_erase(interface->win_level);
            outliner(interface);
            window_refresh(interface->win_level);
            break;
        default:
            break;
        }
        interface_tools_update(interface);
    }
}

void interface_level_actions(interface_t *interface, niveau_t *niveau, int posY, int posX)
{
    switch (interface->selection)
    {
    case DELETE:
        window_mvaddch(interface->win_level, posX, posY, ' ');
        break;
    case BLOCK:

        drawBlock(interface->win_level, posY, posX, CYAN);
        niveau->objet[posX][posY] = ID_BLOCK;
        break;
    case LADDER:
        drawLadder(interface->win_level, posY, posX, YELLOW);
        niveau->objet[posX][posY] = ID_LADDER;
        break;
    case TRAP:
        drawTrap(interface->win_level, posY, posX, CYAN);
        niveau->objet[posX][posY] = ID_TRAP;
        break;
    case GATE:
        drawGate(interface->win_level, posY, posX, interface->current_color);
        switch (interface->current_color)
        {
        case MAGENTA:
            niveau->objet[posX][posY] = ID_PURPLE_GATE;
            break;
        case GREEN:
            niveau->objet[posX][posY] = ID_GREEN_GATE;
            break;
        case YELLOW:
            niveau->objet[posX][posY] = ID_YELLOW_GATE;
            break;
        case BLUE:
            niveau->objet[posX][posY] = ID_BLUE_GATE;
            break;
        default:
            break;
        }
        break;
    case KEY:
        drawKey(interface->win_level, posY, posX, interface->current_color);
        switch (interface->current_color)
        {
        case MAGENTA:
            niveau->objet[posX][posY] = ID_PURPLE_KEY;
            break;
        case GREEN:
            niveau->objet[posX][posY] = ID_GREEN_KEY;
            break;
        case YELLOW:
            niveau->objet[posX][posY] = ID_YELLOW_KEY;
            break;
        case BLUE:
            niveau->objet[posX][posY] = ID_BLUE_KEY;
            break;
        default:
            break;
        }
        break;
    case DOOR:
        drawDoor(interface->win_level, posY, posX, interface->current_color);
        switch (interface->current_color)
        {
        case MAGENTA:
            switch (nb_door)
            {
            case 1:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR1;
                break;
            case 2:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR2;
                break;
            case 3:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR3;
                break;
            case 4:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR4;
                break;
            case 5:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR5;
                break;
            case 6:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR6;
                break;
            case 7:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR7;
                break;
            case 8:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR8;
                break;
            case 9:
                niveau->objet[posX][posY] = ID_PURPLE_DOOR9;
                break;
            default:
                break;
            }
            break;
        case GREEN:
            switch (nb_door)
            {
            case 1:
                niveau->objet[posX][posY] = ID_GREEN_DOOR1;
                break;
            case 2:
                niveau->objet[posX][posY] = ID_GREEN_DOOR2;
                break;
            case 3:
                niveau->objet[posX][posY] = ID_GREEN_DOOR3;
                break;
            case 4:
                niveau->objet[posX][posY] = ID_GREEN_DOOR4;
                break;
            case 5:
                niveau->objet[posX][posY] = ID_GREEN_DOOR5;
                break;
            case 6:
                niveau->objet[posX][posY] = ID_GREEN_DOOR6;
                break;
            case 7:
                niveau->objet[posX][posY] = ID_GREEN_DOOR7;
                break;
            case 8:
                niveau->objet[posX][posY] = ID_GREEN_DOOR8;
                break;
            case 9:
                niveau->objet[posX][posY] = ID_GREEN_DOOR9;
                break;
            default:
                break;
            }
            break;
        case YELLOW:
            switch (nb_door)
            {
            case 1:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR1;
                break;
            case 2:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR2;
                break;
            case 3:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR3;
                break;
            case 4:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR4;
                break;
            case 5:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR5;
                break;
            case 6:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR6;
                break;
            case 7:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR7;
                break;
            case 8:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR8;
                break;
            case 9:
                niveau->objet[posX][posY] = ID_YELLOW_DOOR9;
                break;
            default:
                break;
            }
            break;
        case BLUE:
            switch (nb_door)
            {
            case 1:
                niveau->objet[posX][posY] = ID_BLUE_DOOR1;
                break;
            case 2:
                niveau->objet[posX][posY] = ID_BLUE_DOOR2;
                break;
            case 3:
                niveau->objet[posX][posY] = ID_BLUE_DOOR3;
                break;
            case 4:
                niveau->objet[posX][posY] = ID_BLUE_DOOR4;
                break;
            case 5:
                niveau->objet[posX][posY] = ID_BLUE_DOOR5;
                break;
            case 6:
                niveau->objet[posX][posY] = ID_BLUE_DOOR6;
                break;
            case 7:
                niveau->objet[posX][posY] = ID_BLUE_DOOR7;
                break;
            case 8:
                niveau->objet[posX][posY] = ID_BLUE_DOOR8;
                break;
            case 9:
                niveau->objet[posX][posY] = ID_BLUE_DOOR9;
                break;
            default:
                break;
            }
            break;
        }
        break;
    case EXIT:
        drawExit(interface->win_level, posY, posX, GREEN);
        niveau->objet[posX][posY] = ID_EXIT;
        break;
    case START:
        drawStart(interface->win_level, posY, posX, BLUE);
        niveau->objet[posX][posY] = ID_START;
        break;
    case ROBOT:
        drawRobot(interface->win_level, posY, posX, WHITE);
        niveau->objet[posX][posY] = ID_ROBOT;
        break;
    case PROBE:
        drawProbe(interface->win_level, posY, posX, WHITE);
        niveau->objet[posX][posY] = ID_PROBE;
        break;
    case LIFE:
        drawLife(interface->win_level, posY, posX, GREEN);
        niveau->objet[posX][posY] = ID_LIFE;
        break;
    case BOMB:
        drawBomb(interface->win_level, posY, posX, WHITE);
        niveau->objet[posX][posY] = ID_BOMB;
        break;
    default:
        break;
    }
    window_refresh(interface->win_level);
}

void interface_actions(interface_t *interface, niveau_t *niveau, int c)
{
    int mouseX, mouseY, posX, posY;

    // Mouse management
    if ((c == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {
        if (window_getcoordinates(interface->win_tools, mouseX, mouseY, &posX, &posY))
        {
            interface_tools_actions(interface, posX, posY);
        }
        else if (window_getcoordinates(interface->win_level, mouseX, mouseY, &posX, &posY))
        {
            interface_level_actions(interface, niveau, posX, posY);
        }
    }
}
