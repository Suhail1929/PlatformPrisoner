#include <stdlib.h>
#include <ncurses.h>
#include <limits.h>

#include "fonction.h"
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "item.h"
#include "liste.h"
#include "interface.h"
#include "entity.h"

int nb_door = 1;
char level[15];
int level_nb = 0;
int tab[HEIGHT][WIDTH];
int compteurEntity = 0;

/*
 * Function that create an interface
 * @param level : the level
 */
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

    update_win_level(result, level);

    window_refresh(result->win_level);
    // fenetre tools
    result->win_tools = window_create(62, 0, 15, 22, "Tools", FALSE);

    result->current_color = MAGENTA; // default color
    result->selection = ID_BLOCK;    // default selection
    interface_tools_update(result);
    window_refresh(result->win_tools);

    return result;
}

/**
 * @brief Update the map table with the current level
 * @param interface : the interface
 * @param level : the level
 */
void update_win_level(interface_t *interface, level_t *level)
{
    int i, j, tmp_decalage = 0;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
            tab[i][j] = level->tab[i][j];
    }

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            tmp_decalage = updateEntity(interface, j, i, 1);
            if (tmp_decalage != -1)
                j += tmp_decalage;
        }
    }
}

/*
 * Function that update the tools window ###
 * @param interface : the interface
 */
void interface_delete(interface_t **interface)
{
    int h, w;
    window_delete(&(*interface)->win_infos);
    window_delete(&(*interface)->win_level);
    window_delete(&(*interface)->win_tools);
    for (h = 0; h < HEIGHT; h++)
    {
        for (w = 0; w < WIDTH; w++)
        {
            detruire_liste(&(*interface)->tab_item[h][w]);
        }
    }
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

    if (interface->selection == ID_DELETE)
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

    if (interface->selection == ID_BLOCK)
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

    if (interface->selection == ID_LADDER)
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

    if (interface->selection == ID_TRAP)
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

    if (interface->selection == ID_GATE)
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

    if (interface->selection == ID_KEY)
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

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_DOOR) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_DOOR) ? WHITE : RED);

    window_printw(interface->win_tools, " Door  ");
    if (nb_door < 10)
    {
        window_printw_col(interface->win_tools, WHITE, "<0%d>\n", nb_door);
    }
    else if (nb_door < 100)
    {
        window_printw_col(interface->win_tools, WHITE, "<%d>\n", nb_door);
    }

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_EXIT) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_EXIT) ? WHITE : RED);

    window_printw(interface->win_tools, " Exit\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_START) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_START) ? WHITE : RED);

    window_printw(interface->win_tools, " Start\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_ROBOT) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_ROBOT) ? WHITE : RED);

    window_printw(interface->win_tools, " Robot\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_PROBE) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_PROBE) ? WHITE : RED);

    window_printw(interface->win_tools, " Probe\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_LIFE) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_LIFE) ? WHITE : RED);

    window_printw(interface->win_tools, " Life\n");

    window_printw_col(interface->win_tools, WHITE, (interface->selection == ID_BOMB) ? ">" : " ");
    window_color(interface->win_tools, (interface->selection == ID_BOMB) ? WHITE : RED);

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
 * Manage actions in the palette window. ###
 * @param[in,out] interface the interface
 * @param[in] posX X position of the click in the window
 * @param[in] posY Y position of the click in the window
 */
void interface_tools_actions(int fd, level_t *level, interface_t *interface, int posX, int posY)
{
    if ((posY >= 0) && (posY <= 20))
    {
        // interface->selection = posY; fonctionne pas car les id ont été modifiés
        switch (posY)
        {
        case 0:
            interface->selection = ID_DELETE;
            break;
        case 1:
            interface->selection = ID_BLOCK;
            break;
        case 2:
            interface->selection = ID_LADDER;
            break;
        case 3:
            interface->selection = ID_TRAP;
            break;
        case 4:
            if (posX < 6)
                interface->selection = ID_GATE;
            break;
        case 5:
            interface->selection = ID_KEY;
            break;
        case 6:
            interface->selection = ID_DOOR;
            break;
        case 7:
            interface->selection = ID_EXIT;
            break;
        case 8:
            interface->selection = ID_START;
            break;
        case 9:
            interface->selection = ID_ROBOT;
            break;
        case 10:
            interface->selection = ID_PROBE;
            break;
        case 11:
            interface->selection = ID_LIFE;
            break;
        case 12:
            interface->selection = ID_BOMB;
            break;
        case 15:
            interface->selection = LEVEL;
            break;
        case 17:
            interface->selection = CLEAR;
            break;
        case 19:
            interface->selection = SAVE;
            break;
        default:
            break;
        }
        if (posY == 4) // ID_GATE
        {
            if (posX >= 8 && posX <= 11)
                interface->current_color = posX - 7;
        }
        else if (posY == 6) // ID_DOOR
        {
            if (posX == 8 && nb_door > 1)
                nb_door--;
            else if (posX == 11 && nb_door < 99)
                nb_door++;
        }
        else if (posY == 15) // LEVEL
        {
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
                    clearInterface(interface);
                    level = initLevel();
                    level->id = level_nb;
                    update_win_level(interface, level);
                }
                else
                {
                    clearInterface(interface);
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
                    clearInterface(interface);
                    level = initLevel();
                    level->id = level_nb;
                    update_win_level(interface, level);
                }
                else
                {
                    clearInterface(interface);
                    update_win_level(interface, level);
                }
            }
        }
        else if (posY == 17) // CLEAR
        {
            clearInterface(interface);
        }
        else if (posY == 19) // SAVE
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
            updateBloc(fd, 0, loadBloc(fd, 0));
        }
        interface_tools_update(interface);
    }
}

/*
 * Function that implement the actions of the interface in level
 * @param interface : the interface
 * @param posX : the position X
 * @param posY : the position Y
 * @param restore : the restore
 */
void interface_level_actions(interface_t *interface, int posX, int posY, int restore)
{
    char charID[5];
    int integerID;
    switch (interface->selection)
    {
    case ID_DELETE:
        updateEntity(interface, posX, posY, 0);
        break;
    case ID_BLOCK:
        if ((!insertEntityID(posX, posY, 1, 1, ID_BLOCK) && restore == 0) || restore == 1)
            window_mvaddch_col(interface->win_level, posY, posX, CYAN, ' ' | A_REVERSE);
        break;
    case ID_LADDER:
        if ((!insertEntityID(posX, posY, 3, 1, ID_LADDER) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, YELLOW, ' ' | ACS_LTEE);
            window_mvaddch_col(interface->win_level, posY, posX + 1, YELLOW, ' ' | ACS_HLINE);
            window_mvaddch_col(interface->win_level, posY, posX + 2, YELLOW, ' ' | ACS_RTEE);
        }
        break;
    case ID_TRAP:
        if ((!insertEntityID(posX, posY, 1, 1, ID_TRAP) && restore == 0) || restore == 1)
            window_mvaddch_col(interface->win_level, posY, posX, CYAN, '#' | A_REVERSE);
        break;
    case 10 ... 14: // ID_GATE
        if (!restore)
            sprintf(charID, "%d%d", interface->selection / 10, interface->current_color);
        integerID = atoi(charID);
        // if ((!insertEntityID(posX, posY, 1, 4, integerID) && restore == 0) || restore == 1)
        if ((!insertEntityID(posX, posY, 1, 1, integerID) && restore == 0) || restore == 1)
        {
            // for (int i = 0; i < 4; i++)
            //     window_mvaddch_col(interface->win_level, posY + i, posX, interface->current_color, ' ' | ACS_PLUS);
            window_mvaddch_col(interface->win_level, posY, posX, interface->current_color, ' ' | ACS_PLUS);
        }
        break;
    case 20 ... 24: // ID_KEY
        if (!restore)
            sprintf(charID, "%d%d", interface->selection / 10, interface->current_color);
        integerID = atoi(charID);
        // interface->selection += interface->current_color; // mauvaise idée
        if ((!insertEntityID(posX, posY, 1, 2, integerID) && restore == 0) || restore == 1)
        {
            window_mvaddch_col(interface->win_level, posY, posX, interface->current_color, ' ' | A_REVERSE);
            window_mvaddch_col(interface->win_level, posY + 1, posX, interface->current_color, ' ' | ACS_LLCORNER);
        }
        break;
    case 3000 ... 3499: // ID_DOOR
        if (!restore)
        {
            if (nb_door < 10)
                sprintf(charID, "%d%d0%d", interface->selection / 1000, interface->current_color, nb_door);
            else if (nb_door >= 10 && nb_door < 100)
                sprintf(charID, "%d%d%d", interface->selection / 1000, interface->current_color, nb_door);
        }
        integerID = atoi(charID);
        if ((!insertEntityID(posX, posY, 3, 4, integerID) && restore == 0) || restore == 1)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 4; j++)
                    window_mvaddch_col(interface->win_level, posY + j, posX + i, interface->current_color, ' ' | A_REVERSE);
            sprintf(charID, "%d", nb_door / 10);
            window_mvaddch(interface->win_level, posY, posX, charID[0]);
            sprintf(charID, "%d", nb_door % 10);
            window_mvaddch(interface->win_level, posY, posX + 1, charID[0]);
        }
        break;
    case ID_EXIT:
        if ((!insertEntityID(posX, posY, 3, 4, ID_EXIT) && restore == 0) || restore == 1)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 4; j++)
                    window_mvaddch_col(interface->win_level, posY + j, posX + i, YELLOW, ' ' | A_REVERSE);
        }
        break;
    case ID_START:
        if ((!insertEntityID(posX, posY, 3, 4, ID_START) && restore == 0) || restore == 1)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 4; j++)
                    window_mvaddch_col(interface->win_level, posY + j, posX + i, MAGENTA, ' ' | A_REVERSE);
        }
        break;
    case ID_ROBOT:
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
    case ID_PROBE:
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
    case ID_LIFE:
        if ((!insertEntityID(posX, posY, 1, 1, ID_LIFE) && restore == 0) || restore == 1)
            window_mvaddch_col(interface->win_level, posY, posX, RED, 'V');
        break;
    case ID_BOMB:
        if ((!insertEntityID(posX, posY, 1, 1, ID_BOMB) && restore == 0) || restore == 1)
            window_mvaddch_col(interface->win_level, posY, posX, WHITE, 'o');
        break;
    default:
        break;
    }
    window_refresh(interface->win_level);
}

/*
 * Function that implement the actions of the interface
 * @param interface : the interface
 * @param c : the key pressed
 * @param fd : the file descriptor
 * @param level : the level
 */
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
            window_mvprintw_col(interface->win_infos, 1, 0, WHITE, "X : %d Y : %d\n", posX, posY);
        }
        interface_debug(interface, mouseX, mouseY);
        window_refresh(interface->win_infos);
    }
}

/*
 * Function that draw the outliner
 * @param interface : the interface
 */
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

/*
 * Function insert an entity ID in the map and return 0 if success or 1 if error
 * @param posX : the position X
 * @param posY : the position Y
 * @param largeur : the width
 * @param hauteur : the height
 * @param entityID : the entity ID
 */
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

/**
 * @brief Function update an entity ID in the map and return a width to jump or -1 if error
 * @param posX : the position X
 * @param posY : the position Y
 * @param action : the action if == 0 delete the entity else update the entity
 */
int updateEntity(interface_t *interface, int posX, int posY, int action) // action {0 : delete, 1: restore}
{
    int bloc_width = 0;
    int bloc_height = 0;
    int prev_select = getEntityDetail(interface, tab, posX, posY, &bloc_width, &bloc_height, &nb_door);
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
    getHeadEntity(tab, &posX, &posY, bloc_width, bloc_height);

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

/*
 * Function that return the head of the entity
 * @param posX : the position X
 * @param posY : the position Y
 * @param largeur : the width
 * @param hauteur : the height
 */
void getHeadEntity(int tab_lvl[20][60], int *posX, int *posY, int bloc_width, int bloc_height)
{
    // Récupérer l'ID du bloc
    int bloc_id = tab_lvl[*posY][*posX];
    // position de la case de départ sélectionnée
    int tmp_x = *posX;
    int tmp_y = *posY;
    int nb_cells = 0;

    // parcours vers le haut -----
    if ((*posY - bloc_height >= 0) && tab_lvl[*posY - bloc_height][*posX] == bloc_id)
    {
        // compteurHeight qui compte les cases au dessus tant que c'est le même bloc_id
        int compteurHeight = 0;
        while ((*posY - compteurHeight >= 0) && tab_lvl[*posY - compteurHeight][*posX] == bloc_id)
        {
            compteurHeight++;
        }
        nb_cells = (int)((compteurHeight - 1) / bloc_height) * bloc_height;
        // on descend de nb_cells case vers le bas et donc head_y prend la nouvelle valeur y de la position de la case.
        tmp_y = (*posY - compteurHeight + 1) + nb_cells;
    }
    else
    {
        while (tmp_y >= 0 && tab_lvl[tmp_y][tmp_x] == bloc_id)
        {
            tmp_y--;
        }
        tmp_y++;
    }
    // parcours vers la gauche
    if ((*posX - bloc_width >= 0) && tab_lvl[tmp_y][*posX - bloc_width] == bloc_id)
    {
        // compteurHeight qui compte les cases au dessus tant que c'est le même bloc_id
        int compteurWidth = 0;
        while ((*posX - compteurWidth >= 0) && tab_lvl[*posY][*posX - compteurWidth] == bloc_id)
        {
            compteurWidth++;
        }
        nb_cells = (int)((compteurWidth - 1) / bloc_width) * bloc_width;
        // on descend de nb_cells case vers le bas et donc head_y prend la nouvelle valeur y de la position de la case.
        tmp_x = (*posX - compteurWidth + 1) + nb_cells;
    }
    else
    {
        while (tmp_x >= 0 && tab_lvl[tmp_y][tmp_x] == bloc_id)
        {
            tmp_x--;
        }
        tmp_x++;
    }
    *posX = tmp_x;
    *posY = tmp_y;
}

/*
 *Function that clear the map ID
 */
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

/*
 *Function that clear the interface
 */
void clearInterface(interface_t *interface)
{
    window_erase(interface->win_level);
    clearMapID();
    outliner(interface);
    window_refresh(interface->win_level);
}

/*
 *Function that display the map ID
 */
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

/**
 * @brief Function that display the interface id for debugging
 */
void interface_debug(interface_t *interface, int posX, int posY)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (tab[i][j] == ID_DELETE)
            {
                mvprintw(i + 1, j + 80, "%d", tab[i][j]);
            }
            else
            {
                int entier = tab[i][j];
                while (entier >= 10)
                {
                    entier /= 10;
                }
                attron(COLOR_PAIR(MAGENTA));
                mvprintw(i + 1, j + 80, "%d", entier);
                attroff(COLOR_PAIR(MAGENTA));
            }
        }
    }
    if (posX >= 80 && posX < 140 && posY >= 1 && posY <= 20)
    {
        if (interface->tab_item[posY - 1][posX - 80].tete == NULL)
        {
            mvprintw(23, 80, "dernier id ajouté : %d      ", interface->tab_item[posY - 1][posX - 80].tete->item->id);
        }
        else
        {
            mvprintw(23, 80, "dernier id ajouté : %d      ", tab[posY - 1][posX - 80]);
        }
        mvprintw(24, 80, "Position x: %d, y: %d      ", posX - 80, posY - 1);
        afficher_liste(interface->tab_item[posY - 1][posX - 80]);
    }
}
