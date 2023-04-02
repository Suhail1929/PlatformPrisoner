#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>

#include "couleur.h"
#include "window.h"
#include "item.h"
#include "liste.h"
#include "data_table.h"
#include "interface.h"

item_t *init_item(unsigned int id, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    item_t *item = (item_t *)malloc(sizeof(item_t));
    item->id = id;
    item->x = x;
    item->y = y;
    item->width = width;
    item->height = height;

    // ajout des propriétés
    switch (id)
    {
    case 4:             // ID_TRAP
    case 6:             // ID_PROBE
    case 7:             // ID_ROBOT
    case 70:            // ID_EXPLOSION
    case 50:            // ID_ACTIVE_BOMB
    case 2:             // ID_LIFE
    case 3:             // ID_BOMB
        item->etat = 1; // actif
        break;
    case 3101 ... 3499: // ID_DOOR(3) + color(1-4) + nb_door(01-99)
        item->properties.door.nb_door = id % 100;
        item->properties.door.color.color = (id / 100) % 10;
        break;
    case 11 ... 14: // ID_GATE
    case 21 ... 24: // ID_KEY
        item->properties.color.color = id % 10;
        break;
    case 41 ... 44:     // ID_PLAYER
        item->etat = 1; // actif
        item->properties.player.nb_key = 0;
        item->properties.player.nb_life = 5;
        item->properties.player.nb_bomb = 3;
        item->properties.player.color.color = id % 10;
        for (int i = 0; i < 4; i++)
        {
            item->properties.player.key[i].color = 0;
        }
        break;
    default:
        break;
    }
    return item;
}

void display_item(window_t *window, item_t item, int posX, int posY)
{
    char charID[5];
    switch (item.id)
    {
    case 11 ... 14: // ID_GATE (en 1x1)
        // for (int i = 0; i < 4; i++) // for gate (1x4)
        // window_mvaddch_col(window, posY + i, posX, item.properties.color.color, ' ' | ACS_PLUS);
        window_mvaddch_col(window, posY, posX, item.properties.color.color, ' ' | ACS_PLUS);
        break;
    case ID_BLOCK:
        window_mvaddch_col(window, posY, posX, CYAN, ' ' | A_REVERSE);
        break;
    case ID_LIFE:
        if (item.etat == 1)
            window_mvaddch_col(window, posY, posX, RED, 'V');
        else
            window_mvaddch_col(window, posY, posX, BLACK, ' ');
        break;
    case ID_BOMB:
        if (item.etat == 1)
            window_mvaddch_col(window, posY, posX, WHITE, 'o');
        else
            window_mvaddch_col(window, posY, posX, BLACK, ' ');
        break;
    case ID_TRAP:
        if (item.etat == 1)
            window_mvaddch_col(window, posY, posX, CYAN, '#' | A_REVERSE);
        else
            window_mvaddch_col(window, posY, posX, BLACK, ' ');
        break;
    case 5: // ID_LADDER
        window_mvaddch_col(window, posY, posX, YELLOW, ' ' | ACS_LTEE);
        window_mvaddch_col(window, posY, posX + 1, YELLOW, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY, posX + 2, YELLOW, ' ' | ACS_RTEE);
        break;
    case 6: // ID_PROBE
        window_mvaddch_col(window, posY, posX, WHITE, ' ' | ACS_LTEE);
        window_mvaddch_col(window, posY, posX + 1, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY, posX + 2, WHITE, ' ' | ACS_RTEE);
        window_mvaddch_col(window, posY + 1, posX, WHITE, ' ' | ACS_LLCORNER);
        window_mvaddch_col(window, posY + 1, posX + 1, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY + 1, posX + 2, WHITE, ' ' | ACS_LRCORNER);
        break;
    case 3101 ... 3499: // ID_DOOR(3) + color(1-4) + nb_door(01-99)
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                window_mvaddch_col(window, posY + j, posX + i, item.properties.door.color.color, ' ' | A_REVERSE);
        sprintf(charID, "%d", item.properties.door.nb_door / 10);
        window_mvaddch(window, posY, posX, charID[0]);
        sprintf(charID, "%d", item.properties.door.nb_door % 10);
        window_mvaddch(window, posY, posX + 1, charID[0]);
        break;
    case 41 ... 44: // ID_PLAYER(4) + color(1-4)
        if (item.etat == 1)
        {
            window_mvaddch_col(window, posY, posX, item.properties.player.color.color, ' ' | A_REVERSE);
            window_mvaddch_col(window, posY, posX + 1, item.properties.player.color.color, '-' | A_REVERSE);
        }
        else
        {
            window_mvaddch_col(window, posY, posX + 1, item.properties.player.color.color, ' ' | A_REVERSE);
            window_mvaddch_col(window, posY, posX, item.properties.player.color.color, '-' | A_REVERSE);
        }
        window_mvaddch_col(window, posY + 1, posX, item.properties.player.color.color, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY + 1, posX + 1, item.properties.player.color.color, ' ' | ACS_PLUS);
        window_mvaddch_col(window, posY + 1, posX + 2, item.properties.player.color.color, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY + 2, posX, item.properties.player.color.color, ' ' | ACS_ULCORNER);
        window_mvaddch_col(window, posY + 2, posX + 1, item.properties.player.color.color, ' ' | ACS_BTEE);
        window_mvaddch_col(window, posY + 2, posX + 2, item.properties.player.color.color, ' ' | ACS_URCORNER);
        window_mvaddch_col(window, posY + 3, posX, item.properties.player.color.color, ' ' | ACS_VLINE);
        window_mvaddch_col(window, posY + 3, posX + 2, item.properties.player.color.color, ' ' | ACS_VLINE);
        break;
    case ID_ROBOT:
        window_mvaddch_col(window, posY, posX, WHITE, ' ' | ACS_ULCORNER);
        window_mvaddch_col(window, posY, posX + 1, WHITE, ' ' | ACS_BTEE);
        window_mvaddch_col(window, posY, posX + 2, WHITE, ' ' | ACS_URCORNER);
        window_mvaddch_col(window, posY + 1, posX, WHITE, ' ' | ACS_LLCORNER);
        window_mvaddch_col(window, posY + 1, posX + 1, WHITE, ' ' | ACS_TTEE);
        window_mvaddch_col(window, posY + 1, posX + 2, WHITE, ' ' | ACS_LRCORNER);
        window_mvaddch_col(window, posY + 2, posX, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY + 2, posX + 1, WHITE, ' ' | ACS_PLUS);
        window_mvaddch_col(window, posY + 2, posX + 2, WHITE, ' ' | ACS_HLINE);
        window_mvaddch_col(window, posY + 3, posX, WHITE, ' ' | ACS_ULCORNER);
        window_mvaddch_col(window, posY + 3, posX + 1, WHITE, ' ' | ACS_BTEE);
        window_mvaddch_col(window, posY + 3, posX + 2, WHITE, ' ' | ACS_URCORNER);
        break;
    case ID_EXIT:
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                window_mvaddch_col(window, posY + j, posX + i, YELLOW, ' ' | A_REVERSE);
        break;
    case ID_START:
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                window_mvaddch_col(window, posY + j, posX + i, MAGENTA, ' ' | A_REVERSE);
        break;
    case 21 ... 24: // ID_KEY
        window_mvaddch_col(window, posY, posX, item.properties.color.color, ' ' | A_REVERSE);
        window_mvaddch_col(window, posY + 1, posX, item.properties.color.color, ' ' | ACS_LLCORNER);
        break;
    case ID_ACTIVE_BOMB:
        window_mvaddch_col(window, posY, posX, RED, '@');
        break;
    case ID_EXPLOSION:
        window_mvaddch_col(window, posY, posX, WHITE, '#');
        break;
    default: // ID_DELETE
        window_mvaddch_col(window, posY, posX, BLACK, ' ');
        break;
    }
    // window_refresh(window); // 11
}