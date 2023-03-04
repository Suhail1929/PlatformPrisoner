#include "fonction.h"
#include <ncurses.h>
#include <stdlib.h>

void ncurses_init()
{
    initscr();            // Initialise ncurses
    cbreak();             // Pour les saisies clavier (desac. mise en buffer)
    noecho();             // Desactive l'affichage des caracteres saisis au clavier
    keypad(stdscr, TRUE); // Active les touches specifiques (F1, Flèches, etc.)
    refresh();            // Affiche l'ecran
    curs_set(FALSE);      // Cache le curseur
}

void ncurses_stop()
{
    endwin();
}

/**
 * initialise les couleurs
 */
void ncurses_colors()
{
    // verifie si le terminal supporte les couleurs
    if (has_colors() == FALSE)
    {
        ncurses_stop();
        fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
        exit(EXIT_FAILURE);
    }

    // active les couleurs
    start_color();
}

/**
 * initialise la souris
 */
void ncurses_init_mouse()
{
    if (!mousemask(BUTTON1_PRESSED, NULL))
    {
        ncurses_stop();
        fprintf(stderr, "Mouse isn't supported.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * recupere la position de la souris
 */
int mouse_getpos(int *x, int *y)
{
    MEVENT event;
    int result = getmouse(&event);
    if (result == OK)
    {
        *x = event.x;
        *y = event.y;
    }
    return result;
}
