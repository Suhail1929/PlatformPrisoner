#include <locale.h>
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
#include "item.h"
#include "liste.h"
#include "interface.h"

int main(int argc, char *argv[])
{

    // Check arguments
    if (argc == 3)
    {
        // Load a file
        // argv[1] : ip
        // argv[2] : port
    }
    else
    {
        // Bad arguments
        fprintf(stderr, "Specify an ip and a port:\n\t%s <ip> <port>\n", argv[0]);
        fprintf(stderr, "<ip> : ip of the server\n<port> : port of the server\n");
        exit(EXIT_FAILURE);
    }
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

    // int adresseIPServeur = atoi(argv[1]);
    // int portUDPServeur = atoi(argv[2]);

    char path[100];
    sprintf(path, "bin/%s.bin", room_name); // gestion d'erreur ?

    // interface_t *interface;
    interface_t **all_interface = NULL;

    // ncurses initialisation
    setlocale(LC_ALL, "");
    ncurses_init();
    ncurses_init_mouse();
    ncurses_colors();
    palette();
    clear();
    refresh();

    // Creation de l'interface
    int nb_interface = 1;
    all_interface = interface_create_game(path, &nb_interface);
    int ch;

    while ((ch = getch()) != 27)
    {
        // on donne au player l'interface dans lequel il se trouve, pour effectuer les actions
        // pour le teste : 0
        interface_game_actions(all_interface[0], ch);
    }
    // Quitter le mode d'affichage ncurses
    ncurses_stop();

    // // supprimer l'interface
    for (int i = 0; i < nb_interface; i++)
    {
        interface_delete(&all_interface[i]);
    }

    free(room_name);
    return EXIT_SUCCESS;
}