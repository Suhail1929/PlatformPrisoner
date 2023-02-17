#include <ncurses.h>
#include <stdlib.h>
#include "../Fonction/fonction.h"
#include "../Couleur/couleur.h"
#include "../Window/window.h"
#include "Block.h"
Block_t *initBlock(int largeur, int hauteur)
{
    Block_t *block = malloc(sizeof(Block_t));
    block->largeur = largeur;
    block->hauteur = hauteur;
    block->couleur = CYAN;
    return block;
}

void drawBlock(window_t *window, int x, int y, int color)
{
    window_mvaddch_col(window, y, x, color, ' ' | A_REVERSE);
}
