// author : HAYAT RAHIM & MTARFI Souhail
#ifndef __DATA_TABLE_H__
#define __DATA_TABLE_H__

#define NB_LEVEL 5
#define HEIGHT 20
#define WIDTH 60

int nb_level;

/*
 * Struct representing a level
 * hole : 0 = address, 1 = hole
 * tab : the level map
 * id : the level id
 */

typedef struct level_t level_t;
struct level_t
{
    int hole; // 0 = address, 1 = hole
    int tab[HEIGHT][WIDTH];
    int id;
};

/*
 * Struct representing a bloc
 * nb_available_entry : the number of available entry in the bloc
 * level : the level array
 * bloc_next : the next bloc
 */

typedef struct bloc_t bloc_t;
struct bloc_t
{
    int nb_available_entry;
    off_t level[NB_LEVEL];
    off_t bloc_next;
};
/*
 *  Function that load the level
 *  @param fd : le descripteur de fichier
 */
void load_nb_level(int fd);
/*
 * Function that open a file
 */

int openFile(char *fileName);
/*
 *Function that close a file
 */
void closeFile(int fd);
/*
 * Function that init a level
 */
level_t *initLevel();
/*
 * Function that init a bloc
 */

bloc_t *initBloc();
/*
 * Function that save a level in a file and save the offset of the level in the bloc
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param level : the level
 */
void addLevel(int fd, bloc_t *Bloc, level_t *level);
/*
 * Function that update a level
 * @param fd : the file descriptor
 * @param offset : the offset of the level
 * @param level : the level
 */
void updateLevel(int fd, off_t offset, level_t *level);
/*
 * Function that save a bloc in the file and return the offset
 * @param fd : the file descriptor
 * @param bloc : the bloc
 */
off_t saveBloc(int fd, bloc_t *Bloc);

/*
 * Function that update a bloc
 * @param fd : the file descriptor
 * @param offset : the offset of the bloc
 * @param bloc : the bloc
 */
void updateBloc(int fd, off_t offset, bloc_t *Bloc);
/*
 * Function that load a bloc
 * @param fd : the file descriptor
 * @param offset : the offset of the bloc
 */
bloc_t *loadBloc(int fd, off_t offset);
/*
 * Function that find a level in a bloc
 * @param fd : the file descriptor
 * @param Bloc : the bloc
 * @param id : the id of the level
 */
off_t findLevel(int fd, bloc_t *Bloc, int id);
/*
 * Function that delete a level
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param id : the id of the level
 */
void deleteLevel(int fd, bloc_t *Bloc, int id);
/*
 * Function that display a bloc
 * @param fd : the file descriptor
 * @param bloc : the bloc
 */
void void displayBloc(int fd, bloc_t *bloc);
/*
 * Function that display a level
 * @param level : the level
 */
void displayLevel(level_t *level);
/*
 * Function that load a level
 * @param fd : the file descriptor
 * @param offset : the offset of the level
 */
level_t *loadLevel(int fd, off_t offset);
/*
 * Function that find a level in a bloc
 * @param fd : the file descriptor
 * @param Bloc : the bloc
 * @param id : the id of the level
 */
level_t *loadLevelById(int fd, bloc_t *Bloc, int id);

#endif // __DATA_TABLE_H__
