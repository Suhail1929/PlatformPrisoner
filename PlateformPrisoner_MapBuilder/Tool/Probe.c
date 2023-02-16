#include "Probe.h"

Probe_t *initProbe(int largeur, int hauteur)
{
    Probe_t *probe = malloc(sizeof(Probe_t));
    probe->largeur = largeur;
    probe->hauteur = hauteur;
    return probe;
}

void drawProbe(window_t *window, int x, int y, int color)
{
    window_mvaddch_col(window, y, x, color, ' ' | ACS_LTEE);
    window_mvaddch_col(window, y, x + 1, color, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y, x + 2, color, ' ' | ACS_RTEE);
    window_mvaddch_col(window, y + 1, x, color, ' ' | ACS_LLCORNER);
    window_mvaddch_col(window, y + 1, x + 1, color, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y + 1, x + 2, color, ' ' | ACS_LRCORNER);
}