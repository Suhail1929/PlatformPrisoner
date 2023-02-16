// Created by: Souhail Mtarfi
#ifndef BLOCK_H
#define BLOCK_H
#include <ncurses.h>
#include <stdlib.h>
#include "../Fonction/fonction.h"
#include "../Couleur/couleur.h"
#include "../Level/niveau.h"
#include "../Interface/interface.h"
#include "../Window/window.h"

typedef struct Block
{
    int largeur;
    int hauteur;
    int couleur;
} Block_t;

Block_t *initBlock(int largeur, int hauteur);
void drawBlock(window_t *window, int x, int y, int color);

#endif // BLOCK_H