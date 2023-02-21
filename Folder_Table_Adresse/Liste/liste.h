// # Author: HAYAT Rahim && MTARFI Souhail

#ifndef __LISTE_H__
#define __LISTE_H__
#include "../Table_Adresse/bloc.h"

typedef struct list_t
{
    off_t head;
    int total_entry;
    int nb_available;
} list_t;

void init_list(list_t *l);
void destroy_list(list_t *l);
void insert_list(list_t *l, bloc_t *c);
void display_list(list_t l);
bloc_t *search_list(list_t l, int sommet);
void delete_list(list_t *l, bloc_t *c);

#endif // __LISTE_H__