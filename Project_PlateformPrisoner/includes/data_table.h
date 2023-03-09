// author : HAYAT RAHIM & MTARFI Souhail
#ifndef __DATA_TABLE_H__
#define __DATA_TABLE_H__

#define NB_LEVEL 5
#define HEIGHT 20
#define WIDTH 60

int nb_level;

/**
 * @brief Structure that represent a level
 * @param hole : 0 = address, 1 = hole
 * @param tab : the level table
 * @param id : the level id
 */
typedef struct level_t level_t;
struct level_t
{
    int hole; // 0 = address, 1 = hole
    int tab[HEIGHT][WIDTH];
    int id;
};

/**
 * @brief Structure that represent a bloc
 * @param nb_available_entry : the number of available entry
 * @param level : the level table (offset)
 * @param bloc_next : the next bloc (offset)
 */
typedef struct bloc_t bloc_t;
struct bloc_t
{
    int nb_available_entry;
    off_t level[NB_LEVEL];
    off_t bloc_next;
};

/**
 * @brief Function that load the number of level
 * @param id : the level id
 */
void load_nb_level(int fd);

/**
 * @brief Function to open a file
 * @param fileName : the file name
 * @return the file descriptor
 */
int openFile(char *fileName);

/**
 * @brief Function to close a file
 * @param fd : the file descriptor
 */
void closeFile(int fd);

/**
 * @brief Function that init a level
 * @return the level structure
 */
level_t *initLevel();

/**
 * @brief Function that init a bloc
 * @return the bloc structure
 */
bloc_t *initBloc();

/**
 * @brief Function that add the level offset in a bloc and save the level in a file
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param level : the level
 */
void addLevel(int fd, bloc_t *Bloc, level_t *level);

/**
 * @brief Function that update a level in the file
 * @param fd : the file descriptor
 * @param offset : the offset of the level
 * @param level : the level
 */
void updateLevel(int fd, off_t offset, level_t *level);

/**
 * @brief Function that save a bloc in the file
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @return the offset of the bloc
 */
off_t saveBloc(int fd, bloc_t *Bloc);

/**
 * @brief Function that update a bloc in the file
 * @param fd : the file descriptor
 * @param offset : the offset of the bloc
 * @param bloc : the bloc
 */
void updateBloc(int fd, off_t offset, bloc_t *Bloc);

/**
 * @brief Function that load a bloc from the file
 * @param fd : the file descriptor
 * @param offset : the offset of the bloc
 * @return the bloc
 */
bloc_t *loadBloc(int fd, off_t offset);

/**
 * @brief Function that load a level by id from the file
 * @param fd : the file descriptor
 * @param Bloc : the bloc
 * @param id : the id of the level
 * @return the level
 */
off_t findLevel(int fd, bloc_t *Bloc, int id);

/**
 * @brief Function that delete a level (set the hole to 1)
 * @param fd : the file descriptor
 * @param bloc : the bloc
 * @param id : the id of the level
 */
void deleteLevel(int fd, bloc_t *Bloc, int id);

/**
 * @brief Function that display a bloc
 * @param fd : the file descriptor
 * @param bloc : the bloc
 */
void displayBloc(int fd, bloc_t *bloc);

/**
 * @brief Function that display a level
 * @param level : the level
 */
void displayLevel(level_t *level);

/**
 * @brief Function that load a level from the file by offset
 * @param fd : the file descriptor
 * @param offset : the offset of the level
 * @return the level structure
 */
level_t *loadLevel(int fd, off_t offset);

/**
 * @brief Function that load a level by id from the file by id
 * @param fd : the file descriptor
 * @param Bloc : the bloc
 * @param id : the id of the level
 * @return the level structure
 */
level_t *loadLevelById(int fd, bloc_t *Bloc, int id);

int loadOrCreatefromFile(const char *path, bloc_t *bloc, level_t *level, int createIfNotExists);

#endif // __DATA_TABLE_H__
