#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

void init_list(list_t *l)
{
    l->head = NULL;
}

void insert_list(list_t *l, bloc_t *c)
{
    if (l->head == NULL)
    {
        c->next = NULL;
    }
    else
    {
        c->next = l->head;
        l->head->prev = c;
    }
    c->prev = NULL;
    l->head = c;
    l->nb_entry++;
}

void display_list(list_t l)
{
    // à compléter
    //   if (l.tete == NULL)
    // {
    //     printf("Liste vide\n");
    // }
    // else
    // {
    //     cellule *c = l.tete;
    //     while (c != NULL)
    //     {
    //         if (c->poids == -1) // graphe non valué
    //         {
    //             printf("%d -> ", c->sommet);
    //         }
    //         else // valué
    //         {
    //             printf("%d{%d} -> ", c->sommet, c->poids);
    //         }
    //         c = c->succ;
    //     }
    //     printf("NULL\n");
    // }
}

bloc_t *search_list(list_t l, int sommet)
{
    // à compléter
    // bloc_t *c = l.head;
    // if (l.head == NULL)
    // {
    //     printf("list_t vide\n");
    // }
    // else
    // {
    //     while (c != NULL && c->sommet != sommet) // on a pas sommet !!!!!!
    //     {
    //         c = c->next;
    //     }
    // }
    // return c;
}

void delete_list(list_t *l, bloc_t *c)
{
    if (c != NULL)
    {
        if (c->prev != NULL)
        {
            c->prev->next = c->next;
        }
        else
        {
            l->head = c->next;
        }
        if (c->next != NULL)
        {
            c->next->prev = c->prev;
        }
        else
        {
            l->head = c->prev;
        }
        l->nb_entry--;
        free(c);
    }
    else
    {
        perror("cellule non existante\n");
    }
}

void destroy_list(list_t *l)
{
    while (l->head != NULL)
    {
        delete_list(l, l->head);
    }
}
