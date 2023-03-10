#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

#include "fonction.h"
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "item.h"
#include "interface.h"
#include "entity.h"

#include <limits.h>

void creer_partie()
{
    system("clear");
    printf("Création d'une partie...\n");
    // Code pour créer une partie
    // On peut afficher un message de confirmation et revenir au menu principal
    printf("La partie a été créée avec succès !\n");
    printf("Appuyez sur Entrée pour continuer.\n");
    while (getchar() != '\n')
        ;
    getchar(); // attendre une touche
}

char *afficher_salons()
{
    system("clear");
    // Affichage de la liste des salons de jeu disponibles
    DIR *dir;
    struct dirent *ent;      // structure pour stocker les informations d'un fichier
    char *ext = ".bin";      // extension des fichiers à rechercher
    int found = 0;           // booléen pour savoir si des fichiers ont été trouvés
    char **filenames = NULL; // tableau dynamique pour stocker les noms de fichier
    int num_files = 0;       // nombre de fichiers trouvés

    dir = opendir("./bin");
    if (dir == NULL)
    {
        perror("Erreur lors de l'ouverture du répertoire");
        exit(EXIT_FAILURE);
    }

    printf("Veuillez sélectionner un monde parmi ceux disponibles :\n");
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_type == DT_REG && strstr(ent->d_name, ext) != NULL)
        {
            char *name = strtok(ent->d_name, ".");
            printf("%s\n", name);
            found = 1;

            char *filename = malloc(strlen(ent->d_name) + 1);
            if (filename == NULL)
            {
                perror("Erreur lors de l'allocation de mémoire");
                exit(EXIT_FAILURE);
            }
            strcpy(filename, ent->d_name);
            for (char *c = filename; *c; c++)
                *c = tolower(*c);

            char **tmp_filenames = realloc(filenames, (num_files + 1) * sizeof(char *));
            if (tmp_filenames == NULL)
            {
                perror("Erreur lors de l'allocation de mémoire");
                exit(EXIT_FAILURE);
            }
            filenames = tmp_filenames;
            filenames[num_files++] = filename;
        }
    }

    if (errno != 0)
    {
        perror("Erreur lors de la lecture du répertoire");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    if (!found)
    {
        printf("Aucun monde trouvé.\n");
    }

    if (closedir(dir) != 0)
    {
        perror("Erreur lors de la fermeture du répertoire");
        exit(EXIT_FAILURE);
    }

    if (found)
    {
        printf("Saississez le nom du salon : ");
        char salon[50];
        scanf("%50s", salon);
        // convertir le nom du salon en minuscules
        for (char *c = salon; *c; c++)
        {
            *c = tolower(*c);
        }
        // vérifier si le nom du salon est dans le tableau
        int room_found = 0;
        for (int i = 0; i < num_files; i++)
        {
            // extraire le nom de fichier sans l'extension
            char *name = strtok(filenames[i], ".");
            if (strcmp(salon, name) == 0)
            {
                room_found = 1;
                break;
            }
        }
        if (room_found)
        {
            printf("Vous avez selectionné le salon %s.\n", salon);
            char *room_name = malloc(strlen(salon) + 1);
            if (room_name == NULL)
            {
                perror("Erreur lors de l'allocation de mémoire");
                exit(EXIT_FAILURE);
            }
            strcpy(room_name, salon);
            return room_name;
        }
        else
        {
            printf("Le salon %s n'existe pas. Veuillez réessayer.\n", salon);
        }
        // désallouer la mémoire allouée pour les noms de fichier
        for (int i = 0; i < num_files; i++)
        {
            free(filenames[i]);
        }
        free(filenames);
    }
    printf("Appuyez sur Entrée pour continuer.\n");
    while (getchar() != '\n')
        ;
    getchar(); // attendre une touche
    return NULL;
}

interface_t *interface_create_game(char *path)
{
    interface_t *result;

    // Structure allocation
    if ((result = malloc(sizeof(interface_t))) == NULL)
    {
        ncurses_stop();
        perror("Erreur allocation interface");
        exit(EXIT_FAILURE);
    }

    // fenetre informations
    result->win_infos = window_create(0, 22, 77, 5, "Informations", TRUE);
    window_printw_col(result->win_infos, RED, "Entrez sur 'ECHAP' pour quitter\n");
    window_refresh(result->win_infos);

    // fenetre coordonnées
    result->win_level = window_create(0, 0, 62, 22, "Level", FALSE);

    window_refresh(result->win_level);
    // fenetre tools
    result->win_tools = window_create(62, 0, 15, 22, "HUD", FALSE); // HUD : Heads Up Display

    result->current_color = MAGENTA; // default color
    result->selection = ID_BLOCK;    // default selection
    interface_hud_update(result);
    window_refresh(result->win_tools);

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        ncurses_stop();
        fprintf(stderr, "Erreur lors de la création du fichier %s : %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    bloc_t *bloc = loadBloc(fd, 0);
    if (bloc == NULL)
    {
        fprintf(stderr, "Error: Failed to load block from file %s\n", path);
        closeFile(fd);
        ncurses_stop();
        exit(EXIT_FAILURE);
    }
    level_t *level = loadLevelById(fd, bloc, 0);
    if (level == NULL)
    {
        fprintf(stderr, "Error: Failed to load level from file %s\n", path);
        closeFile(fd);
        ncurses_stop();
        exit(EXIT_FAILURE);
    }

    convertToItem(result, level);

    free(bloc);
    free(level);
    closeFile(fd);

    return result;
}

void convertToItem(interface_t *interface, level_t *level)
{
    int i, j, tmp_posX, tmp_posY;
    int bloc_width = 0, bloc_height = 0, nb_door = 0;
    int tab[HEIGHT][WIDTH];

    // get the integer tab from the file
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
            tab[i][j] = level->tab[i][j];
    }
    // convert the integer tab to item tab by getting the head & details of each entity
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            getEntityDetail(interface, tab, j, i, &bloc_width, &bloc_height, &nb_door);
            tmp_posX = j, tmp_posY = i;
            if (bloc_width == 0 || bloc_height == 0)
                continue;
            getHeadEntity(tab, &tmp_posX, &tmp_posY, bloc_width, bloc_height);
            if (tmp_posX == j && tmp_posY == i)
            {
                interface->tab_item[i][j] = init_item(tab[i][j], j, i, bloc_width, bloc_height);
            }
            else // ajout de pointeur vers l'item de tête
            {
                // si on ne peut pas ajouter de pointeur d'item dans le tab_item, on ajoute un next dans sa structure
                // interface->tab_item[i][j] = init_item(tab[i][j], j, i, bloc_width, bloc_height);
                // interface->tab_item[tmp_posY][tmp_posX].next = &interface->tab_item[i][j];

                // DEBUG
                // ncurses_stop();
                // printf("ELSE bloc id : %d, bloc_width : %d, bloc_height : %d, position [%d][%d]\n", tab[i][j], bloc_width, bloc_height, i, j);
                // interface_delete(&interface);
                // exit(EXIT_FAILURE);
                // interface->tab_item[i][j] = init_item(9, j, i, bloc_width, bloc_height);
            }
        }
    }
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (interface->tab_item[i][j].id != 0)
            {
                display_item(interface->win_level, interface->tab_item[i][j], j, i);
            }
        }
    }
    // ncurses_stop();
    // printf("TAB : \n");
    // for (i = 0; i < HEIGHT; i++)
    // {
    //     for (j = 0; j < WIDTH; j++)
    //         printf("%d ", tab[i][j]);
    //     printf("\n");
    // }
    // printf("TAB ITEM : \n");
    // for (i = 0; i < HEIGHT; i++)
    // {
    //     for (j = 0; j < WIDTH; j++)
    //         printf("%d ", interface->tab_item[i][j].id);
    //     printf("\n");
    // }
    // interface_delete(&interface);
    // exit(EXIT_FAILURE);
}

void interface_hud_actions(interface_t *interface, int c)
{
    int mouseX, mouseY, posX, posY;
    // Mouse management
    if ((c == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {
        if (window_getcoordinates(interface->win_tools, mouseX, mouseY, &posX, &posY))
        {
            interface_hud_update(interface);
            window_refresh(interface->win_tools);
        }
    }
}

void interface_hud_update(interface_t *interface)
{
    window_erase(interface->win_tools);
    window_mvprintw(interface->win_tools, 1, 1, "Key");

    // temporaire : player
    item_t player = init_item(ID_PLAYER + 1, 2, 2, 3, 4);

    // temporaire : pour afficher les clés
    int color = 1;
    for (int j = 0; j < player.properties.player.nb_life * 2; j += 2)
    {
        color++;
        if (color > 4)
            color = 1;

        item_t key = init_item(ID_KEY + color, 2 + j, 3, 1, 2);
        display_item(interface->win_tools, key, key.x, key.y);
    }

    window_mvprintw(interface->win_tools, 6, 1, "Lives");

    // temporaire : pour afficher les vies
    for (int i = 0; i < player.properties.player.nb_life; i++)
    {
        item_t life = init_item(ID_LIFE, 2 + i, 8, 1, 1);
        display_item(interface->win_tools, life, life.x, life.y);
    }

    window_mvprintw(interface->win_tools, 10, 1, "Bombs");

    // temporaire : pour afficher les bombes
    for (int i = 0; i < player.properties.player.nb_bomb; i++)
    {
        item_t bomb = init_item(ID_BOMB, 2 + i, 12, 1, 1);
        display_item(interface->win_tools, bomb, bomb.x, bomb.y);
    }

    window_mvprintw(interface->win_tools, 14, 1, "Levels");
    // window_printw(interface->win_tools, " \n  %02d \n", nb_level);

    window_refresh(interface->win_tools);
}