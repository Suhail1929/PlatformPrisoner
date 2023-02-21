#ifndef __DATA_TABLE_H__
#define __DATA_TABLE_H__

#define BLOC_SIZE 5

typedef struct cell_t cell_t;
struct cell_t
{
    int hole; // 0 = address, 1 = hole
    off_t data_position;
    int data_size;
};

typedef struct data_t data_t;
struct data_t
{
    cell_t bloc[BLOC_SIZE];
    int total_entry;
    int nb_available_entry;
    off_t next;
};

int creerFichier(char *fileName);
void create_data_block(data_t *block);
void delete_all_tables(int fd, data_t *first_table);
void print_data(int fd, data_t *table);

#endif // __DATA_TABLE_H__
