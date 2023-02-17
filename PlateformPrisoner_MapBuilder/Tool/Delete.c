#include "../Window/window.h"
#include "../Couleur/couleur.h"
#include "Delete.h"

Delete_t *initDelete(int largeur, int hauteur)
{
    Delete_t *delete = malloc(sizeof(Delete_t));
    delete->largeur = largeur;
    delete->hauteur = hauteur;
    return delete;
}