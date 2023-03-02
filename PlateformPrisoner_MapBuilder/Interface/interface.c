#include <stdlib.h>
#include <ncurses.h>

#include "../Fonction/fonction.h"
#include "../Couleur/couleur.h"
#include "../Window/window.h"
#include "../Data/data_table.h"
#include "interface.h"
#include "../Fonction/entity.h"
#include <limits.h>

int nb_door = 1;
char level[15];
int level_nb = 0;
int tab[HEIGHT][WIDTH];
int compteurEntity = 0;

interface_t *interface_create(level_t *level)
{
    interface_t *result;

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
    // default tab : empty
    // clearMapID();

    update_win_level(result, level);

    window_refresh(result->win_level);
    // fenetre tools
    result->win_tools = window_create(62, 0, 15, 22, "Tools", FALSE);

    result->current_color = MAGENTA; // default color
    result->selection = Block;       // default selection
    interface_tools_update(result);
    window_refresh(result->win_tools);

    return result;
}

void update_win_level(interface_t *interface, level_t *level)
{
    int i, j;
    int tmp_decalage = 0;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            tab[i][j] = level->tab[i][j];
        }
    }

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            tmp_decalage = updateEntity(interface, j, i, 1);
            if (tmp_decalage != -1)
            {
                j += tmp_decalage;
            }
        }
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
    window_mvprintw_col(interface->win_tools, 15, 3, (interface->selection == LEVEL) ? YELLOW : WHITE, level);
    window_mvprintw_col(interface->win_tools, 17, 2, (interface->selection == CLEAR) ? YELLOW : RED, "  CLEAR");
    window_mvprintw_col(interface->win_tools, 19, 2, (interface->selection == SAVE) ? YELLOW : RED, "  SAVE");
    window_refresh(interface->win_tools);
}

/**
 * Manage actions in the palette window.
 * @param[in,out] interface the interface
 * @param[in] posX X position of the click in the window
 * @param[in] posY Y position of the click in the window
 */
void interface_tools_actions(int fd, level_t *level, interface_t *interface, int posX, int posY)
{
    if ((posY >= 0) && (posY <= 20))
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
            if ((posX >= 3 && posX <= 4) && level_nb > 0)
            {
                for (int i = 0; i < HEIGHT; i++)
                {
                    for (int j = 0; j < WIDTH; j++)
                    {
                        level->tab[i][j] = tab[i][j];
                    }
                }
                level->id = level_nb;
                bloc_t *bloc = loadBloc(fd, 0);
                int offset = findLevel(fd, bloc, level_nb);
                if (offset == -1)
                {
                    level->id = level_nb;
                    addLevel(fd, bloc, level);
                    updateBloc(fd, 0, bloc);
                }
                else
                {
                    updateLevel(fd, offset, level);
                    updateBloc(fd, 0, bloc);
                }
                level_nb--;
                level = loadLevelById(fd, bloc, level_nb);
                if (level == NULL)
                {
                    clearInerface(interface);
                    level = initLevel();
                    level->id = level_nb;
                    update_win_level(interface, level);
                }
                else
                {
                    clearInerface(interface);
                    update_win_level(interface, level);
                }
            }
            else if ((posX >= 8 && posX <= 9) && level_nb < 99)
            {
                for (int i = 0; i < HEIGHT; i++)
                {
                    for (int j = 0; j < WIDTH; j++)
                    {
                        level->tab[i][j] = tab[i][j];
                    }
                }
                level->id = level_nb;
                bloc_t *bloc = loadBloc(fd, 0);
                int offset = findLevel(fd, bloc, level_nb);
                if (offset == -1)
                {
                    addLevel(fd, bloc, level);
                    updateBloc(fd, 0, bloc);
                }
                else
                {
                    updateLevel(fd, offset, level);
                    updateBloc(fd, 0, bloc);
                }
                level_nb++;
                level = loadLevelById(fd, bloc, level_nb);
                if (level == NULL)
                {
                    clearInerface(interface);
                    level = initLevel();
                    level->id = level_nb;
                    update_win_level(interface, level);
                }
                else
                {
                    clearInerface(interface);
                    update_win_level(interface, level);
                }
            }
            break;
        case CLEAR:
            clearInerface(interface);
            break;
        case SAVE:
            for (int i = 0; i < HEIGHT; i++)
            {
                for (int j = 0; j < WIDTH; j++)
                {
                    level->tab[i][j] = tab[i][j];
                }
            }
            level->id = level_nb;
            bloc_t *bloc = loadBloc(fd, 0);
            int offset = findLevel(fd, bloc, level_nb);
            if (offset == -1)
            {
                addLevel(fd, bloc, level);
                updateBloc(fd, 0, bloc);
            }
            else
            {
                updateLevel(fd, offset, level);
                updateBloc(fd, 0, bloc);
            }
            updateBloc(fd, 0, loadBloc(fd, 0));
            break;
        default:
            break;
        }
        interface_tools_update(interface);
    }
}

void interface_level_actions(interface_t *interface, int posX, int posY, int restore)
{
    // ncurses_stop();
    // printf("Detect id: %d, selection %d, couleur %d\n", tab[posY][posX], interface->selection, interface->current_color);

    int draw = 0; // boolean to draw or not the gate/key/... wich have multiple color
    switch (interface->selection)
    {
    case Delete:
        updateEntity(interface, posX, posY, 0);
        break;
    case Block:
        if ((!insertEntityID(posX, posY, 1, 1, ID_BLOCK) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, CYAN, ' ' | A_REVERSE);
        }
        break;
    case Ladder:
        if ((!insertEntityID(posX, posY, 3, 1, ID_LADDER) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, YELLOW, ' ' | ACS_LTEE);
            window_mvaddch_col(interface->win_level, posY, posX + 1, YELLOW, ' ' | ACS_HLINE);
            window_mvaddch_col(interface->win_level, posY, posX + 2, YELLOW, ' ' | ACS_RTEE);
        }
        break;
    case Trap:
        if ((!insertEntityID(posX, posY, 1, 1, ID_TRAP) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, CYAN, '#' | A_REVERSE);
        }
        break;
    case Gate:
        switch (interface->current_color)
        {
        case MAGENTA:
            if ((!insertEntityID(posX, posY, 1, 4, ID_PURPLE_GATE) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        case GREEN:
            if ((!insertEntityID(posX, posY, 1, 4, ID_GREEN_GATE) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        case YELLOW:
            if ((!insertEntityID(posX, posY, 1, 4, ID_YELLOW_GATE) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        case BLUE:
            if ((!insertEntityID(posX, posY, 1, 4, ID_BLUE_GATE) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        default:
            break;
        }
        if (draw)
        {
            for (size_t i = 0; i < 4; i++)
            {
                window_mvaddch_col(interface->win_level, posY + i, posX, interface->current_color, ' ' | ACS_PLUS);
            }
        }
        break;
    case Key:
        switch (interface->current_color)
        {
        case MAGENTA:
            if ((!insertEntityID(posX, posY, 1, 2, ID_PURPLE_KEY) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        case GREEN:
            if ((!insertEntityID(posX, posY, 1, 2, ID_GREEN_KEY) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        case YELLOW:
            if ((!insertEntityID(posX, posY, 1, 2, ID_YELLOW_KEY) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        case BLUE:
            if ((!insertEntityID(posX, posY, 1, 2, ID_BLUE_KEY) && restore == 0) || restore == 1)
            {
                draw = 1;
            }
            break;
        default:
            break;
        }
        if (draw)
        {
            window_mvaddch_col(interface->win_level, posY, posX, interface->current_color, ' ' | A_REVERSE);
            window_mvaddch_col(interface->win_level, posY + 1, posX, interface->current_color, ' ' | ACS_LLCORNER);
        }
        break;
    case Door:
        switch (interface->current_color)
        {
        case MAGENTA:
            switch (nb_door)
            {
            case 1:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR1) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 2:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR2) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 3:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR3) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 4:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR4) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 5:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR5) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 6:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR6) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 7:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR7) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 8:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR8) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 9:
                if ((!insertEntityID(posX, posY, 3, 4, ID_PURPLE_DOOR9) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            default:
                break;
            }
            break;
        case GREEN:
            switch (nb_door)
            {
            case 1:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR1) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 2:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR2) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 3:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR3) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 4:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR4) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 5:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR5) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 6:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR6) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 7:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR7) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 8:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR8) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 9:
                if ((!insertEntityID(posX, posY, 3, 4, ID_GREEN_DOOR9) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            default:
                break;
            }
            break;
        case YELLOW:
            switch (nb_door)
            {
            case 1:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR1) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 2:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR2) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 3:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR3) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 4:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR4) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 5:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR5) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 6:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR6) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 7:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR7) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 8:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR8) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 9:
                if ((!insertEntityID(posX, posY, 3, 4, ID_YELLOW_DOOR9) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            default:
                break;
            }
            break;
        case BLUE:
            switch (nb_door)
            {
            case 1:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR1) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 2:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR2) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 3:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR3) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 4:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR4) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 5:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR5) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 6:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR6) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 7:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR7) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 8:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR8) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            case 9:
                if ((!insertEntityID(posX, posY, 3, 4, ID_BLUE_DOOR9) && restore == 0) || restore == 1)
                {
                    draw = 1;
                }
                break;
            default:
                break;
            }
            break;
        }
        if (draw)
        {
            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    window_mvaddch_col(interface->win_level, posY + j, posX + i, interface->current_color, ' ' | A_REVERSE);
                }
            }
            window_mvaddch(interface->win_level, posY, posX, '0');
            window_mvaddch(interface->win_level, posY, posX + 1, "123456789"[nb_door - 1]);
        }
        break;
    case Exit:
        if ((!insertEntityID(posX, posY, 3, 4, ID_EXIT) && restore == 0) || restore == 1)
        {
            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    window_mvaddch_col(interface->win_level, posY + j, posX + i, YELLOW, ' ' | A_REVERSE);
                }
            }
        }
        break;
    case Start:
        if ((!insertEntityID(posX, posY, 3, 4, ID_START) && restore == 0) || restore == 1)
        {
            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    window_mvaddch_col(interface->win_level, posY + j, posX + i, MAGENTA, ' ' | A_REVERSE);
                }
            }
        }
        break;
    case Robot:
        if ((!insertEntityID(posX, posY, 3, 4, ID_ROBOT) && restore == 0) || restore == 1)
        {
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
        }
        break;
    case Probe:
        if ((!insertEntityID(posX, posY, 2, 3, ID_PROBE) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, WHITE, ' ' | ACS_LTEE);
            window_mvaddch_col(interface->win_level, posY, posX + 1, WHITE, ' ' | ACS_HLINE);
            window_mvaddch_col(interface->win_level, posY, posX + 2, WHITE, ' ' | ACS_RTEE);
            window_mvaddch_col(interface->win_level, posY + 1, posX, WHITE, ' ' | ACS_LLCORNER);
            window_mvaddch_col(interface->win_level, posY + 1, posX + 1, WHITE, ' ' | ACS_HLINE);
            window_mvaddch_col(interface->win_level, posY + 1, posX + 2, WHITE, ' ' | ACS_LRCORNER);
        }
        break;
    case Life:
        if ((!insertEntityID(posX, posY, 1, 1, ID_LIFE) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, RED, 'V');
        }
        break;
    case Bomb:
        if ((!insertEntityID(posX, posY, 1, 1, ID_BOMB) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, WHITE, 'o');
        }
        break;
    default:
        break;
    }
    window_refresh(interface->win_level);
}

void interface_actions(int fd, level_t *level, interface_t *interface, int c)
{
    int mouseX, mouseY, posX, posY;

    // Mouse management
    if ((c == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {
        if (window_getcoordinates(interface->win_tools, mouseX, mouseY, &posX, &posY))
        {
            interface_tools_actions(fd, level, interface, posX, posY);
        }
        else if (window_getcoordinates(interface->win_level, mouseX, mouseY, &posX, &posY))
        {
            interface_level_actions(interface, posX, posY, 0);
        }
        window_mvprintw_col(interface->win_infos, 1, 0, WHITE, "X : %d Y : %d\n", posX, posY);
        window_refresh(interface->win_infos);
    }
}

void outliner(interface_t *interface)
{
    for (int i = 0; i <= 59; i++)
    {
        insertEntityID(i, 0, 1, 1, ID_BLOCK);
        window_mvaddch_col(interface->win_level, 0, i, CYAN, ' ' | A_REVERSE);
        insertEntityID(i, 19, 1, 1, ID_BLOCK);
        window_mvaddch_col(interface->win_level, 19, i, CYAN, ' ' | A_REVERSE);
    }
    for (int i = 0; i <= 19; i++)
    {
        insertEntityID(0, i, 1, 1, ID_BLOCK);
        window_mvaddch_col(interface->win_level, i, 0, CYAN, ' ' | A_REVERSE);
        insertEntityID(59, i, 1, 1, ID_BLOCK);
        window_mvaddch_col(interface->win_level, i, 59, CYAN, ' ' | A_REVERSE);
    }
}

int insertEntityID(int posX, int posY, int entity_WIDTH, int entity_HEIGHT, int entityID)
{
    // check if there is not already an entity
    for (int i = 0; i < entity_HEIGHT; i++)
    {
        for (int j = 0; j < entity_WIDTH; j++)
        {
            if (tab[posY + i][posX + j] != 0)
            {
                return 1;
            }
        }
    }
    for (int i = 0; i < entity_HEIGHT; i++)
    {
        for (int j = 0; j < entity_WIDTH; j++)
        {
            tab[posY + i][posX + j] = entityID;
        }
    }
    return 0;
}

int updateEntity(interface_t *interface, int posX, int posY, int action) // action {0 : delete, 1: restore}
{
    int bloc_width = 0;
    int bloc_height = 0;
    int prev_select = getEntityDetail(interface, tab, posX, posY, &bloc_width, &bloc_height, nb_door);
    if (action == 0)
    {
        interface->selection = prev_select;
    }
    // tmp_pos : position du clic et pos : position de la tête du bloc
    int tmp_posX = posX;
    int tmp_posY = posY;
    if (bloc_width == 0 || bloc_height == 0)
    {
        return -1;
    }
    getHeadEntity(&posX, &posY, bloc_width, bloc_height);

    // ----- debug : entity information -----
    // ncurses_stop();
    // printf("Detect id: %d, posX: %d, posY: %d\n", tab[posY][posX], tmp_posX, tmp_posY);
    // printf("Head (posX, posY): (%d, %d)\n", posX, posY);
    // printf("Detail (bloc_width, bloc_height): (%d, %d)\n", bloc_width, bloc_height);
    // exit(0);

    // Supprimer/Restaurer toutes les cases de l'ID spécifié
    if (action == 1) // restore
    {
        if (tmp_posX == posX && tmp_posY == posY)
        {
            interface_level_actions(interface, posX, posY, 1);
            return bloc_width - 1;
        }
        else
        {
            return -1;
        }
    }
    else // delete
    {
        for (int i = 0; i < bloc_height; i++)
        {
            for (int j = 0; j < bloc_width; j++)
            {
                tab[posY + i][posX + j] = 0;
                window_mvaddch(interface->win_level, posY + i, posX + j, ' ');
            }
        }
    }
    return -1;
}

void getHeadEntity(int *posX, int *posY, int bloc_width, int bloc_height)
{
    // Récupérer l'ID du bloc
    int bloc_id = tab[*posY][*posX];
    // position de la case de départ sélectionnée
    int tmp_x = *posX;
    int tmp_y = *posY;
    int nb_cells = 0;

    // parcours vers le haut -----
    if ((*posY - bloc_height >= 0) && tab[*posY - bloc_height][*posX] == bloc_id)
    {
        // compteurHeight qui compte les cases au dessus tant que c'est le même bloc_id
        int compteurHeight = 0;
        while ((*posY - compteurHeight >= 0) && tab[*posY - compteurHeight][*posX] == bloc_id)
        {
            compteurHeight++;
        }
        nb_cells = (int)((compteurHeight - 1) / bloc_height) * bloc_height;
        // on descend de nb_cells case vers le bas et donc head_y prend la nouvelle valeur y de la position de la case.
        tmp_y = (*posY - compteurHeight + 1) + nb_cells;
        // ncurses_stop();
        // printf("nb_cells : %d, compteur Height : %d, PosY: %d, tmp_y,: %d\n", nb_cells, compteurHeight, posY, tmp_y);
        // exit(0);
    }
    else
    {
        while (tmp_y >= 0 && tab[tmp_y][tmp_x] == bloc_id)
        {
            tmp_y--;
        }
        tmp_y++;
    }

    // parcours vers la gauche
    if ((*posX - bloc_width >= 0) && tab[tmp_y][*posX - bloc_width] == bloc_id)
    {
        // compteurHeight qui compte les cases au dessus tant que c'est le même bloc_id
        int compteurWidth = 0;
        while ((*posX - compteurWidth >= 0) && tab[*posY][*posX - compteurWidth] == bloc_id)
        {
            compteurWidth++;
        }
        nb_cells = (int)((compteurWidth - 1) / bloc_width) * bloc_width;
        // on descend de nb_cells case vers le bas et donc head_y prend la nouvelle valeur y de la position de la case.
        tmp_x = (*posX - compteurWidth + 1) + nb_cells;
    }
    else
    {
        while (tmp_x >= 0 && tab[tmp_y][tmp_x] == bloc_id)
        {
            tmp_x--;
        }
        tmp_x++;
    }
    *posX = tmp_x;
    *posY = tmp_y;
}

void clearMapID()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            tab[i][j] = 0;
        }
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        tab[i][0] = 1;
        tab[i][WIDTH - 1] = 1;
    }
    for (int i = 0; i < WIDTH; i++)
    {
        tab[0][i] = 1;
        tab[HEIGHT - 1][i] = 1;
    }
}
void clearInerface(interface_t *interface)
{
    window_erase(interface->win_level);
    clearMapID();
    outliner(interface);
    window_refresh(interface->win_level);
}
void displayMapID() // for debugging
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (tab[i][j] >= 10)
            {
                printf("%d", tab[i][j]);
            }
            else
            {
                printf("%d ", tab[i][j]);
            }
        }
        printf("\n");
    }
}
