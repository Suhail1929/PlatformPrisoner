#include "monstre.h"
#include <ncurses.h>
#include <stdlib.h>

void init_monstre(Monstre *monstre)
{
    // caractères : [ ACS_ULCORNER, ACS_BTEE, ACS_URCORNER ], [ ACS_LLCORNER, ACS_TTEE, ACS_LRCORNER ], [ACS_HLINE, ACS_PLUS, ACS_HLINE ], [ACS_ULCORNER, ACS_BTEE, ACS_URCORNER ]
    monstre->sprite[0][0] = ACS_ULCORNER;
    monstre->sprite[0][1] = ACS_BTEE;
    monstre->sprite[0][2] = ACS_URCORNER;
    monstre->sprite[1][0] = ACS_LLCORNER;
    monstre->sprite[1][1] = ACS_TTEE;
    monstre->sprite[1][2] = ACS_LRCORNER;
    monstre->sprite[2][0] = ACS_HLINE;
    monstre->sprite[2][1] = ACS_PLUS;
    monstre->sprite[2][2] = ACS_HLINE;
    monstre->sprite[3][0] = ACS_ULCORNER;
    monstre->sprite[3][1] = ACS_BTEE;
    monstre->sprite[3][2] = ACS_URCORNER;
}
