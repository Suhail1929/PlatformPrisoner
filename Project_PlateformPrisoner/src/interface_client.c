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
#include <pthread.h>

#include "fonction.h"
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "item.h"
#include "liste.h"
#include "interface.h"
#include "entity.h"

#define HORIZONTAL 1
#define VERTICAL 2

pthread_t affiche;
int stop = 0;
int nb_active_bomb = 0;
interface_t **tab_interface = NULL;
interface_t interface;
int current_level = 0;

pthread_mutex_t deplacement_mutex = PTHREAD_MUTEX_INITIALIZER;

// int compteur = 0;

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
        scanf("%49s", salon);
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

interface_t **interface_create_game(char *path, int *nb_interface)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        ncurses_stop();
        fprintf(stderr, "Erreur lors de la création du fichier %s : %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    bloc_t *block = loadBloc(fd, 0);
    int count_nb_level = 0;
    level_t *tab_levels = (level_t *)malloc(sizeof(level_t));
    if (tab_levels == NULL)
    {
        ncurses_stop();
        perror("Erreur d'allocation pour tab_levels");
        exit(EXIT_FAILURE);
    }

    while (block != NULL)
    {
        if (block == NULL)
        {
            printf("block == NULL \n");
            break;
        }

        for (int i = 0; i < 5; i++)
        {
            if (block->level[i] != -1)
            {
                level_t *newLevel = loadLevel(fd, block->level[i]);
                tab_levels = (level_t *)realloc(tab_levels, sizeof(level_t) * (count_nb_level + 1));
                if (tab_levels == NULL)
                {
                    ncurses_stop();
                    perror("Erreur de reallocation pour tab_levels");
                    exit(EXIT_FAILURE);
                }
                tab_levels[count_nb_level] = *newLevel;
                count_nb_level++;
            }
        }

        if (block->bloc_next == -1)
        {
            break;
        }
        block = loadBloc(fd, block->bloc_next);
    }

    *nb_interface = count_nb_level;

    if ((tab_interface = (interface_t **)malloc(sizeof(interface_t *) * count_nb_level)) == NULL)
    {
        ncurses_stop();
        perror("Erreur allocation interface");
        exit(EXIT_FAILURE);
    }

    for (int n_interface = 0; n_interface < count_nb_level; n_interface++)
    {
        if ((tab_interface[n_interface] = (interface_t *)malloc(sizeof(interface_t))) == NULL)
        {
            ncurses_stop();
            perror("Erreur allocation interface");
            exit(EXIT_FAILURE);
        }

        // niveau de l'interface
        tab_interface[n_interface]->n_level = n_interface;

        // fenetre informations
        tab_interface[n_interface]->win_infos = window_create(0, 22, 77, 5, "Informations", TRUE);
        window_printw_col(tab_interface[n_interface]->win_infos, RED, "Entrez sur 'ECHAP' pour quitter\n");

        // fenetre coordonnées
        tab_interface[n_interface]->win_level = window_create(0, 0, 62, 22, "Level", FALSE);

        // fenetre tools
        tab_interface[n_interface]->win_tools = window_create(62, 0, 15, 22, "HUD", FALSE); // HUD : Heads Up Display

        tab_interface[n_interface]->current_color = MAGENTA; // default color
        tab_interface[n_interface]->selection = ID_BLOCK;    // default selection

        // fenetre debug
        tab_interface[n_interface]->win_debug = window_create(80, 0, 62, 27, "DEBUG", FALSE);

        convertToItem(tab_interface[n_interface], &tab_levels[n_interface]);
    }

    // find the start and init the player at first level
    if (tab_interface[0]->n_level == 0)
    {
        find_start(tab_interface[0]);
    }

    free(block);
    free(tab_levels);

    closeFile(fd);

    interface_hud_update(tab_interface[0]);

    interface.current_interface = tab_interface[0];
    pthread_create(&affiche, NULL, routine_display, &interface);

    return tab_interface;
}

void pass_door(interface_t *interface, item_t *player, int door_id)
{
    if (door_id < 3100 || door_id > 3500)
    {
        ncurses_stop();
        printf("Erreur: l'identifiant de la porte est invalide.\n");
        return;
    }

    item_t *door_A = tab_door[door_id % 100 - 1].door_A;
    item_t *door_B = tab_door[door_id % 100 - 1].door_B;

    if (door_A->x == door_B->x && door_A->y == door_B->y)
    {
        // ncurses_stop();
        // printf("if 1\n");
        // exit(EXIT_FAILURE);
        if (door_A->properties.door.id_level == interface->n_level)
        {
            move_player_to_door(player, door_B, interface, interface);
        }
        else
        {
            move_player_to_door(player, door_A, interface, interface);
            change_interface(interface, door_A->properties.door.id_level);
        }
    }
    else if (player->x == door_A->x && player->y == door_A->y)
    {
        // ncurses_stop();
        // printf("if 2\n");
        // exit(EXIT_FAILURE);
        move_player_to_door(player, door_B, interface, interface);
        if (door_B->properties.door.id_level != interface->n_level)
        {
            change_interface(interface, door_B->properties.door.id_level);
        }
    }
    else if (player->x == door_B->x && player->y == door_B->y)
    {
        move_player_to_door(player, door_A, interface, interface);
        if (door_A->properties.door.id_level != interface->n_level)
        {
            change_interface(interface, door_A->properties.door.id_level);
        }
    }
    else
    {
        // ncurses_stop();
        // printf("Erreur: la porte avec l'identifiant %d est introuvable.\n", door_id);
        for (int i = 0; i < 99; i++)
        {
            if (tab_door[i].door_A != NULL || tab_door[i].door_B != NULL)
            {
                printf("lvl door_A: %d (%d, %d)\n", tab_door[i].door_A->properties.door.id_level, tab_door[i].door_A->x, tab_door[i].door_A->y);
                printf("lvl door_B: %d (%d, %d)\n", tab_door[i].door_B->properties.door.id_level, tab_door[i].door_B->x, tab_door[i].door_B->y);
                printf("\n");
            }
        }
        exit(EXIT_FAILURE);

        return;
    }
}

interface_t *change_interface(interface_t *interface, int new_level)
{
    // ncurses_stop();
    // printf("Changement de niveau: %d -> %d \n", interface->n_level, new_level);
    // exit(EXIT_FAILURE);

    // interface->current_interface = tab_interface[new_level];
    current_level = new_level;
    return tab_interface[new_level];
}

void move_player_to_door(item_t *player, item_t *destination, interface_t *interface, interface_t *new_interface)
{
    if (destination->properties.door.id_level != interface->n_level)
    {
        // déplacer l'item du player et ses pointeurs aussi.
        // + changer d'interface
        // ncurses_stop();
        // printf("if 3\n");
        // exit(EXIT_FAILURE);

        new_interface = tab_interface[destination->properties.door.id_level];
        for (int h = 0; h < player->height; h++)
        {
            for (int w = 0; w < player->width; w++)
            {
                // cleanup à faire !
                pthread_mutex_lock(&interface->tab_item[player->y + h][player->x + w].mutex);
                pthread_mutex_lock(&interface->tab_item[destination->y + h][destination->x + w].mutex);

                cellule *move_cell = rechercher(interface->tab_item[player->y + h][player->x + w], player->id);
                inserer(&new_interface->tab_item[destination->y + h][destination->x + w], init_cellule(move_cell->item));
                supprimer(&interface->tab_item[player->y + h][player->x + w], rechercher(interface->tab_item[player->y + h][player->x + w], player->id), DELETE_POINTER);

                pthread_mutex_unlock(&interface->tab_item[destination->y + h][destination->x + w].mutex);
                pthread_mutex_unlock(&interface->tab_item[player->y + h][player->x + w].mutex);
            }
        }
        // pthread_mutex_lock(&tab_player.mutex);
        // cellule *move_player = rechercher(interface->tab_player, player->id);
        // inserer(&new_interface->tab_player, init_cellule(move_player->item));
        // supprimer(&interface->tab_player, rechercher(interface->tab_player, player->id), DELETE_ITEM);
        // // ncurses_stop();
        // // printf("player\n");
        // // exit(EXIT_FAILURE);
        // pthread_mutex_unlock(&tab_player.mutex);

        player->y = destination->y;
        player->x = destination->x;
        change_interface(interface, destination->properties.door.id_level);
    }
    else
    {
        // déplacer l'item du player et ses pointeurs aussi.
        for (int h = 0; h < player->height; h++)
        {
            for (int w = 0; w < player->width; w++)
            {
                // cleanup à faire !
                // //#pthread_mutex_lock(&interface->tab_item[player->y + h][player->x + w].mutex);
                // //#pthread_mutex_lock(&interface->tab_item[destination->y + h][destination->x + w].mutex);

                cellule *move_cell = rechercher(interface->tab_item[player->y + h][player->x + w], player->id);
                inserer(&interface->tab_item[destination->y + h][destination->x + w], init_cellule(move_cell->item));
                supprimer(&interface->tab_item[player->y + h][player->x + w], rechercher(interface->tab_item[player->y + h][player->x + w], player->id), DELETE_POINTER);

                // //#pthread_mutex_unlock(&interface->tab_item[destination->y + h][destination->x + w].mutex);
                // //#pthread_mutex_unlock(&interface->tab_item[player->y + h][player->x + w].mutex);
            }
        }

        // pthread_mutex_lock(&interface->tab_player.mutex);
        // cellule *move_player = rechercher(interface->tab_player, player->id);
        // inserer(&new_interface->tab_player, init_cellule(move_player->item));
        // supprimer(&interface->tab_player, rechercher(interface->tab_player, player->id), DELETE_ITEM);
        // // pthread_mutex_unlock(&interface->tab_player.mutex);
        player->y = destination->y;
        player->x = destination->x;
    }
}

void convertToItem(interface_t *interface, level_t *level)
{
    int i, j, tmp_posX, tmp_posY;
    int bloc_width = 0, bloc_height = 0, nb_door = 0;

    // get the integer tab from the file + init all the list
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            tab[i][j] = level->tab[i][j];
            pthread_mutex_init(&interface->tab_item[i][j].mutex, NULL);
            initialiser_liste(&interface->tab_item[i][j]);
        }
    }
    pthread_mutex_init(&interface->global_item.mutex, NULL);
    initialiser_liste(&interface->global_item);

    // convert the integer tab to item tab by getting the head & details of each entity
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            getEntityDetail(interface, tab, j, i, &bloc_width, &bloc_height, &nb_door);
            tmp_posX = j, tmp_posY = i;
            if (bloc_width == 0 || bloc_height == 0 || tab[i][j] == 0)
                continue;
            getHeadEntity(tab, &tmp_posX, &tmp_posY, bloc_width, bloc_height);

            // head : create the item, otherwise add pointer to the head item
            if (tmp_posX == j && tmp_posY == i)
            {
                // item creation
                item_t *item = init_item(tab[i][j], j, i, bloc_width, bloc_height);

                // ajout des doors dans le tableau
                if (item->id > 3100 && item->id < 3500)
                {
                    item->properties.door.id_level = interface->n_level;
                    if (tab_door[(item->id % 100) - 1].door_A == NULL)
                    {
                        tab_door[(item->id % 100) - 1].door_A = item;
                    }
                    else if (tab_door[(item->id % 100) - 1].door_B == NULL)
                    {
                        tab_door[(item->id % 100) - 1].door_B = item;
                    }
                    else
                    {
                        // si + dea 2 portes avec le même identifiant
                        // Ignorer les autres portes portaient le même identifiant
                        // printf("Erreur: la porte %d a déjà deux portes associées.\n", item->id);
                    }
                }

                pthread_mutex_lock(&interface->global_item.mutex);
                inserer(&interface->global_item, init_cellule(item)); // ajout pour la tête
                pthread_mutex_unlock(&interface->global_item.mutex);
                init_thread_item(interface, item);

                // pointer to the item
                item_t *p_item = item;

                for (int h = 0; h < item->height; h++)
                {
                    for (int w = 0; w < item->width; w++)
                    {
                        // #pthread_mutex_lock(&interface->tab_item[item->y + h][item->x + w].mutex);
                        inserer(&interface->tab_item[item->y + h][item->x + w], init_cellule(p_item)); // add pointer to the item in the map
                        // #pthread_mutex_unlock(&interface->tab_item[item->y + h][item->x + w].mutex);
                    }
                }
            } // already item placed, skip the bloc width
            else
            {
                // j += bloc_width - 1; // A débugger !
                // BONUS, pour sauter les blocs déjà placés
            }
        }
    }
}

void init_thread_item(interface_t *interface, item_t *item)
{
    pthread_cond_init(&item->cond, NULL);

    switch (item->id)
    {
    case ID_ROBOT:
        item->thread = (pthread_t *)malloc(sizeof(pthread_t));
        data_thread *robot_data = malloc(sizeof(data_thread));
        robot_data->interface = interface;
        robot_data->item = item;

        pthread_create(item->thread, NULL, routine_robot, (void *)robot_data);
        break;
    case ID_TRAP:
        item->thread = (pthread_t *)malloc(sizeof(pthread_t));
        data_thread *trap_data = malloc(sizeof(data_thread));
        trap_data->interface = interface;
        trap_data->item = item;

        pthread_create(item->thread, NULL, routine_trap, (void *)trap_data);
        break;
    case ID_PROBE:
        item->thread = (pthread_t *)malloc(sizeof(pthread_t));
        data_thread *probe_data = malloc(sizeof(data_thread));
        probe_data->interface = interface;
        probe_data->item = item;

        pthread_create(item->thread, NULL, routine_probe, (void *)probe_data);
        break;
    case ID_LIFE:
        item->thread = (pthread_t *)malloc(sizeof(pthread_t));
        data_thread *life_data = malloc(sizeof(data_thread));
        life_data->interface = interface;
        life_data->item = item;

        pthread_create(item->thread, NULL, routine_bonus, (void *)life_data);
        break;
    case ID_BOMB:
        item->thread = (pthread_t *)malloc(sizeof(pthread_t));
        data_thread *bomb_data = malloc(sizeof(data_thread));
        bomb_data->interface = interface;
        bomb_data->item = item;

        pthread_create(item->thread, NULL, routine_bonus, (void *)bomb_data);
        break;
    case ID_ACTIVE_BOMB:
        item->thread = (pthread_t *)malloc(sizeof(pthread_t));
        data_thread *explosion_data = malloc(sizeof(data_thread));
        explosion_data->interface = interface;
        explosion_data->item = item;

        pthread_create(item->thread, NULL, routine_explosion, (void *)explosion_data);
        break;
    case 41 ... 49: // ID_PLAYER
        break;
    default:
        break;
    }
}

// void undraw_item(interface_t *interface, item_t item)
// {
//     // check the list, and draw the item if it's the head
//     item_t *item_behind = NULL;
//     for (int h = 0; h < item.height; h++)
//     {
//         for (int w = 0; w < item.width; w++)
//         {
//             if (interface->tab_item[item.y + h][item.x + w].tete != NULL)
//             {
//                 if (interface->tab_item[item.y + h][item.x + w].tete->item->id == ID_ACTIVE_BOMB)
//                 {
//                     // display_item(interface->win_level, *interface->tab_item[item.y + h][item.x + w].tete->item, item.x + w, item.y + h);
//                 }
//                 else if (interface->tab_item[item.y + h][item.x + w].tete->succ != NULL)
//                 {
//                     item_behind = interface->tab_item[item.y + h][item.x + w].tete->succ->item;
//                     if (item_behind != NULL)
//                     {
//                         // display_item(interface->win_level, *item_behind, item_behind->x, item_behind->y);
//                     }
//                 }
//                 else
//                 {
//                     window_mvaddch_col(interface->win_level, item.y + h, item.x + w, BLACK, ' ');
//                 }
//             }
//             else
//             {
//                 window_mvaddch_col(interface->win_level, item.y + h, item.x + w, BLACK, ' ');
//             }
//         }
//     }
// }

int interface_game_update(interface_t *interface, item_t *item, int c)
{
    // For bottom and right, we must verify if the player bloc is not out of the map (because the head is on top left)
    int obstacle = 0;
    int deplacement = -1;
    srand(time(NULL) + getpid());
    switch (c)
    {
    case 'z':
    case 'Z':
    case KEY_UP:
    case 2:
        if ((item->id >= 40 && item->id < 50) || item->id == ID_PROBE)
        {
            if (item->y - 1 >= 0)
            {
                obstacle += is_obstacle(interface, item, item->y - 1, item->x, HORIZONTAL);
                if (item->id == ID_PROBE && obstacle > 0)
                {
                    deplacement = rand() % 4; // 0-1-2-3
                }

                if ((!obstacle) || (obstacle > 3100 && obstacle < 3500) || (obstacle == 8)) // any obstacle, shift the item pointer
                {
                    // remove the item display from the map
                    // undraw_item(interface, *item);

                    // shift the item pointer
                    for (int w = 0; w < item->width; w++)
                    {
                        pthread_mutex_lock(&deplacement_mutex);
                        cellule *move_cell = rechercher(interface->tab_item[item->y + item->height - 1][item->x + w], item->id);
                        inserer(&interface->tab_item[item->y - 1][item->x + w], init_cellule(move_cell->item));
                        supprimer(&interface->tab_item[item->y + item->height - 1][item->x + w], rechercher(interface->tab_item[item->y + item->height - 1][item->x + w], item->id), DELETE_POINTER);
                        pthread_mutex_unlock(&deplacement_mutex);
                    }
                    item->y--;

                    if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle > 3100 && obstacle < 3500))
                    {
                        pass_door(interface, item, obstacle);
                        return -1;
                    }
                    else if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle == 8))
                    {
                        ncurses_stop();
                        printf("YOU WIN !\n");
                        printf("CHUCK IS HAPPY\n");
                        stop = 1;
                        exit(0);
                        return -1;
                    }
                }
            }
        }
        break;
    case 'q':
    case 'Q':
    case KEY_LEFT:
    case 0:
        if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10)
        {
            item->etat = 0;
        }
        if (item->x - 1 >= 0)
        {
            obstacle += is_obstacle(interface, item, item->y, item->x - 1, VERTICAL);

            if (item->id == ID_ROBOT && obstacle > 0)
            {
                deplacement = 1; // robot : turn to the right
            }
            else if (item->id == ID_PROBE && obstacle > 0)
            {
                deplacement = rand() % 4; // 0-1-2-3
            }
            if (!obstacle || (obstacle > 3100 && obstacle < 3500) || (obstacle == 8)) // any obstacle, shift the item pointer
            {
                // remove the item display from the map

                // undraw_item(interface, *item);

                for (int h = 0; h < item->height; h++)
                {
                    pthread_mutex_lock(&deplacement_mutex);
                    cellule *move_cell = rechercher(interface->tab_item[item->y + h][item->x + item->width - 1], item->id);
                    inserer(&interface->tab_item[item->y + h][item->x - 1], init_cellule(move_cell->item));
                    supprimer(&interface->tab_item[item->y + h][item->x + item->width - 1], rechercher(interface->tab_item[item->y + h][item->x + item->width - 1], item->id), DELETE_POINTER);
                    pthread_mutex_unlock(&deplacement_mutex);
                }
                item->x--;

                if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle > 3100 && obstacle < 3500))
                {
                    pass_door(interface, item, obstacle);
                    return -1;
                }
                else if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle == 8))
                {
                    ncurses_stop();
                    printf("YOU WIN !\n");
                    printf("CHUCK IS HAPPY\n");
                    stop = 1;
                    exit(0);
                    return -1;
                }
            }
        }
        // chute
        if (item->id != ID_ROBOT && item->id != ID_PROBE)
        {
            chute_player(interface, item);
        }
        break;
    case 's':
    case 'S':
    case KEY_DOWN:
    case 3:
        if ((item->id >= 40 && item->id < 50) || item->id == ID_PROBE)
        {
            if (item->y + item->height + 1 <= HEIGHT)
            {
                obstacle += is_obstacle(interface, item, item->y + item->height, item->x, HORIZONTAL);
                if (item->id == ID_PROBE && obstacle > 0)
                {
                    deplacement = rand() % 4; // 0-1-2-3
                }
                if (!obstacle || (obstacle > 3100 && obstacle < 3500) || (obstacle == 8)) // any obstacle, shift the item pointer
                {
                    // remove the item display from the map
                    // undraw_item(interface, *item);

                    for (int w = 0; w < item->width; w++)
                    {
                        pthread_mutex_lock(&deplacement_mutex);
                        cellule *move_cell = rechercher(interface->tab_item[item->y][item->x + w], item->id);
                        inserer(&interface->tab_item[item->y + item->height][item->x + w], init_cellule(move_cell->item));
                        supprimer(&interface->tab_item[item->y][item->x + w], rechercher(interface->tab_item[item->y][item->x + w], item->id), DELETE_POINTER);
                        pthread_mutex_unlock(&deplacement_mutex);
                    }
                    item->y++;
                    if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle > 3100 && obstacle < 3500))
                    {
                        pass_door(interface, item, obstacle);
                        return -1;
                    }
                    else if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle == 8))
                    {
                        ncurses_stop();
                        printf("YOU WIN !\n");
                        printf("CHUCK IS HAPPY\n");
                        stop = 1;
                        exit(0);
                        return -1;
                    }
                }
            }
        }
        break;
    case 'd':
    case 'D':
    case KEY_RIGHT:
    case 1:
        if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10)
        {
            item->etat = 1;
        }
        if (item->x + item->width + 1 <= WIDTH)
        {
            obstacle += is_obstacle(interface, item, item->y, item->x + item->width, VERTICAL);

            if (item->id == ID_ROBOT && obstacle > 0)
            {
                deplacement = 0;
            }
            else if (item->id == ID_PROBE && obstacle > 0)
            {
                deplacement = rand() % 4; // 0-1-2-3
            }
            if (obstacle == 0 || (obstacle > 3100 && obstacle < 3500) || (obstacle == 8)) // any obstacle, shift the item pointer
            {
                // remove the item display from the map
                // undraw_item(interface, *item);
                for (int h = 0; h < item->height; h++)
                {
                    pthread_mutex_lock(&deplacement_mutex);
                    cellule *move_cell = rechercher(interface->tab_item[item->y + h][item->x], item->id);
                    inserer(&interface->tab_item[item->y + h][item->x + item->width], init_cellule(move_cell->item));
                    supprimer(&interface->tab_item[item->y + h][item->x], rechercher(interface->tab_item[item->y + h][item->x], item->id), DELETE_POINTER);
                    pthread_mutex_unlock(&deplacement_mutex);
                }
                item->x++;

                if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle > 3100 && obstacle < 3500))
                {
                    pass_door(interface, item, obstacle);
                    return -1;
                }
                else if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10 && (obstacle == 8))
                {
                    ncurses_stop();
                    printf("YOU WIN !\n");
                    printf("CHUCK IS HAPPY\n");
                    stop = 1;
                    exit(0);
                    return -1;
                }
            }
            // chute
            if (item->id != ID_ROBOT && item->id != ID_PROBE)
            {
                chute_player(interface, item);
            }
        }
        break;
        // space bar
    case ' ':
        // change_interface(interface, 0);
        if (item->properties.player.nb_bomb > 0)
        {
            // pthread_mutex_lock(&interface->global_item.mutex);
            item->properties.player.nb_bomb--;
            nb_active_bomb++;
            // Create a new bomb item
            item_t *bomb = init_item(ID_ACTIVE_BOMB, item->x + 1, item->y + 3, 1, 1);
            // Add the bomb to the interface's tab_item array
            inserer(&interface->global_item, init_cellule(bomb));
            // pthread_mutex_unlock(&interface->global_item.mutex);
            item_t *p_bomb = bomb;
            inserer(&interface->tab_item[bomb->y][bomb->x], init_cellule(p_bomb)); // add pointer to the bomb
            bomb->thread = (pthread_t *)malloc(sizeof(pthread_t));
            data_thread *explosion_data = malloc(sizeof(data_thread));
            explosion_data->interface = interface;
            explosion_data->item = bomb;

            pthread_create(bomb->thread, NULL, routine_explosion, (void *)explosion_data);
        }
        break;
    default:
        break;
    }

    // PRINT PLAYER, ROBOT, PROBE
    // display_item(interface->win_level, *item, item->x, item->y);

    // if (nb_active_bomb > 0)
    // {
    //     for (int w = 0; w < item->width; w++)
    //     {
    //         cellule *loop_cell = interface->tab_item[item->y + item->height - 1][item->x + w].tete;
    //         while (loop_cell != NULL)
    //         {
    //             if (loop_cell->item->id == ID_ACTIVE_BOMB)
    //             {
    //                 // display_item(interface->win_level, *loop_cell->item, loop_cell->item->x, loop_cell->item->y);
    //                 break;
    //             }
    //             loop_cell = loop_cell->succ;
    //         }
    //     }
    // }

    if (item->id == ID_ROBOT || item->id == ID_PROBE)
    {
        if (deplacement != -1)
            return deplacement;
        else
            return c;
    }

    interface_debug(interface);
    return obstacle;
}

void chute_player(interface_t *interface, item_t *item)
{
    int nb_obstacle_under = 0, chute = 0;
    int find_ground = 0;
    for (int w = 0; w < item->width; w++)
    {
        if (interface->tab_item[item->y + item->height][item->x + w].tete != NULL)
        {
            cellule *last_cell = interface->tab_item[item->y + item->height][item->x + w].tete;
            while (last_cell->succ != NULL)
                last_cell = last_cell->succ;
            if (last_cell->item->id == ID_LADDER || last_cell->item->id == ID_BLOCK || last_cell->item->id == ID_TRAP)
            {
                find_ground = 1;
                if (last_cell->item->id == ID_TRAP)
                {
                    if (last_cell->item->etat == 0)
                        find_ground = 0;
                }
                break;
            }
        }
    }
    if (find_ground == 0)
    {
        do
        {
            nb_obstacle_under = is_obstacle(interface, item, item->y + item->height, item->x, HORIZONTAL);
            if (nb_obstacle_under == 0)
            {
                interface_game_update(interface, item, 's');
                chute++;
            }
        } while (nb_obstacle_under < 1);
        // par la suite faire qu'il prenne des dégats par rapport au nombre de case qu'il a chuté
        if (chute != 0)
        {
            if (item->properties.player.nb_life > 0)
            {
                item->properties.player.nb_life--;
                if (item->properties.player.nb_life == 0)
                {
                    ncurses_stop();
                    printf("GAME OVER ! NOOB\n");
                    stop = 1;
                    exit(0);
                }
            }
        }
    }
}

int is_obstacle(interface_t *interface, item_t *item, int new_y, int new_x, int check_side)
{
    // Item <=> player, robot, probe
    int obstacle = 0; // compteur d'obstacle
    int nb_cell = 0;
    int action = 0, enemy_cell = 0;

    if (check_side == HORIZONTAL)
        nb_cell = item->width;
    else
        nb_cell = item->height;

    for (int tour = 0; tour < 2; tour++) // tour 0: verif si action + aucun obstacle, tour 1: action
    {
        obstacle = 0;
        for (int n = 0; n < nb_cell; n++)
        {
            // #pthread_mutex_lock(&interface->tab_item[(check_side == HORIZONTAL) ? new_y : new_y + n][(check_side == HORIZONTAL) ? new_x + n : new_x].mutex);
            //  Si liste non vide
            cellule *new_cell = interface->tab_item[(check_side == HORIZONTAL) ? new_y : new_y + n][(check_side == HORIZONTAL) ? new_x + n : new_x].tete;
            if (new_cell != NULL)
            {
                int id_obstacle = new_cell->item->id;
                switch (id_obstacle)
                {
                // STOP
                case ID_BLOCK:
                    obstacle++;
                    break;

                    // PASS + ACTION
                case 11 ... 14: // ID_GATE
                    if (item->id == ID_ROBOT || item->id == ID_PROBE)
                    {
                        obstacle++;
                        break;
                    }
                    if (action)
                    {
                        // faire -> open_gate(interface,item);
                        if (item->properties.player.key[(id_obstacle % 10) - 1].color == 0)
                        {
                            obstacle++;
                        }
                    }
                    action = 1;
                    break;
                case 21 ... 24: // ID_KEY
                    if (item->id == ID_ROBOT || item->id == ID_PROBE)
                    {
                        break;
                    }
                    if (action)
                    {
                        // add_key(interface,item);
                        if (item->properties.player.nb_key <= 4)
                        {
                            if (item->properties.player.key[(id_obstacle % 10) - 1].color == 0)
                            {
                                item->properties.player.key[(id_obstacle % 10) - 1].color = id_obstacle;
                                item->properties.player.nb_key++;
                                // window_refresh(interface->win_tools); // #5
                            }
                        }
                        break;
                    }
                    action = 1;
                    break;
                case ID_EXIT:
                case 3101 ... 3499: // ID_DOOR
                    if (item->id > ID_PLAYER && item->id < ID_PLAYER + 10)
                    {
                        if (new_cell->item->x == item->x + 1 || new_cell->item->x == item->x - 1)
                        {
                            if (new_cell->item->y == item->y)
                            {
                                return id_obstacle;
                            }
                        }
                    }
                    break;
                case ID_ROBOT:
                case ID_PROBE:
                    if (item->id == ID_ROBOT || item->id == ID_PROBE)
                    {
                        obstacle++;
                        break;
                    }
                    if (new_cell->item->etat == 1)
                    {
                        if (action && tour > 0 && enemy_cell == 0)
                        {
                            // receive_damage(interface,item);
                            if (item->properties.player.nb_life > 0)
                            {
                                item->properties.player.nb_life--;
                                enemy_cell++;
                                if (item->properties.player.nb_life == 0)
                                {
                                    ncurses_stop();
                                    printf("GAME OVER ! NOOB\n");
                                    stop = 1;
                                    exit(0);
                                }
                            }
                        }
                        obstacle++;
                        action = 1;
                    }
                    // Si le robot ou la probe est actif : alors obstacle++ et dégats
                    // Sinon pas d'obstacle et pas de dégats
                    break;
                case ID_LIFE:
                    if (item->id == ID_ROBOT || item->id == ID_PROBE)
                    {
                        break;
                    }
                    if (action)
                    {
                        if (new_cell->item->etat == 1)
                        {
                            if (item->properties.player.nb_life < 5)
                            {
                                item->properties.player.nb_life = 5;
                                // send signal to thread life
                                pthread_cond_signal(&new_cell->item->cond);
                            }
                        }
                        break;
                    }
                    action = 1;
                    break;
                case ID_BOMB:
                    if (item->id == ID_ROBOT || item->id == ID_PROBE)
                    {
                        break;
                    }
                    if (action)
                    {
                        if (new_cell->item->etat == 1)
                        {

                            if (item->properties.player.nb_bomb < 3)
                            {
                                pthread_mutex_lock(&item->mutex);
                                item->properties.player.nb_bomb = 3;
                                // send signal to thread bomb
                                pthread_cond_signal(&new_cell->item->cond);
                                pthread_mutex_unlock(&item->mutex);
                            }
                        }
                        break;
                    }
                    action = 1;
                    break;
                case ID_TRAP:
                    if (item->id == ID_ROBOT || item->id == ID_PROBE)
                    {
                        obstacle++;
                        break;
                    }
                    if (new_cell->item->etat == 1)
                    {
                        obstacle++; // temporaire pour test
                        action = 1;
                    }
                    // Récup l'etat du trap
                    // Si actif : alors aucun obstacle + pas d'affichage
                    // Sinon obstacle++

                    break; // temporaire

                // PASS
                case 41 ... 44: // ID_PLAYER
                case ID_LADDER:
                case ID_START:
                    obstacle += ID_DELETE;
                    break;
                }
            }
            else
            {
                if (check_side == HORIZONTAL)
                {
                    // Si je monte
                    if (new_y < item->y)
                    {
                        // Si liste non vide
                        if (interface->tab_item[new_y + item->height][new_x + n].tete != NULL)
                        {
                            cellule *last_cell = interface->tab_item[new_y + item->height][new_x + n].tete;
                            while (last_cell->succ != NULL)
                                last_cell = last_cell->succ;

                            if (last_cell->item->id == ID_LADDER)
                                obstacle += ID_DELETE;
                            else if (last_cell->item->id != ID_PROBE)
                                obstacle++;
                        }
                        else
                        {
                            if (item->id != ID_PROBE)
                                obstacle++;
                        }
                    }
                }
                else
                {
                    obstacle += ID_DELETE;
                }
            }
            // #pthread_mutex_unlock(&interface->tab_item[(check_side == HORIZONTAL) ? new_y : new_y + n][(check_side == HORIZONTAL) ? new_x + n : new_x].mutex);
        }
    }
    return obstacle;
}

void interface_game_actions(int c)
{
    int mouseX, mouseY, posX, posY;
    interface_t *interface = tab_interface[current_level];
    /*
    Temporaire : utilisation du tab pour déplacer le premier player
    */
    pthread_mutex_lock(&tab_player.mutex);
    item_t *item = tab_player.tete->item; // récup le bon item donné en paramètre à interface_game_actions
    pthread_mutex_unlock(&tab_player.mutex);

    // Keyboard management
    interface_game_update(interface, item, c);
    window_mvprintw_col(interface->win_infos, 1, 0, WHITE, "Keyboard : %d", c);

    // Mouse management
    if ((c == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {

        if (window_getcoordinates(interface->win_level, mouseX, mouseY, &posX, &posY))
        {
            interface_debug_detail(interface, posX, posY);
        }
        else if (window_getcoordinates(interface->win_debug, mouseX, mouseY, &posX, &posY))
        {
            interface_debug_detail(interface, posX, posY);
        }
    }

    interface_hud_update(interface);
    // window_refresh(interface->win_tools); // #9
}

void interface_hud_update(interface_t *interface)
{
    // window_erase(interface->win_tools); // #1
    window_mvprintw(interface->win_tools, 1, 1, "Key");

    // Player HUD
    pthread_mutex_lock(&tab_player.mutex);
    if (tab_player.tete != NULL)
    {
        item_t *player = tab_player.tete->item;

        for (int i = 0; i < 4; i++)
        {
            // Display key
            if (player->properties.player.key[i].color != 0)
            {
                item_t *key = init_item(player->properties.player.key[i].color, 2 + (i * 2), 3, 1, 2);
                display_item(interface->win_tools, *key, key->x, key->y);
                free(key);
            }
        }

        window_mvprintw(interface->win_tools, 6, 1, "Lives");

        // Display life
        for (int i = 0; i < player->properties.player.nb_life; i++)
        {
            item_t life = *init_item(ID_LIFE, 2 + i, 8, 1, 1);
            display_item(interface->win_tools, life, life.x, life.y);
        }

        window_mvprintw(interface->win_tools, 10, 1, "Bombs");

        // Display bomb
        for (int i = 0; i < player->properties.player.nb_bomb; i++)
        {
            item_t bomb = *init_item(ID_BOMB, 2 + i, 12, 1, 1);
            display_item(interface->win_tools, bomb, bomb.x, bomb.y);
        }
    }
    pthread_mutex_unlock(&tab_player.mutex);

    window_mvprintw(interface->win_tools, 14, 1, "Levels");
    // window_printw(interface->win_tools, " \n  %02d \n", nb_level);

    // window_refresh(interface->win_tools); // #10
}

void init_player(interface_t *interface, int x, int y)
{
    // on initialise tab_player
    pthread_mutex_init(&tab_player.mutex, NULL);
    initialiser_liste(&tab_player);
    // on y place le player
    int id_player = 1;
    item_t *player = init_item(ID_PLAYER + id_player, x, y, 3, 4);
    pthread_mutex_lock(&tab_player.mutex);
    inserer(&tab_player, init_cellule(player));
    pthread_mutex_unlock(&tab_player.mutex);

    // place des pointeurs vers le player dans la map d'item
    item_t *p_player = player;
    for (int h = 0; h < player->height; h++)
    {
        for (int w = 0; w < player->width; w++)
        {
            inserer(&interface->tab_item[player->y + h][player->x + w], init_cellule(p_player));
        }
    }
}

void find_start(interface_t *interface)
{
    // parcourir interface->global_item si on trouve ID_START on initialise le player
    if (interface->global_item.tete != NULL)
    {
        cellule *itt_cell_global = interface->global_item.tete;
        while (itt_cell_global != NULL)
        {
            if (itt_cell_global->item->id == ID_START)
            {
                // on initialise et place le player à la position du start
                init_player(interface, itt_cell_global->item->x, itt_cell_global->item->y);
                return;
            }
            itt_cell_global = itt_cell_global->succ;
        }
    }
    // si on arrive ici, il n'y a pas de start
    perror("The level doesn't have a start !\n");
    ncurses_stop();
    exit(EXIT_FAILURE);
}

void draw_explosion(interface_t *interface, item_t *item)
{
    int x = item->x;
    int y = item->y;

    // on affiche les explosions
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int new_x = x + j;
            int new_y = y + i;

            if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT)
            {
                // #pthread_mutex_lock(&interface->tab_item[new_y][new_x].mutex);
                if (interface->tab_item[new_y][new_x].tete != NULL)
                {
                    if (interface->tab_item[new_y][new_x].tete->item->id == ID_ROBOT || interface->tab_item[new_y][new_x].tete->item->id == ID_PROBE)
                    {
                        interface->tab_item[new_y][new_x].tete->item->etat = 0;
                    }
                }
                if (interface->tab_item[new_y][new_x].tete == NULL)
                {
                    item_t *explosion = init_item(ID_EXPLOSION, new_x, new_y, 1, 1);
                    inserer(&interface->tab_item[new_y][new_x], init_cellule(explosion));
                }

                // #pthread_mutex_unlock(&interface->tab_item[new_y][new_x].mutex);
            }
        }
    }
}

void undraw_explosion(interface_t *interface, item_t *item)
{
    int x = item->x;
    int y = item->y;
    // #pthread_mutex_lock(&interface->tab_item[y][x].mutex);
    if (rechercher(interface->tab_item[y][x], ID_ACTIVE_BOMB) != NULL)
    {
        supprimer(&interface->tab_item[y][x], rechercher(interface->tab_item[y][x], ID_ACTIVE_BOMB), DELETE_ITEM);
    }
    // #pthread_mutex_unlock(&interface->tab_item[y][x].mutex);
    //  on affiche les explosions
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int new_x = x + j;
            int new_y = y + i;

            if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT)
            {
                // #pthread_mutex_lock(&interface->tab_item[new_y][new_x].mutex);

                if (interface->tab_item[new_y][new_x].tete != NULL)
                {
                    if (rechercher(interface->tab_item[new_y][new_x], ID_EXPLOSION) != NULL)
                    {
                        supprimer(&interface->tab_item[new_y][new_x], rechercher(interface->tab_item[new_y][new_x], ID_EXPLOSION), DELETE_ITEM);
                    }
                }

                // #pthread_mutex_unlock(&interface->tab_item[new_y][new_x].mutex);
            }
        }
    }
}

void *routine_display(void *arg)
{
    interface_t *interface = (interface_t *)arg;

    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        perror("pthread_detach");
        exit(EXIT_FAILURE);
    }
    stop = 0;

    while (1)
    {
        if (stop == 1)
        {
            pthread_cancel(pthread_self());
            pthread_testcancel();
        }

        // if (interface->current_interface != NULL)
        // {
        //     interface = interface->current_interface;
        // }

        interface = tab_interface[current_level];

        window_erase(interface->win_tools);
        window_erase(interface->win_level);
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (interface->tab_item[i][j].tete != NULL)
                {
                    display_item(interface->win_level, *interface->tab_item[i][j].tete->item, interface->tab_item[i][j].tete->item->x, interface->tab_item[i][j].tete->item->y);
                }
            }
        }

        // // pthread_mutex_lock(&interface->global_item.mutex);
        // cellule *itt_cell = interface->global_item.tete;
        // while (itt_cell != NULL)
        // {
        //     display_item(interface->win_level, *itt_cell->item, itt_cell->item->x, itt_cell->item->y);
        //     itt_cell = itt_cell->succ;
        // }
        // // pthread_mutex_unlock(&interface->global_item.mutex);

        // // pthread_mutex_lock(&interface->tab_player.mutex);
        // cellule *itt_cell_player = tab_player.tete;
        // while (itt_cell_player != NULL)
        // {
        //     display_item(interface->win_level, *itt_cell_player->item, itt_cell_player->item->x, itt_cell_player->item->y);
        //     itt_cell_player = itt_cell_player->succ;
        // }
        // // pthread_mutex_unlock(&interface->tab_player.mutex);

        // Met à jour le HUD
        interface_hud_update(interface);

        window_refresh(interface->win_infos);
        window_refresh(interface->win_level);
        window_refresh(interface->win_tools);

        interface_debug(interface);
        window_refresh(interface->win_debug);

        usleep(10000);
    }
    pthread_exit(NULL);
}

void *routine_robot(void *arg)
{
    data_thread robot_data = *(data_thread *)arg;
    interface_t *interface = robot_data.interface;
    item_t *item = robot_data.item;

    int deplacement = 1;

    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        perror("pthread_detach");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (stop == 1)
        {
            pthread_cancel(pthread_self());
            pthread_testcancel();
        }

        if (item->etat == 0)
        {
            sleep(6);
            item->etat = 1;
        }

        // Déplacement du robot
        deplacement = interface_game_update(interface, item, deplacement);
        usleep(200000);
    }
    pthread_exit(NULL);
}

void *routine_trap(void *arg)
{
    data_thread trap_data = *(data_thread *)arg;
    // interface_t *interface = trap_data.interface;
    item_t *item = trap_data.item;

    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        perror("pthread_detach");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (stop == 1)
        {
            pthread_cancel(pthread_self());
            pthread_testcancel();
        }

        // Déplacement du robot
        item->etat = (item->etat + 1) % 2;
        usleep(1000000);
    }
    pthread_exit(NULL);
}

void *routine_probe(void *arg)
{
    data_thread probe_data = *(data_thread *)arg;
    interface_t *interface = probe_data.interface;
    item_t *item = probe_data.item;
    int deplacement = 1;
    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        perror("pthread_detach");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        if (stop == 1)
        {
            pthread_cancel(pthread_self());
            pthread_testcancel();
        }
        if (item->etat == 0)
        {
            sleep(6);
            item->etat = 1;
        }

        deplacement = interface_game_update(interface, item, deplacement);

        usleep(100000);
    }
    pthread_exit(NULL);
}

void *routine_explosion(void *arg)
{
    data_thread explosion_data = *(data_thread *)arg;
    interface_t *interface = explosion_data.interface;
    item_t *item = explosion_data.item;

    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        perror("pthread_detach");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (stop == 1)
        {
            pthread_cancel(pthread_self());
            pthread_testcancel();
        }
        usleep(1500000);
        draw_explosion(interface, item);
        usleep(1500000);
        undraw_explosion(interface, item);
        pthread_cancel(pthread_self());
    }
    pthread_exit(NULL);
}

void *routine_bonus(void *arg)
{
    data_thread life_data = *(data_thread *)arg;
    interface_t *interface = life_data.interface;
    item_t *item = life_data.item;

    int status = pthread_detach(pthread_self());
    if (status != 0)
    {
        perror("pthread_detach");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (stop == 1)
        {
            pthread_cancel(pthread_self());
            pthread_testcancel();
        }

        // #pthread_mutex_lock(&interface->tab_item[item->y][item->x].mutex);
        item->etat = 1;
        pthread_cond_wait(&item->cond, &interface->tab_item[item->y][item->x].mutex);
        item->etat = 0;
        // #pthread_mutex_unlock(&interface->tab_item[item->y][item->x].mutex);
        sleep(5);
    }
    pthread_exit(NULL);
}
