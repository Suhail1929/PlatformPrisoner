#include "data.h"

void creerTableAdresse(data_t **table, int numEntries, int isHole)
{
    if (isHole < 0 || isHole > 1)
    {
        fprintf(stderr, "Erreur: le paramètre isHole doit être 0 ou 1\n");
        exit(EXIT_FAILURE);
    }
    *table = (data_t *)malloc(numEntries * sizeof(data_t));
    for (int i = 0; i < numEntries; i++)
    {
        (*table)[i].hole = isHole;
    }
}

void supprimerTableAdresse(data_t **table)
{
    free(*table);
    *table = NULL;
}

void lireEntreeAdresse(data_t *table, int entry)
{
    printf("Position: %lld\n", table[entry].data_position);
    printf("Taille: %d\n", table[entry].data_size);
}

void ecrireEntreeAdresse(data_t *table, int entry, off_t data_position, int data_size)
{
    table[entry].data_position = data_position;
    table[entry].data_size = data_size;
}

void affichierTableAdresse(data_t *table, int numEntries, int fd)
{
    if (numEntries == 0)
    {
        printf("La table est vide\n");
        return;
    }
    printf("La table d'adresse contient %d entrées\n", numEntries);
    for (int i = 0; i < numEntries; i++)
    {
        printf("Entree %d\n", i);
        lireEntreeAdresse(table, i);
        char *buff = (char *)malloc(table[i].data_size);
        lireChaine(fd, table, i, buff, table[i].data_size);
        printf("Chaine: %s\n\n", buff);
    }
}

// Fonction pour ajouter une entrée à la table d'adresses
void ajouterEntreeAdresse(data_t *table, int *numEntries, off_t data_position, int data_size)
{
    table[*numEntries].data_position = data_position;
    table[*numEntries].data_size = data_size;
    (*numEntries)++;
}

// Fonction pour supprimer une entrée de la table d'adresses
void supprimerEntreeAdresse(data_t *adresse_t, int *total_entry, int entry) // prendra 2 liste : address et hole, total entry : sera dans la structure de la liste
{
    // ajouter dans la table des trous, mais puisqu'il peut y avoir plusieurs table de trous différent.
    // Il faut faire une liste chainée de table de trous, aussi pour la table d'adresse

    for (int i = entry; i < *total_entry - 1; i++)
    {
        adresse_t[i].data_position = adresse_t[i + 1].data_position;
        adresse_t[i].data_size = adresse_t[i + 1].data_size;
    }
    (*total_entry)--;
}

void lireChaine(int fd, data_t *table, int entry, char *buffer, int bufferdata_size)
{
    if (lseek(fd, table[entry].data_position, SEEK_SET) == -1)
    {
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");
        if (errno == EINVAL)
            fprintf(stderr, "The value of whence is invalid.\n");
        exit(EXIT_FAILURE);
    }
    if (read(fd, buffer, table[entry].data_size) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and no data was yet available for reading.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");

        exit(EXIT_FAILURE);
    }
}

void ecrireChaine(int fd, data_t *table, int entry, char *buffer, int bufferdata_size)
{
    if (lseek(fd, table[entry].data_position, SEEK_SET) == -1)
    {
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");
        if (errno == EINVAL)
            fprintf(stderr, "The value of whence is invalid.\n");

        exit(EXIT_FAILURE);
    }
    if (write(fd, buffer, bufferdata_size) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and the write would block.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for writing.\n");
        exit(EXIT_FAILURE);
    }
}

int creerFichier(char *fileName)
{
    int fd;
    if ((fd = open(fileName, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
    {
        if (errno == EACCES)
            fprintf(stderr, "Search permission is denied for a component of the path prefix.\n");
        if (errno == EEXIST)
            fprintf(stderr, "O_CREAT and O_EXCL are set, but the named file exists.\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void ecrireTableDansFichier(int fd, data_t *addressTable, int numAddressEntries, data_t *holeTable, int numHoleEntries)
{
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");
        if (errno == EINVAL)
            fprintf(stderr, "The value of whence is invalid.\n");
        exit(EXIT_FAILURE);
    }
    if (write(fd, &numAddressEntries, sizeof(int)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and the write would block.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for writing.\n");
        exit(EXIT_FAILURE);
    }
    if (write(fd, addressTable, numAddressEntries * sizeof(data_t)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and the write would block.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for writing.\n");
        exit(EXIT_FAILURE);
    }
    if (write(fd, &numHoleEntries, sizeof(int)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and the write would block.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for writing.\n");
        exit(EXIT_FAILURE);
    }
    if (write(fd, holeTable, numHoleEntries * sizeof(data_t)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and the write would block.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for writing.\n");
        exit(EXIT_FAILURE);
    }
    if (lseek(fd, 0, SEEK_END) == -1)
    {
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");
        if (errno == EINVAL)
            fprintf(stderr, "The value of whence is invalid.\n");
        exit(EXIT_FAILURE);
    }
}

void lireTableDepuisFichier(int fd, data_t **addressTable, int *numAddressEntries, data_t **holeTable, int *numHoleEntries)
{
    if (read(fd, numAddressEntries, sizeof(int)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and no data was yet available for reading.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");

        exit(EXIT_FAILURE);
    }
    *addressTable = (data_t *)malloc(*numAddressEntries * sizeof(data_t));
    if (read(fd, *addressTable, *numAddressEntries * sizeof(data_t)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and no data was yet available for reading.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");

        exit(EXIT_FAILURE);
    }
    if (read(fd, numHoleEntries, sizeof(int)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and no data was yet available for reading.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");

        exit(EXIT_FAILURE);
    }
    *holeTable = (data_t *)malloc(*numHoleEntries * sizeof(data_t));
    if (read(fd, *holeTable, *numHoleEntries * sizeof(data_t)) == -1)
    {
        if (errno == EAGAIN)
            fprintf(stderr, "Non-blocking I/O has been selected using O_NONBLOCK and no data was yet available for reading.\n");
        if (errno == EBADF)
            fprintf(stderr, "fd is not a valid file descriptor or is not open for reading.\n");

        exit(EXIT_FAILURE);
    }
}
