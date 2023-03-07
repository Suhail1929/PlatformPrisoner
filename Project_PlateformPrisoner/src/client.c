#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

#include "fonction.h"
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "interface.h"

void creer_partie();
char *afficher_salons();

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        fprintf(stderr, "erreur 3 argument attendu\n");
        return EXIT_FAILURE;
    }

    system("clear");
    char nom_joueur[50];
    int choix = 0;
    printf("Bonjour, quel est votre pseudo ? ");
    scanf("%s", nom_joueur);
    printf("\nBienvenue %s !\n\n", nom_joueur);
    printf("Appuyez sur Entrée pour continuer.\n");
    while (getchar() != '\n')
        ;
    getchar(); // attendre une touche

    char *room_name = NULL;
    do
    {
        system("clear");
        printf("1 : Créer une partie\n2 : Afficher la liste des salons de jeu dispo.\n3 : Quitter\n");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            creer_partie();
            break;
        case 2:
            room_name = afficher_salons();
            if (room_name != NULL)
                choix = 3;
            break;
        case 3:
            printf("Au revoir %s !\n", nom_joueur);
            break;
        case 4:
            // cas où on a rejoin ou créé une partie
            break;
        default:
            system("clear");
            printf("Veuillez entrer une option valide (1, 2 ou 3).\n");
            printf("Appuyez sur Entrée pour continuer.\n");
            while (getchar() != '\n')
                ;
            getchar(); // attendre une touche
            break;
        }
    } while (choix != 3);

    printf("(main) MONDE SELECTIONNE = %s\n", room_name);
    free(room_name);

    // int adresseIPServeur = atoi(argv[1]);
    // int portUDPServeur = atoi(argv[2]);

    interface_t *interface;

    ncurses_init();
    ncurses_colors();
    palette();
    ncurses_init_mouse();

    // Creation de l'interface
    interface = interface_create_game();

    int ch;
    while ((ch = getch()) != 27)
    {
        interface_hud_actions(interface, ch);
    }
    // Quitter le mode d'affichage ncurses
    ncurses_stop();

    // // supprimer l'interface
    interface_delete_game(&interface);
    return 0;
}

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
        printf("Voulez-vous rejoindre un salon ? (y/n) ");
        char choix;
        scanf(" %c", &choix);
        if (choix == 'y' || choix == 'Y')
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
    }
    printf("Appuyez sur Entrée pour continuer.\n");
    while (getchar() != '\n')
        ;
    getchar(); // attendre une touche
    return NULL;
}