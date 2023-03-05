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

interface_t *interface_create_menu()
{
    interface_t *result;

    // Structure allocation
    if ((result = malloc(sizeof(interface_t))) == NULL)
    {
        ncurses_stop();
        perror("Erreur allocation interface");
        exit(EXIT_FAILURE);
    }

    // fenetre Menu
    result->win_menu = window_create(0, 0, MENU_WIDTH, MENU_HEIGHT, "Menu", FALSE);
    window_color(result->win_menu, RED);

    window_mvprintw(result->win_menu, MENU_HEIGHT / 2, (MENU_WIDTH / 2) - strlen("Créer une partie") / 2, "Créer une partie");
    window_mvprintw(result->win_menu, MENU_HEIGHT / 2 + 1, (MENU_WIDTH / 2) - strlen("S'inscrire à une partie") / 2, "S'inscrire à une partie");
    window_refresh(result->win_menu);

    result->selection = MENU;

    return result;
}

void interface_menu_actions(interface_t *interface, int ch)
{
    int mouseX, mouseY, posX, posY;
    // Mouse management
    if ((ch == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {
        if (window_getcoordinates(interface->win_menu, mouseX, mouseY, &posX, &posY))
        {
            interface_menu_update(interface, posX, posY);
            window_refresh(interface->win_menu);
        }
    }
}

void interface_menu_update(interface_t *interface, int posX, int posY)
{
    window_erase(interface->win_menu);

    if (interface->selection == MENU)
    {
        if ((posY >= MENU_HEIGHT / 2) && (posY <= MENU_HEIGHT / 2 + 1))
        {
            switch (posY)
            {
            case MENU_HEIGHT / 2:
                interface->selection = CREER_PARTIE;
                break;
            case MENU_HEIGHT / 2 + 1:
                interface->selection = REJOINDRE_PARTIE;
                break;
            }
        }
        if (interface->selection == CREER_PARTIE)
        {
            window_mvprintw_col(interface->win_menu, MENU_HEIGHT / 2, (MENU_WIDTH / 2) - strlen("Créer une partie") / 2 - 2, WHITE, ">");
            window_color(interface->win_menu, WHITE);
        }
        else
            window_color(interface->win_menu, RED);

        window_mvprintw(interface->win_menu, MENU_HEIGHT / 2, (MENU_WIDTH / 2) - strlen("Créer une partie") / 2, "Créer une partie");

        if (interface->selection == REJOINDRE_PARTIE)
        {
            window_mvprintw_col(interface->win_menu, MENU_HEIGHT / 2 + 1, (MENU_WIDTH / 2) - strlen("S'inscrire à une partie") / 2 - 2, WHITE, ">");
            window_color(interface->win_menu, WHITE);
        }
        else
            window_color(interface->win_menu, RED);

        window_mvprintw(interface->win_menu, MENU_HEIGHT / 2 + 1, (MENU_WIDTH / 2) - strlen("S'inscrire à une partie") / 2, "S'inscrire à une partie");
    }
    else if (interface->selection == CREER_PARTIE)
    {
        window_mvprintw(interface->win_menu, MENU_HEIGHT / 2 + 1, (MENU_WIDTH / 2) - strlen("BONJOURRRRRR") / 2, "BONJOURRRRRR");
    }
    else if (interface->selection == REJOINDRE_PARTIE)
    {
    }

    window_refresh(interface->win_menu);
}

void interface_delete_menu(interface_t **interface)
{
    window_delete(&(*interface)->win_menu);

    free(*interface);
    interface = NULL;
}

interface_t *interface_create_client()
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
    window_printw_col(result->win_infos, RED, "Appuyez sur 'ECHAP' pour quitter ");
    window_refresh(result->win_infos);

    // fenetre coordonnées
    result->win_level = window_create(0, 0, 62, 22, "Level", FALSE);
    window_refresh(result->win_level);

    // fenetre tools
    result->win_tools = window_create(62, 0, 15, 22, "Tools", FALSE);
    window_refresh(result->win_tools);

    // interface_affiche(result);

    // window_refresh(result->win_level);
    // window_refresh(result->win_infos);

    return result;
}