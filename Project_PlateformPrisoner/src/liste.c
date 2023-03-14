#include <stdlib.h>
#include <ncurses.h>

#include "window.h"
#include "item.h"
#include "cellule.h"
#include "liste.h"

void initialiser_liste(liste *l)
{
    l->tete = NULL;
}

void inserer(liste *l, cellule *c)
{
    if (l->tete == NULL)
    {
        c->succ = NULL;
    }
    else
    {
        c->succ = l->tete;
        l->tete->pred = c;
    }
    c->pred = NULL;
    l->tete = c;
}

void afficher_liste(liste l)
{

    if (l.tete == NULL)
    {
        move(25, 80);
        clrtoeol();
        mvprintw(25, 80, "Liste vide");
    }
    else
    {
        char chaine[256] = "";
        cellule *c = l.tete;
        move(25, 80);
        clrtoeol();
        while (c != NULL)
        {
            if (c->item != NULL)
            {
                sprintf(chaine, "%s{%d} -> ", chaine, c->item->id);
            }
            c = c->succ;
        }
        sprintf(chaine, "%sNULL", chaine);
        mvprintw(25, 80, chaine);
    }
}

cellule *rechercher(liste l, int id)
{
    cellule *c = l.tete;
    if (l.tete == NULL)
    {
        // printf("liste vide\n");
    }
    else
    {
        while (c != NULL && c->item->id != id)
        {
            c = c->succ;
        }
    }
    return c;
}

void supprimer(liste *l, cellule *c, int deleteItem)
{
    if (c != NULL)
    {
        if (c->pred != NULL)
        {
            c->pred->succ = c->succ;
        }
        else
        {
            l->tete = c->succ;
        }
        if (c->succ != NULL)
        {
            c->succ->pred = c->pred;
        }
        else
        {
            l->tete = c->pred;
        }
        if (deleteItem && c->item != NULL)
        {
            free(c->item);
            c->item = NULL;
        }
        free(c);
        c = NULL;
    }
    else
    {
        // printf("cellule non existante\n");
    }
}

void detruire_liste(liste *l)
{
    while (l->tete != NULL)
    {
        supprimer(l, l->tete, DELETE_ITEM);
    }
}

void delete_all_list(liste *l, liste (*l2)[60])
{
    while (l->tete != NULL)
    {
        item_t *item = l->tete->item;
        for (int h = 0; h < item->height; h++)
        {
            for (int w = 0; w < item->width; w++)
            {
                supprimer(&l2[item->y + h][item->x + w], rechercher(l2[item->y + h][item->x + w], item->id), DELETE_CELL);
            }
        }
        supprimer(l, l->tete, DELETE_ITEM);
    }
}