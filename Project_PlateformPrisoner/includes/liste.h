// # Author: HAYAT Rahim

#ifndef __LISTE_H__
#define __LISTE_H__
#include "cellule.h"

typedef struct liste liste;
struct liste
{
    cellule *tete;
};

void initialiser_liste(liste *l);
void detruire_liste(liste *l);
void inserer(liste *l, cellule *c);
void afficher_liste(liste l);
cellule *rechercher(liste l, int id);
void supprimer(liste *l, cellule *c);

#endif // __LISTE_H__
