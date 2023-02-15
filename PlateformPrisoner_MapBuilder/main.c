#include <ncurses.h>
#include <stdlib.h>
#include "Fonction/fonction.h"
#include "Couleur/couleur.h"
#include "Window/window.h"
#include "Interface/interface.h"

int main()
{
    interface_t *interface;

    ncurses_init();
    ncurses_colors();
    palette();
    ncurses_init_mouse();

    wbkgd(stdscr, COLOR_PAIR(0));

    refresh();

    // Creation de l'interface
    interface = interface_create();

    int ch;
    while ((ch = getch()) != 27)
    {
        interface_actions(interface, ch);
    }
    // Quitter le mode d'affichage ncurses
    ncurses_stop();

    // supprimer l'interface
    interface_delete(&interface);
    return 0;
}
