#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "fonction.h"
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "interface.h"
#include "entity.h"

#include <limits.h>

#define MENU_HEIGHT 27
#define MENU_WIDTH 77

interface_t *interface_create_game()
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

    window_refresh(result->win_level);
    // fenetre tools
    result->win_hud = window_create(62, 0, 15, 22, "HUD", FALSE); // HUD : Heads Up Display

    result->current_color = MAGENTA; // default color
    result->selection = ID_BLOCK;    // default selection
    interface_hud_update(result);
    window_refresh(result->win_hud);

    return result;
}

void interface_hud_actions(interface_t *interface, int c)
{
    int mouseX, mouseY, posX, posY;
    // Mouse management
    if ((c == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {
        if (window_getcoordinates(interface->win_hud, mouseX, mouseY, &posX, &posY))
        {
            interface_hud_update(interface);
            window_refresh(interface->win_hud);
        }
    }
}

void interface_hud_update(interface_t *interface)
{
    window_erase(interface->win_hud);
    window_mvprintw(interface->win_hud, 1, 1, "Key");

    // for (int j = 0; j < player1.carac.player.nb_vie * 2; j += 2)
    // {
    //     objet key;
    //     init_objet(&key, ID_KEY, 2 + j, 2);
    //     key.carac.key.couleur = RED;
    //     affichage_objet(key, key.y, key.x, interface->win_hud);
    // }

    window_mvprintw(interface->win_hud, 5, 1, "Lives");

    // for (int i = 0; i < player1.carac.player.nb_vie; i++)
    // {
    //     objet life;
    //     init_objet(&life, ID_LIFE, 2 + i, 7);
    //     affichage_objet(life, life.y, life.x, interface->win_hud);
    // }

    window_mvprintw(interface->win_hud, 10, 1, "Bombs");

    // for (int i = 0; i < player1.carac.player.nb_bomb; i++)
    // {
    //     objet bomb;
    //     init_objet(&bomb, ID_BOMB, 2 + i, 11);
    //     affichage_objet(bomb, bomb.y, bomb.x, interface->win_tools);
    // }

    window_mvprintw(interface->win_hud, 15, 1, "Levels");
    // window_printw(interface->win_hud, " \n  %02d \n", nb_level);

    window_refresh(interface->win_hud);
}

void interface_delete_game(interface_t **interface)
{
    window_delete(&(*interface)->win_infos);
    window_delete(&(*interface)->win_level);
    window_delete(&(*interface)->win_hud);

    free(*interface);
    interface = NULL;
}
