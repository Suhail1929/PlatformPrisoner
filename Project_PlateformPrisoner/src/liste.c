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
        printf("liste vide\n");
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

void supprimer(liste *l, cellule *c)
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
        free(c->item);
        free(c);
    }
    else
    {
        printf("cellule non existante\n");
    }
}

void detruire_liste(liste *l)
{
    while (l->tete != NULL)
    {
        supprimer(l, l->tete);
    }
}