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
#include "liste.h"
#include "interface.h"
#include "entity.h"

#define HORIZONTAL 1
#define VERTICAL 2

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

    // fenetre debug
    result->win_debug = window_create(80, 0, 62, 22, "DEBUG", FALSE);
    window_refresh(result->win_debug);

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

    interface_hud_update(result);
    window_refresh(result->win_tools);

    return result;
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
            initialiser_liste(&interface->tab_item[i][j]);
        }
    }
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
                inserer(&interface->global_item, init_cellule(item)); // ajout pour la tête

                // pointer to the item
                item_t *p_item = item;

                for (int h = 0; h < item->height; h++)
                {
                    for (int w = 0; w < item->width; w++)
                    {
                        inserer(&interface->tab_item[item->y + h][item->x + w], init_cellule(p_item)); // add pointer to the item in the map
                    }
                }
            } // already item placed, skip the bloc width
            else
            {
                // j += bloc_width - 1; // A débugger !
            }
        }
    }

    // find the start and init the player
    find_start(interface);

    // display the game interface
    if (interface->global_item.tete != NULL)
    {
        cellule *itt_cell_global = interface->global_item.tete;
        while (itt_cell_global != NULL)
        {
            display_item(interface->win_level, *(interface->tab_item[itt_cell_global->item->y][itt_cell_global->item->x].tete->item), itt_cell_global->item->x, itt_cell_global->item->y);
            itt_cell_global = itt_cell_global->succ;
        }
    }
}

void undraw_item(interface_t *interface, item_t item)
{
    // check the list, and draw the item if it's the head
    item_t *item_behind = NULL;
    for (int h = 0; h < item.height; h++)
    {
        for (int w = 0; w < item.width; w++)
        {
            if (interface->tab_item[item.y + h][item.x + w].tete != NULL)
            {
                // METHODE 1
                if (interface->tab_item[item.y + h][item.x + w].tete->succ != NULL)
                {
                    item_behind = interface->tab_item[item.y + h][item.x + w].tete->succ->item;
                    if (item_behind != NULL)
                    {
                        display_item(interface->win_level, *item_behind, item_behind->x, item_behind->y);
                    }
                }
                else
                {
                    window_mvaddch_col(interface->win_level, item.y + h, item.x + w, BLACK, ' ');
                }
            }
            else
            {
                window_mvaddch_col(interface->win_level, item.y + h, item.x + w, BLACK, ' ');
            }
        }
    }
}

void interface_game_update(interface_t *interface, int c)
{
    /*
    Temporaire : utilisation du tab pour déplacer le premier player
    */
    item_t *item = interface->tab_player.tete->item;

    // For bottom and right, we must verify if the player bloc is not out of the map (because the head is on top left)
    int obstacle = 0;
    switch (c)
    {
    case 'z':
    case KEY_UP:
        if (item->id >= 40 && item->id < 50)
        {

            if (item->y - 1 >= 0)
            {
                obstacle += is_obstacle(interface, item, item->y - 1, item->x, HORIZONTAL);
                if (!obstacle) // any obstacle, shift the item pointer
                {
                    // remove the item display from the map
                    undraw_item(interface, *item);

                    // shift the item pointer
                    for (int w = 0; w < item->width; w++)
                    {
                        cellule *move_cell = rechercher(interface->tab_item[item->y + item->height - 1][item->x + w], item->id);
                        inserer(&interface->tab_item[item->y - 1][item->x + w], init_cellule(move_cell->item));
                        supprimer(&interface->tab_item[item->y + item->height - 1][item->x + w], rechercher(interface->tab_item[item->y + item->height - 1][item->x + w], item->id), DELETE_POINTER);
                    }
                    item->y--;
                }
            }
        }
        break;
    case 'q':
    case KEY_LEFT:

        if (item->x - 1 >= 0)
        {
            obstacle += is_obstacle(interface, item, item->y, item->x - 1, VERTICAL);
            if (!obstacle) // any obstacle, shift the item pointer
            {
                // remove the item display from the map
                undraw_item(interface, *item);

                for (int h = 0; h < item->height; h++)
                {
                    cellule *move_cell = rechercher(interface->tab_item[item->y + h][item->x + item->width - 1], item->id);
                    inserer(&interface->tab_item[item->y + h][item->x - 1], init_cellule(move_cell->item));
                    supprimer(&interface->tab_item[item->y + h][item->x + item->width - 1], rechercher(interface->tab_item[item->y + h][item->x + item->width - 1], item->id), DELETE_POINTER);
                }
                item->x--;
            }
        }
        // chute
        chute_player(interface, item);
        break;
    case 's':
    case KEY_DOWN:
        if (item->id >= 40 && item->id < 50)
        {
            if (item->y + item->height + 1 <= HEIGHT)
            {
                obstacle += is_obstacle(interface, item, item->y + item->height, item->x, HORIZONTAL);
                if (!obstacle) // any obstacle, shift the item pointer
                {
                    // remove the item display from the map
                    undraw_item(interface, *item);

                    for (int w = 0; w < item->width; w++)
                    {
                        cellule *move_cell = rechercher(interface->tab_item[item->y][item->x + w], item->id);
                        inserer(&interface->tab_item[item->y + item->height][item->x + w], init_cellule(move_cell->item));
                        supprimer(&interface->tab_item[item->y][item->x + w], rechercher(interface->tab_item[item->y][item->x + w], item->id), DELETE_POINTER);
                    }
                    item->y++;
                }
            }
        }
        break;
    case 'd':
    case KEY_RIGHT:
        if (item->x + item->width + 1 <= WIDTH)
        {
            obstacle += is_obstacle(interface, item, item->y, item->x + item->width, VERTICAL);
            if (obstacle == 0) // any obstacle, shift the item pointer
            {
                // remove the item display from the map
                undraw_item(interface, *item);

                // METHODE 1
                for (int h = 0; h < item->height; h++)
                {
                    cellule *move_cell = rechercher(interface->tab_item[item->y + h][item->x], item->id);
                    inserer(&interface->tab_item[item->y + h][item->x + item->width], init_cellule(move_cell->item));
                    supprimer(&interface->tab_item[item->y + h][item->x], rechercher(interface->tab_item[item->y + h][item->x], item->id), DELETE_POINTER);
                }
                item->x++;
            }
            // chute
            chute_player(interface, item);
        }
        break;
    // space bar
    case ' ':
        if (item->properties.player.nb_bomb > 0)
        {
            item->properties.player.nb_bomb--;
        }
        break;
    default:
        break;
    }

    // PRINT PLAYER
    display_item(interface->win_level, *item, item->x, item->y);

    interface_debug(interface, item->x, item->y);
    return;
}

void chute_player(interface_t *interface, item_t *item)
{
    int nb_obstacle_under = 0, chute = 0;
    int find_ladder = 0;
    for (int w = 0; w < item->width; w++)
    {
        if (interface->tab_item[item->y + item->height][item->x + w].tete != NULL)
        {
            cellule *last_cell = interface->tab_item[item->y + item->height][item->x + w].tete;
            while (last_cell->succ != NULL)
                last_cell = last_cell->succ;
            if (last_cell->item->id == ID_LADDER || last_cell->item->id == ID_BLOCK)
            {
                find_ladder = 1;
                break;
            }
        }
    }
    if (find_ladder == 0)
    {
        do
        {
            nb_obstacle_under = is_obstacle(interface, item, item->y + item->height, item->x, HORIZONTAL);
            if (nb_obstacle_under == 0)
            {
                interface_game_update(interface, 's');
                chute++;
            }
        } while (nb_obstacle_under != 3);
        // par la suite faire qu'il prenne des dégats par rapport au nombre de case qu'il a chuté
        if (chute != 0)
        {
            if (item->properties.player.nb_life > 0)
                item->properties.player.nb_life--;
        }
    }
}

int is_obstacle(interface_t *interface, item_t *item, int new_y, int new_x, int check_side)
{
    // Item <=> player, robot, probe
    int obstacle = 0; // compteur d'obstacle
    int nb_cell = 0;
    int action = 0;

    if (check_side == HORIZONTAL)
        nb_cell = item->width;
    else
        nb_cell = item->height;

    for (int tour = 0; tour < 2; tour++) // tour 0: verif si action + aucun obstacle, tour 1: action
    {
        obstacle = 0;
        for (int n = 0; n < nb_cell; n++)
        {
            // Si liste non vide
            if (interface->tab_item[(check_side == HORIZONTAL) ? new_y : new_y + n][(check_side == HORIZONTAL) ? new_x + n : new_x].tete != NULL)
            {
                int id_obstacle = interface->tab_item[(check_side == HORIZONTAL) ? new_y : new_y + n][(check_side == HORIZONTAL) ? new_x + n : new_x].tete->item->id;
                switch (id_obstacle)
                {
                // STOP
                case ID_BLOCK:
                    obstacle++;
                    break;

                    // PASS + ACTION
                case 11 ... 14: // ID_GATE
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
                    if (action)
                    {
                        // add_key(interface,item);
                        if (item->properties.player.nb_key <= 4)
                        {
                            if (item->properties.player.key[(id_obstacle % 10) - 1].color == 0)
                            {
                                item->properties.player.key[(id_obstacle % 10) - 1].color = id_obstacle;
                                item->properties.player.nb_key++;
                                window_refresh(interface->win_tools);
                            }
                        }
                        break;
                    }
                    action = 1;
                    break;
                case 3101 ... 3199: // ID_DOOR
                    break;
                case ID_ROBOT:
                case ID_PROBE:
                    if (action)
                    {
                        // Si le robot ou la probe est actif : alors obstacle++ et dégats
                        // Sinon pas d'obstacle et pas de dégats
                        // Donc récupérer l'état du robot ou du probe touché

                        // receive_damage(interface,item); , problème : enlève toute la vie du joueur
                        if (item->properties.player.nb_life > 0)
                        {
                            item->properties.player.nb_life--;
                            window_refresh(interface->win_tools);
                        }
                    }
                    action = 1;
                    break;
                case ID_LIFE:
                    if (action)
                    {
                        // faire -> add_life(interface,item);
                        item->properties.player.nb_life = 5;
                        // Envoyer un signal au thread life pour qu'il disparaisse un moment
                        window_refresh(interface->win_tools);
                        break;
                    }
                    action = 1;
                    break;
                case ID_BOMB:
                    if (action)
                    {
                        // faire -> add_bombe(interface,item);
                        item->properties.player.nb_bomb = 3;
                        // Envoyer un signal au thread bombe pour qu'il disparaisse un moment
                        window_refresh(interface->win_tools);
                        break;
                    }
                    action = 1;
                    break;
                case ID_TRAP:
                    if (action)
                    {
                        // Récup l'etat du trap
                        // Si actif : alors aucun obstacle + pas d'affichage
                        // Sinon obstacle++
                    }
                    obstacle++; // temporaire pour test
                    action = 1;
                    break; // temporaire

                // PASS
                case 41 ... 44: // ID_PLAYER
                case ID_LADDER:
                case ID_START:
                case ID_EXIT:
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
                            else
                                obstacle++;
                        }
                        else
                        {
                            obstacle++;
                        }
                    }
                }
                else
                {
                    obstacle += ID_DELETE;
                }
            }
        }
    }
    return obstacle;
}

void interface_game_actions(interface_t *interface, int c)
{
    int mouseX, mouseY, posX, posY;

    // Keyboard management
    interface_game_update(interface, c);
    window_mvprintw_col(interface->win_infos, 1, 0, WHITE, "Keyboard : %d", c);

    // Mouse management
    if ((c == KEY_MOUSE) && (mouse_getpos(&mouseX, &mouseY) == OK))
    {

        if (window_getcoordinates(interface->win_level, mouseX, mouseY, &posX, &posY))
        {
            interface_debug(interface, posX, posY);
        }
        else if (window_getcoordinates(interface->win_debug, mouseX, mouseY, &posX, &posY))
        {
            interface_debug(interface, posX, posY);
        }
    }

    interface_hud_update(interface);
    window_refresh(interface->win_tools);
}

void interface_hud_update(interface_t *interface)
{
    window_erase(interface->win_tools);
    window_mvprintw(interface->win_tools, 1, 1, "Key");

    // temporaire : player
    if (interface->tab_player.tete != NULL)
    {
        item_t *player = interface->tab_player.tete->item;

        for (int i = 0; i < 4; i++)
        {
            // afficher les clé du player
            if (player->properties.player.key[i].color != 0)
            {
                item_t *key = init_item(player->properties.player.key[i].color, 2 + (i * 2), 3, 1, 2);
                display_item(interface->win_tools, *key, key->x, key->y);
                free(key);
            }
        }

        window_mvprintw(interface->win_tools, 6, 1, "Lives");

        // temporaire : pour afficher les vies
        for (int i = 0; i < player->properties.player.nb_life; i++)
        {
            item_t life = *init_item(ID_LIFE, 2 + i, 8, 1, 1);
            display_item(interface->win_tools, life, life.x, life.y);
        }

        window_mvprintw(interface->win_tools, 10, 1, "Bombs");

        // temporaire : pour afficher les bombes
        for (int i = 0; i < player->properties.player.nb_bomb; i++)
        {
            item_t bomb = *init_item(ID_BOMB, 2 + i, 12, 1, 1);
            display_item(interface->win_tools, bomb, bomb.x, bomb.y);
        }
    }

    window_mvprintw(interface->win_tools, 14, 1, "Levels");
    // window_printw(interface->win_tools, " \n  %02d \n", nb_level);

    window_refresh(interface->win_tools);
}

void init_player(interface_t *interface, int x, int y)
{
    // on initialise tab_player
    initialiser_liste(&interface->tab_player);
    // on y place le player
    int id_player = 1;
    item_t *player = init_item(ID_PLAYER + id_player, x, y, 3, 4);
    inserer(&interface->tab_player, init_cellule(player));

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