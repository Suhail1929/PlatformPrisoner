#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "niveau.h"
#define TAILLE_MAX 10
typedef struct data_array
{
    off_t niveaux[TAILLE_MAX];
    int nbre_niveau;
    off_t next;
    // à modifier
} data_array_t;

void data_array_init(data_array_t *data_array);
void data_array_add(data_array_t *data_array, niveau_t niveau, char *file);
void data_array_remove(data_array_t *data_array, int index, char *file);

#endif