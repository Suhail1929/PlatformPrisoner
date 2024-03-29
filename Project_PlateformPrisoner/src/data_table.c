#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "data_table.h"

int nb_level;

/*
 *  Function that load the level
 *  @param fd : le descripteur de fichier
 */

void load_nb_level(int fd)
{
    lseek(fd, 0, SEEK_SET);
    read(fd, &nb_level, sizeof(int));
}

/*
 * Function that open a file
 */

int openFile(char *fileName)
{
    int fd = open(fileName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
    {
        fprintf(stderr, "Erreur lors de la création du fichier %s : %s\n", fileName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
}

/*
 *Function that close a file
 */
void closeFile(int fd)
{
    if (close(fd) == -1)
    {
        fprintf(stderr, "Erreur lors de la fermeture du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/*
 * Function that init a level
 */
level_t *initLevel()
{
    level_t *level = malloc(sizeof(level_t));
    if (level == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    level->id = nb_level;
    level->hole = 0;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            level->tab[i][j] = 0;
        }
    }
    //  un outliner
    for (int i = 0; i < HEIGHT; i++)
    {
        level->tab[i][0] = 1;
        level->tab[i][WIDTH - 1] = 1;
    }
    for (int i = 0; i < WIDTH; i++)
    {
        level->tab[0][i] = 1;
        level->tab[HEIGHT - 1][i] = 1;
    }
    nb_level++;
    return level;
}

/*
 * Function that init a bloc
 */

bloc_t *initBloc()
{
    bloc_t *bloc = malloc(sizeof(bloc_t));
    if (bloc == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        bloc->level[i] = -1;
    }
    bloc->nb_available_entry = MAX_LEVEL;
    bloc->bloc_next = -1;
    return bloc;
}

/*
 * Function that save a level in a file and save the offset of the level in the bloc
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param level : the level
 */

void addLevel(int fd, bloc_t *bloc, level_t *level)
{
    if (fd == -1)
    {
        fprintf(stderr, "Erreur lors de la création du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bloc == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (level == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (bloc->nb_available_entry != 0)
    {
        for (int i = 0; i < MAX_LEVEL; i++)
        {
            if (bloc->level[i] == -1)
            {
                bloc->level[i] = lseek(fd, 0, SEEK_END);
                if (write(fd, level, sizeof(level_t)) == -1)
                {
                    fprintf(stderr, "Erreur lors de l'écriture du niveau : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                bloc->nb_available_entry--;
                return;
            }
        }
    }

    if (bloc->nb_available_entry == 0 && bloc->bloc_next == -1)
    {
        bloc_t *next_bloc = initBloc();
        addLevel(fd, next_bloc, level);
        bloc->bloc_next = saveBloc(fd, next_bloc);
    }
    else if (bloc->nb_available_entry == 0 && bloc->bloc_next != -1)
    {
        if (lseek(fd, bloc->bloc_next, SEEK_SET) == -1)
        {
            fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        bloc_t *next_bloc = malloc(sizeof(bloc_t));
        if (read(fd, next_bloc, sizeof(bloc_t)) == -1)
        {
            fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        addLevel(fd, next_bloc, level);
        updateBloc(fd, bloc->bloc_next, next_bloc);
    }
}
/*
 * Function that update a level
 * @param fd : the file descriptor
 * @param offset : the offset of the level
 * @param level : the level
 */
void updateLevel(int fd, off_t offset, level_t *level)
{
    if (level == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (write(fd, level, sizeof(level_t)) == -1)
    {
        fprintf(stderr, "Erreur lors de l'écriture du niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
/*
 * Function that delete a level
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param id : the id of the level
 */
void deleteLevel(int fd, bloc_t *bloc, int id)
{
    if (fd == -1)
    {
        fprintf(stderr, "Erreur lors de la création du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bloc == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le niveau : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_LEVEL; i++)
    {
        if (bloc->level[i] != -1)
        {
            if (lseek(fd, bloc->level[i], SEEK_SET) == -1)
            {
                fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            level_t *level = malloc(sizeof(level_t));
            if (read(fd, &level, sizeof(level_t)) == -1)
            {
                fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            if (level->id == id)
            {
                level->hole = 1;
                bloc->nb_available_entry++;
                return;
            }
        }
    }
    if (bloc->bloc_next != -1)
    {
        if (lseek(fd, bloc->bloc_next, SEEK_SET) == -1)
        {
            fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        bloc_t *next_bloc = malloc(sizeof(bloc_t));
        if (read(fd, &next_bloc, sizeof(bloc_t)) == -1)
        {
            fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        deleteLevel(fd, next_bloc, id);
        printf("Aucun niveau n'est supprimé\n");
        return;
    }
}
/*
 * Function that save a bloc in the file and return the offset
 * @param fd : the file descriptor
 * @param bloc : the bloc
 */
off_t saveBloc(int fd, bloc_t *bloc)
{
    if (fd == -1)
    {
        fprintf(stderr, "Erreur lors de la création du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bloc == NULL)
    {
        fprintf(stderr, "Erreur : la structure bloc est NULL\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(fd, 0, SEEK_END);
    if (offset == -1)
    {
        fprintf(stderr, "Erreur lors de la récupération de l'offset : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(fd, bloc, sizeof(bloc_t));
    if (bytes_written == -1)
    {
        fprintf(stderr, "Erreur lors de l'écriture des données : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bytes_written != sizeof(bloc_t))
    {
        fprintf(stderr, "Erreur : nombre d'octets écrits incorrect : %ld\n", bytes_written);
        exit(EXIT_FAILURE);
    }

    return offset;
}
/*
 * Function that update a bloc
 * @param fd : the file descriptor
 * @param offset : the offset of the bloc
 * @param bloc : the bloc
 */
void updateBloc(int fd, off_t offset, bloc_t *bloc)
{
    if (fd == -1)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bloc == NULL)
    {
        fprintf(stderr, "Erreur : la structure bloc est NULL\n");
        exit(EXIT_FAILURE);
    }

    // On se place à l'offset spécifié
    off_t current_offset = lseek(fd, offset, SEEK_SET);
    if (current_offset == -1)
    {
        fprintf(stderr, "Erreur lors de la récupération de l'offset : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(fd, bloc, sizeof(bloc_t));
    if (bytes_written == -1)
    {
        fprintf(stderr, "Erreur lors de l'écriture des données : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bytes_written != sizeof(bloc_t))
    {
        fprintf(stderr, "Erreur : nombre d'octets écrits incorrect : %ld\n", bytes_written);
        exit(EXIT_FAILURE);
    }
}
/*
 * Function that load a bloc
 * @param fd : the file descriptor
 * @param offset : the offset of the bloc
 */
bloc_t *loadBloc(int fd, off_t offset)
{
    if (fd == -1)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // On se positionne à l'offset indiqué
    if (lseek(fd, offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    bloc_t *bloc = malloc(sizeof(bloc_t));
    if (bloc == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire pour la structure bloc : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    ssize_t read_result = read(fd, bloc, sizeof(bloc_t));
    if (read_result == -1)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
        free(bloc);
        return NULL;
    }
    else if (read_result != sizeof(bloc_t))
    {
        fprintf(stderr, "la lecture du fichier a retourné un nombre d'octets différent de la taille de la structure bloc_t\n");
        free(bloc);
        return NULL;
    }

    return bloc;
}
/*
 * Function that display a bloc
 * @param fd : the file descriptor
 * @param bloc : the bloc
 */
void displayBloc(int fd, bloc_t *bloc)
{
    printf("nb_available_entry: %d\n", bloc->nb_available_entry);
    printf("bloc_next: %ld\n", (long)bloc->bloc_next);

    printf("levels : \n");
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        printf("Level %d -> %ld \n", i, (long)bloc->level[i]);
    }
    printf("\n");

    for (int i = 0; i < 5; i++)
    {
        off_t offset = bloc->level[i];
        if (offset != -1)
        {
            level_t *level = loadLevel(fd, offset);
            displayLevel(level);
        }
    }
    if (bloc->bloc_next != -1)
    {
        bloc_t *next_bloc = loadBloc(fd, bloc->bloc_next);
        displayBloc(fd, next_bloc);
    }
}
/*
 * Function that load a level
 * @param fd : the file descriptor
 * @param offset : the offset of the level
 */
level_t *loadLevel(int fd, off_t offset)
{
    if (lseek(fd, offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier \n");
        exit(EXIT_FAILURE);
    }

    level_t *level = malloc(sizeof(level_t));
    if (level == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire pour la structure level \n");
        exit(EXIT_FAILURE);
    }

    ssize_t read_result = read(fd, level, sizeof(level_t));
    if (read_result == -1)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier \n");
        exit(EXIT_FAILURE);
    }
    else if (read_result != sizeof(level_t))
    {
        fprintf(stderr, "Erreur : la lecture du fichier a retourné un nombre d'octets différent de la taille de la structure level_t\n");
        exit(EXIT_FAILURE);
    }

    return level;
}
/*
 * Function that find a level in a bloc
 * @param fd : the file descriptor
 * @param Bloc : the bloc
 * @param id : the id of the level
 */
off_t findLevel(int fd, bloc_t *Bloc, int id)
{
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        if (Bloc->level[i] != -1)
        {
            if (lseek(fd, Bloc->level[i], SEEK_SET) == -1)
            {
                fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            level_t *level = malloc(sizeof(level_t));
            if (read(fd, level, sizeof(level_t)) == -1)
            {
                fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
                free(level);
                exit(EXIT_FAILURE);
            }

            if (level->id == id)
            {
                free(level);
                return Bloc->level[i];
            }
            free(level);
        }
    }
    if (Bloc->bloc_next != -1)
    {
        bloc_t *bloc = loadBloc(fd, Bloc->bloc_next);
        return findLevel(fd, bloc, id);
    }
    return -1;
}
/*
 * Function that load a level by id
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param id : the id of the level
 */
level_t *loadLevelById(int fd, bloc_t *bloc, int id)
{
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        if (bloc->level[i] != -1)
        {
            if (lseek(fd, bloc->level[i], SEEK_SET) == -1)
            {
                fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            level_t *level = malloc(sizeof(level_t));
            if (read(fd, level, sizeof(level_t)) == -1)
            {
                fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
                free(level);
                exit(EXIT_FAILURE);
            }

            if (level->id == id)
            {
                return level;
            }
            free(level);
        }
    }
    if (bloc->bloc_next != -1)
    {
        if (lseek(fd, bloc->bloc_next, SEEK_SET) == -1)
        {
            fprintf(stderr, "Erreur lors du déplacement dans le fichier : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        bloc_t *next_bloc = malloc(sizeof(bloc_t));
        if (read(fd, next_bloc, sizeof(bloc_t)) == -1)
        {
            fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        return loadLevelById(fd, next_bloc, id);
    }
    // printf("le niveau n'existe pas \n");
    return NULL;
}
/*
 * Function that display a level
 * @param level : the level
 */
void displayLevel(level_t *level)
{
    if (level != NULL)
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (level->tab[i][j] >= 10)
                {
                    printf("%d", level->tab[i][j]);
                }
                else
                {
                    printf("%d ", level->tab[i][j]);
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}

int loadOrCreatefromFile(const char *path, bloc_t *bloc, level_t *level, int createIfNotExists)
{
    int fd = open(path, O_RDWR);
    if (fd == -1 && createIfNotExists)
    {
        fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (fd == -1)
        {
            fprintf(stderr, "Error: Failed to create file %s : %s\n", path, strerror(errno));
            exit(EXIT_FAILURE);
        }

        bloc = loadBloc(fd, 0);
        if (bloc == NULL)
        {
            // Create a Bloc : Address Table
            bloc = initBloc();
            saveBloc(fd, bloc);
            level = initLevel();
            addLevel(fd, bloc, level);
            updateBloc(fd, 0, bloc);
        }
        level = loadLevelById(fd, bloc, 0);
        if (level == NULL)
        {
            level = initLevel();
        }
        return fd;
    }
    else if (fd == -1)
    {
        fprintf(stderr, "Error: Failed to load file %s : %s\n", path, strerror(errno));
        return -1;
    }

    bloc = loadBloc(fd, 0);
    if (bloc == NULL)
    {
        fprintf(stderr, "Error: Failed to load block from file %s\n", path);
        closeFile(fd);
        return -1;
    }

    level = loadLevelById(fd, bloc, 0);
    if (level == NULL)
    {
        fprintf(stderr, "Error: Failed to load level from file %s\n", path);
        closeFile(fd);
        return -1;
    }
    return fd;
}