#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "data_table.h"

int creerFichier(char *fileName)
{
    int fd = open(fileName, O_CREAT | O_RDWR | O_TRUNC, 0666);
    return fd;
}

void create_data_block(data_t *block)
{
    // Vérifie si l'argument est valide
    if (block == NULL)
    {
        errno = EINVAL;
        return;
    }

    // Initialise chaque cellule du bloc de données avec les valeurs par défaut
    for (int i = 0; i < BLOC_SIZE; i++)
    {
        block->bloc[i].hole = 0; // false
        block->bloc[i].data_position = -1;
        block->bloc[i].data_size = 0;
    }

    // Initialise le pointeur suivant à -1 pour indiquer qu'il n'y a pas de bloc suivant
    block->next = -1;
}

void delete_all_tables(int fd, data_t *first_table)
{
    // Vérifie si l'argument est valide
    if (first_table == NULL)
    {
        errno = EINVAL;
        return;
    }

    // bloc de données vide
    data_t empty_block;
    memset(&empty_block, 0, sizeof(data_t));
    empty_block.next = -1;

    // Libère chaque bloc de données
    off_t current_block = 0;
    off_t next_block;

    while (current_block != -1)
    {
        // Récupère le bloc de données courant
        data_t block;
        if (lseek(fd, current_block, SEEK_SET) == -1 || read(fd, &block, sizeof(data_t)) != sizeof(data_t))
        {
            errno = EIO;
            return;
        }

        // Récupère le bloc de données suivant
        next_block = block.next;

        // Supprime le bloc de données courant
        if (lseek(fd, current_block, SEEK_SET) == -1 || write(fd, &empty_block, sizeof(data_t)) != sizeof(data_t))
        {
            errno = EIO;
            return;
        }

        // Déplace le pointeur vers le bloc de données suivant
        current_block = next_block;
    }

    // Initialise le pointeur de la première table à -1 pour indiquer qu'il n'y a plus de table
    if (lseek(fd, 0, SEEK_SET) == -1 || write(fd, &empty_block, sizeof(data_t)) != sizeof(data_t))
    {
        errno = EIO;
        return;
    }
}

void print_data(int fd, data_t *table)
{
    struct stat sb;

    // Vérifier si le fichier est correctement mappé
    if (fstat(fd, &sb) == -1)
    {
        perror("Error: fstat failed");
        return;
    }

    // Calculer le nombre de cellules remplies dans la table
    int num_cells = 0;
    for (int i = 0; i < BLOC_SIZE; i++)
    {
        if (table->bloc[i].data_size > 0)
        {
            num_cells++;
        }
    }

    // Parcourir les cellules remplies dans la table
    for (int i = 0; i < num_cells; i++)
    {
        printf("bloc n° %d, cell n° %d\n", i / BLOC_SIZE, i % BLOC_SIZE);
        printf("position : %lld\n", (long long)table->bloc[i % BLOC_SIZE].data_position);
        printf("taille : %d\ndonnees : ", table->bloc[i % BLOC_SIZE].data_size);

        // Afficher les données de la cellule (tableau 2D d'entiers)
        for (int j = 0; j < table->bloc[i % BLOC_SIZE].data_size; j++)
        {
            for (int k = 0; k < table->bloc[i % BLOC_SIZE].data_size; k++)
            {
                printf("%d ", *((int *)(table->bloc[i % BLOC_SIZE].data_position + (j * table->bloc[i % BLOC_SIZE].data_size + k) * sizeof(int))));
            }
        }
        printf("\n");
    }
}