#ifndef __MONSTRE__
#define __MONSTRE__

#define WIDTH 3
#define HEIGHT 4

typedef struct Monstre
{
    unsigned char sprite[HEIGHT][WIDTH];
    int couleur;
} Monstre;

void init_monstre(Monstre *monstre);

#endif // __MONSTRE__