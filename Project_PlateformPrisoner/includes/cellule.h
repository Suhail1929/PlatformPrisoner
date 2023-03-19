// # Author: HAYAT Rahim

#ifndef __CELLULE_H__
#define __CELLULE_H__

typedef struct cellule cellule;
struct cellule
{
    item_t *item;
    cellule *succ;
};

cellule *init_cellule(item_t *item);

#endif // __CELLULE_H__
