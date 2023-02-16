#include "interface.h"
int nb_door = 1;
char level[15];
int level_nb = 1;
interface_t *interface_create()
{
    interface_t *result;
    // Initialisation du niveau
    niveau_t *niveau = malloc(sizeof(niveau_t));
    niveau_init(niveau);

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
    window_mvaddch_col(result->win_level, 2, 32, RED, ' ' | A_REVERSE);
    window_mvaddch_col(result->win_level, 2, 31, RED, '-' | A_REVERSE);
    window_mvaddch_col(result->win_level, 3, 31, BLUE, ' ' | ACS_HLINE);
    window_mvaddch_col(result->win_level, 3, 32, BLUE, ' ' | ACS_PLUS);
    window_mvaddch_col(result->win_level, 3, 33, BLUE, ' ' | ACS_HLINE);
    window_mvaddch_col(result->win_level, 4, 31, BLUE, ' ' | ACS_ULCORNER);
    window_mvaddch_col(result->win_level, 4, 32, BLUE, ' ' | ACS_BTEE);
    window_mvaddch_col(result->win_level, 4, 33, BLUE, ' ' | ACS_URCORNER);
    window_mvaddch_col(result->win_level, 5, 31, BLUE, ' ' | ACS_VLINE);
    window_mvaddch_col(result->win_level, 5, 33, BLUE, ' ' | ACS_VLINE);

    window_refresh(result->win_level);

    // fenetre tools
    result->win_tools = window_create(62, 0, 15, 22, "Tools", FALSE);

    result->current_color = MAGENTA; // default color
    result->selection = Block;       // default selection
    interface_tools_update(result);
    window_refresh(result->win_tools);

    return result;
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

    if (interface->selection == Delete)
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

    if (interface->selection == Block)
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

    if (interface->selection == Ladder)
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

    if (interface->selection == Trap)
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

    if (interface->selection == Gate)
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

    if (interface->selection == Key)
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

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Door) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Door) ? WHITE : RED);

    // char Door_num[12];
    // sprintf(Door_num, "  Door  %d\n", nb_door);
    window_printw(interface->win_tools, " Door  ");
    if (nb_door < 10)
    {
        window_printw_col(interface->win_tools, WHITE, "<0%d>\n", nb_door);
    }

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Exit) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Exit) ? WHITE : RED);

    window_printw(interface->win_tools, " Exit\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Start) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Start) ? WHITE : RED);

    window_printw(interface->win_tools, " Start\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Robot) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Robot) ? WHITE : RED);

    window_printw(interface->win_tools, " Robot\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Probe) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Probe) ? WHITE : RED);

    window_printw(interface->win_tools, " Probe\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Life) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Life) ? WHITE : RED);

    window_printw(interface->win_tools, " Life\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == Bomb) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == Bomb) ? WHITE : RED);

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
        case Delete:
            interface->selection = Delete;
            break;
        case Block:
            interface->selection = Block;
            break;
        case Ladder:
            interface->selection = Ladder;
            break;
        case Trap:
            interface->selection = Trap;
            break;
        case Gate:
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
                interface->selection = Gate;
            }
            break;
        case Key:
            interface->selection = Key;
            break;
        case Door:
            interface->selection = Door;
            if (posX == 8 && nb_door > 1)
            {
                nb_door--;
            }
            else if (posX == 11 && nb_door < 9)
            {
                nb_door++;
            }
            break;
        case Exit:
            interface->selection = Exit;
            break;
        case Start:
            interface->selection = Start;
            break;
        case Robot:
            interface->selection = Robot;
            break;
        case Probe:
            interface->selection = Probe;
            break;
        case Life:
            interface->selection = Life;
            break;
        case Bomb:
            interface->selection = Bomb;
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
            window_refresh(interface->win_level);
            break;
        default:
            break;
        }
        interface_tools_update(interface);
    }
}

void interface_level_actions(interface_t *interface, int posX, int posY)
{
    switch (interface->selection)
    {
    case Delete:
        window_mvaddch(interface->win_level, posY, posX, ' ');
        break;
    case Block:
        window_mvaddch_col(interface->win_level, posY, posX, CYAN, ' ' | A_REVERSE);
        break;
    case Ladder:
        window_mvaddch_col(interface->win_level, posY, posX, YELLOW, ' ' | ACS_LTEE);
        window_mvaddch_col(interface->win_level, posY, posX + 1, YELLOW, ' ' | ACS_HLINE);
        window_mvaddch_col(interface->win_level, posY, posX + 2, YELLOW, ' ' | ACS_RTEE);
        break;
    case Trap:
        window_mvaddch_col(interface->win_level, posY, posX, CYAN, '#' | A_REVERSE);
        break;
    case Gate:
        for (size_t i = 0; i < 4; i++)
        {
            window_mvaddch_col(interface->win_level, posY + i, posX, interface->current_color, ' ' | ACS_PLUS);
        }
        break;
    case Key:
        window_mvaddch_col(interface->win_level, posY, posX, MAGENTA, ' ' | A_REVERSE);
        window_mvaddch_col(interface->win_level, posY + 1, posX, MAGENTA, ' ' | ACS_LLCORNER);
        break;
    case Door:
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                window_mvaddch_col(interface->win_level, posY + j, posX + i, interface->current_color, ' ' | A_REVERSE);
            }
        }
        window_mvaddch(interface->win_level, posY, posX, '0');
        window_mvaddch(interface->win_level, posY, posX + 1, "123456789"[nb_door - 1]);
        break;
    case Exit:
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                window_mvaddch_col(interface->win_level, posY + j, posX + i, YELLOW, ' ' | A_REVERSE);
            }
        }
        break;
    case Start:
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                window_mvaddch_col(interface->win_level, posY + j, posX + i, MAGENTA, ' ' | A_REVERSE);
            }
        }
        break;
    case Robot:
        window_mvaddch_col(interface->win_level, posY, posX, WHITE, ' ' | ACS_ULCORNER);
        window_mvaddch_col(interface->win_level, posY, posX + 1, WHITE, ' ' | ACS_BTEE);
        window_mvaddch_col(interface->win_level, posY, posX + 2, WHITE, ' ' | ACS_URCORNER);
        window_mvaddch_col(interface->win_level, posY + 1, posX, WHITE, ' ' | ACS_LLCORNER);
        window_mvaddch_col(interface->win_level, posY + 1, posX + 1, WHITE, ' ' | ACS_TTEE);
        window_mvaddch_col(interface->win_level, posY + 1, posX + 2, WHITE, ' ' | ACS_LRCORNER);
        window_mvaddch_col(interface->win_level, posY + 2, posX, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(interface->win_level, posY + 2, posX + 1, WHITE, ' ' | ACS_PLUS);
        window_mvaddch_col(interface->win_level, posY + 2, posX + 2, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(interface->win_level, posY + 3, posX, WHITE, ' ' | ACS_ULCORNER);
        window_mvaddch_col(interface->win_level, posY + 3, posX + 1, WHITE, ' ' | ACS_BTEE);
        window_mvaddch_col(interface->win_level, posY + 3, posX + 2, WHITE, ' ' | ACS_URCORNER);
        break;
    case Probe:
        window_mvaddch_col(interface->win_level, posY, posX, WHITE, ' ' | ACS_LTEE);
        window_mvaddch_col(interface->win_level, posY, posX + 1, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(interface->win_level, posY, posX + 2, WHITE, ' ' | ACS_RTEE);
        window_mvaddch_col(interface->win_level, posY + 1, posX, WHITE, ' ' | ACS_LLCORNER);
        window_mvaddch_col(interface->win_level, posY + 1, posX + 1, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(interface->win_level, posY + 1, posX + 2, WHITE, ' ' | ACS_LRCORNER);
        break;
    case Life:
        window_mvaddch_col(interface->win_level, posY, posX, RED, 'V');
        break;
    case Bomb:
        window_mvaddch_col(interface->win_level, posY, posX, WHITE, 'o');
        break;
    default:
        break;
    }
    window_refresh(interface->win_level);
}
// Monstre *m = (Monstre *)malloc(sizeof(Monstre));
// init_monstre(m);
// // afficher le sprite du monstre, sprite est un tableau de char
// for (int i = 0; i < 4; i++)
// {
//     for (int j = 0; j < 3; j++)
//     {
//         window_mvaddch_col(interface->win_level, posY + i, posX + j, WHITE, NCURSES_ACS(m->sprite[i][j]));
//     }
// }

// window_refresh(interface->win_level);
// free(m);
// }

void interface_actions(interface_t *interface, int c)
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
            interface_level_actions(interface, posX, posY);
        }
    }
}
