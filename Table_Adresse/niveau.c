// # Author: HAYAT Rahim && MTARFI Souhail
#include "niveau.h"
void niveau_init(niveau_t *niveau)
{
    niveau->niveau_position = 0;
    niveau->id = 0;
    niveau->nom = NULL;
    niveau->description = NULL;
    niveau->id_adresse = NULL;
    niveau->is_deleted = 0;
    // à modifier
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
        return 1;
    }
    // on recupre la position du curseur offset
    offset = lseek(fd, niveau_pos, SEEK_SET);
    if (offset == -1)
    {
        perror("Error seeking to position 100");
        return 1;
    }
    if (read(fd, &niveau, sizeof(niveau_t)) == -1)
    {
        perror("Error reading file");
        return 1;
    }
    niveau.is_deleted = 1;
    // on recupre la position du curseur offset
    offset = lseek(fd, -sizeof(niveau_t), SEEK_CUR);
    // on écrit le niveau dans le fichier
    // on écrit le niveau dans le fichier
    if (write(fd, &niveau, sizeof(niveau_t)) == -1)
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
}