#ifndef DELETE_H
#define DELETE_H

typedef struct Delete
{
    int largeur;
    int hauteur;
} Delete_t;

Delete_t *initDelete(int largeur, int hauteur);
#endif