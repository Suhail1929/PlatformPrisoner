#include <ncurses.h>
#include <stdlib.h>
#include "Fonction/fonction.h"
#include "Couleur/couleur.h"
#include "Window/window.h"
#include "Level/niveau.h"
#include "Interface/interface.h"

int main()
{
    interface_t *interface;
    niveau_t *niveau = niveau_init();
    niveau->nom = "First level";
    niveau->description = "This is the first level of the game";
    ncurses_init();
    ncurses_colors();
    palette();
    ncurses_init_mouse();

    wbkgd(stdscr, COLOR_PAIR(0));

    refresh();

    // Creation de l'interface
    interface = interface_create(niveau);

    int ch;
    while ((ch = getch()) != 27)
    {
        interface_actions(interface, niveau, ch);
    }
    // Quitter le mode d'affichage ncurses
    ncurses_stop();

    // supprimer l'interface
    interface_delete(&interface);
    display_niveau(niveau);
    return 0;
}
