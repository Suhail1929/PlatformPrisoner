// # Author: HAYAT Rahim && MTARFI Souhail
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "../Window/window.h"
#include "../Couleur/couleur.h"
#include "../Tool/Block.h"
#include "../Tool/Start.h"
#include "../Tool/Exit.h"
#include "../Tool/Gate.h"
#include "../Tool/Key.h"
#include "../Tool/Ladder.h"
#include "../Tool/Life.h"
#include "../Tool/Probe.h"
#include "../Tool/Robot.h"
#include "../Tool/Bomb.h"
#include "../Tool/Door.h"
#include "../Tool/Player.h"
#include "../Tool/Trap.h"
#include "niveau.h"
#include "../Objet/objet.h"

int level_id = 1;
niveau_t *niveau_init()
{
    niveau_t *niveau = malloc(sizeof(niveau_t));
    niveau->niveau_position = 0;
    niveau->id = level_id;
    niveau->nom = NULL;
    niveau->description = NULL;
    niveau->id_adresse = NULL;
    niveau->is_deleted = 0;
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            niveau->objet[i][j] = 0;
        }
    }
    level_id++;
    // à modifier
    return niveau;
}
off_t niveau_add(niveau_t *niveau, char *file)
{
    off_t offset;
    int fd = open(file, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Erreur: %s\n", strerror(errno));
        return 1;
    }
    // on recupre la position du curseur offset
    offset = lseek(fd, 0, SEEK_END);
    if (offset == -1)
    {
        perror("Error seeking to position 100");
        return 1;
    }
    // on écrit le niveau dans le fichier
    if (write(fd, niveau, sizeof(niveau_t)) == -1)
    {
        perror("Error writing to file");
        return 1;
    }
    // on ferme le fichier
    if (close(fd) == -1)
    {
        perror("Error closing file");
        return 1;
    }
    niveau->niveau_position = offset;
    return offset;
}

void niveau_remove(off_t niveau_pos, char *file)
{
    niveau_t niveau;
    off_t offset;
    // on ouvre le fichier en mode lecture et écriture
    int fd = open(file, O_RDWR);
    if (fd == -1)
    {
        printf("Erreur: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // on recupre la position du curseur offset
    offset = lseek(fd, niveau_pos, SEEK_SET);
    if (offset == -1)
    {
        perror("Error seeking to position 100");
        exit(EXIT_FAILURE);
    }
    if (read(fd, &niveau, sizeof(niveau_t)) == -1)
    {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }
    niveau.is_deleted = 1;
    // on recupre la position du curseur offset
    offset = lseek(fd, -sizeof(niveau_t), SEEK_CUR);
    // on écrit le niveau dans le fichier
    // on écrit le niveau dans le fichier
    if (write(fd, &niveau, sizeof(niveau_t)) == -1)
    {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }
    // on ferme le fichier
    if (close(fd) == -1)
    {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
}
void display_niveau(niveau_t *niveau)
{
    printf("id: %d , nom: %s , description: %s , is_deleted: %d \n", niveau->id, niveau->nom, niveau->description, niveau->is_deleted);
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            printf("%d ", niveau->objet[i][j]);
        }
        printf("\n");
    }
}