#ifndef __DATA_TABLE_H__
#define __DATA_TABLE_H__

#define NB_LEVEL 5
#define HEIGHT 20
#define WIDTH 60

int nb_level;

typedef struct level_t level_t;
struct level_t
{
    int hole; // 0 = address, 1 = hole
    int tab[HEIGHT][WIDTH];
    int id;
};

typedef struct bloc_t bloc_t;
struct bloc_t
{
    int nb_available_entry;
    off_t level[NB_LEVEL];
    off_t bloc_next;
};

// tableau + hole enregistré dans le fichier mais lors de la récup, le tab n'est pas stocké on le place direct dans l'editeur
// on stock juste hole.

void save_nb_level(int fd);
void load_nb_level(int fd);
int openFile(char *fileName);
void closeFile(int fd);
level_t *initLevel();
bloc_t *initBloc();
void addLevel(int fd, bloc_t *Bloc, level_t *level);
void updateLevel(int fd, off_t offset, level_t *level);
off_t saveBloc(int fd, bloc_t *Bloc);
void updateBloc(int fd, off_t offset, bloc_t *Bloc);
bloc_t *loadBloc(int fd, off_t offset);
off_t findLevel(int fd, bloc_t *Bloc, int id);
void deleteLevel(int fd, bloc_t *Bloc, int id);
void displayBloc(int fd, bloc_t *bloc);
void displayLevel(level_t *level);
level_t *loadLevel(int fd, off_t offset);
level_t *loadLevelById(int fd, bloc_t *Bloc, int id);

#endif // __DATA_TABLE_H__
