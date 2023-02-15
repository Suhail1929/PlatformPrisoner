#include "data_array.h"

void data_array_init(data_array_t *data_array)
{
    data_array->nbre_niveau = 0;
    data_array->next = 0;
    for (int i = 0; i < TAILLE_MAX; i++)
    {
        data_array->niveaux[i] = 0;
    }
}
void data_array_add(data_array_t *data_array, niveau_t niveau, char *file)
{
    if (data_array->nbre_niveau == TAILLE_MAX)
    {
        printf("Erreur: la taille maximale de la table d'adresse est atteinte\n");
        // il faut créer un nouveau bloc et ajouter le niveau à ce nouveau bloc avec le pointeur next
        return;
    }
    data_array->niveaux[data_array->nbre_niveau] = niveau_add(&niveau, file);
    data_array->nbre_niveau++;
}
void data_array_remove(data_array_t *data_array, int index, char *file)
{
    if (index >= data_array->nbre_niveau)
    {
        // à faire : il faut verfier l'index s'il existe un niveau à la table d'adresse suivante
        printf("Erreur: l'index est supérieur au nombre de niveaux\n");
        return;
    }
    // mettre le boolen is_delete (niveau_t) d à true du niveau à supprimer
    niveau_remove(data_array->niveaux[index], file);
    // on décale tous les niveaux à partir de l'index
    for (int i = index; i < data_array->nbre_niveau; i++)
    {
        data_array->niveaux[i] = data_array->niveaux[i + 1];
    }
    // on décrémente le nombre de niveau
    data_array->nbre_niveau--;
}
