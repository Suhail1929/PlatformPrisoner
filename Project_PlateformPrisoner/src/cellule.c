#include <stdlib.h>
#include <ncurses.h>

#include "window.h"
#include "item.h"
#include "liste.h"
#include "cellule.h"

cellule *init_cellule(item_t *item)
{
    cellule *c = (cellule *)malloc(sizeof(cellule));
    c->item = item;
    c->succ = NULL;
    c->pred = NULL;
    return c;
}