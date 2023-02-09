// # Author: HAYAT Rahim && MTARFI Souhail

#ifndef NIVEAU_H
#define NIVEAU_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct niveau
{
    off_t niveau_position; // je sais pas si sera utile , on verra

    int id;
    char *nom;
    char *description;
    int *id_adresse;
    int is_deleted; // boolean pour savoir si le niveau est supprimé ou non
    // à modifier
} niveau_t;

void niveau_init(niveau_t *niveau);
off_t niveau_add(niveau_t *niveau, char *file);
void niveau_remove(off_t niveau_pos, char *file);

#endif