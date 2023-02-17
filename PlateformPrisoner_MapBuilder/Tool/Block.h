// Created by: Souhail Mtarfi
#ifndef BLOCK_H
#define BLOCK_H


typedef struct Block
{
    int largeur;
    int hauteur;
    int couleur;
} Block_t;

Block_t *initBlock(int largeur, int hauteur);
void drawBlock(window_t *window, int x, int y, int color);


#endif // BLOCK_H