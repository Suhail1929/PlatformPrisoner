#include <ncurses.h>
#include <stdlib.h>
#include "fonction.h"
#include "couleur.h"
#include "window.h"
#include "data_table.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc == 2)
    {
        // Load a file
        if (argv[1] == NULL)
        {
            fprintf(stderr, "File '%s' does'nt exist.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Bad arguments
        fprintf(stderr, "Load a file:\n\t%s <file>\n\t\tfile: file to load if exist\n", argv[0]);
        fprintf(stderr, "Create a new file:\n\t%s <file>\n\t\tfile: file to create if not exist\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char path[100];
    sprintf(path, "bin/%s.bin", argv[1]); // gestion d'erreur ?
    interface_t *interface;
    int fd = openFile(path);
    bloc_t *bloc = loadBloc(fd, 0);
    if (bloc == NULL)
    {
        printf("Creation du Bloc\n");
        bloc = initBloc();
        saveBloc(fd, bloc);
        level_t *level0 = initLevel();
        addLevel(fd, bloc, level0);
        updateBloc(fd, 0, bloc);
    }
    level_t *level0 = loadLevelById(fd, bloc, 0);
    if (level0 == NULL)
    {
        level0 = initLevel();
    }

    ncurses_init();
    ncurses_colors();
    palette();
    ncurses_init_mouse();

    wbkgd(stdscr, COLOR_PAIR(0));

    refresh();

    // Creation of the interface
    interface = interface_create(level0);

    int ch;
    while ((ch = getch()) != 27)
    {
        interface_actions(fd, level0, interface, ch);
    }

    ncurses_stop();
    // display the bloc
    bloc = loadBloc(fd, 0);
    displayBloc(fd, bloc);
    // supprimer l'interface
    interface_delete(&interface);

    closeFile(fd);
    free(bloc);
    free(level0);

    return EXIT_SUCCESS;
}
