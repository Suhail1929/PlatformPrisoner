#include <stdio.h>
#include <unistd.h>
#include "data_table.h"

int main(int argc, char *argv[])
{
    data_t block;
    int fd = creerFichier("file.bin");
    create_data_block(&block);
    print_data(fd, &block);
    delete_all_tables(fd, &block);
    print_data(fd, &block);
    close(fd);
    return 0;
}