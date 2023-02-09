// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <fcntl.h>
// #include <unistd.h>

// #include "Table_Adresse/bloc.h"
// #define MAX_SIZE 2
// #define HOLE 1
// #define ADDRESS 0

// int main()
// {
//     // Création des tables d'adresses et de vide
//     data_t *addressTable;
//     int numAddressEntries = 0;
//     creerTableAdresse(&addressTable, MAX_SIZE, ADDRESS);

//     data_t *holeTable;
//     int numHoleEntries = 0;
//     creerTableAdresse(&holeTable, MAX_SIZE, HOLE);

//     // Création du fichier
//     int fd = creerFichier("data.bin");
//     ecrireTableDansFichier(fd, addressTable, MAX_SIZE, holeTable, numHoleEntries);

//     // Ajout d'une chaîne dans le fichier
//     char buffer1[100];
//     strcpy(buffer1, "Bonjour tout le monde !");
//     off_t position1 = lseek(fd, 0, SEEK_END);
//     ajouterEntreeAdresse(addressTable, &numAddressEntries, position1, strlen(buffer1));
//     ecrireChaine(fd, addressTable, numAddressEntries - 1, buffer1, strlen(buffer1));

//     char buffer2[100];
//     strcpy(buffer2, "Comment ça va ?");
//     off_t position2 = lseek(fd, 0, SEEK_END);
//     ajouterEntreeAdresse(addressTable, &numAddressEntries, position2, strlen(buffer2));
//     ecrireChaine(fd, addressTable, numAddressEntries - 1, buffer2, strlen(buffer2));

//     // Affichage des tables
//     affichierTableAdresse(addressTable, numAddressEntries, fd);

//     // Suppression d'une entrée de la table d'adresses
//     int entryToDelete = 0;
//     supprimerEntreeAdresse(addressTable, &numAddressEntries, entryToDelete);

//     // Affichage de la table mise à jour
//     printf("\nTable d'adresses après suppression de l'entrée %d:\n", entryToDelete);
//     affichierTableAdresse(addressTable, numAddressEntries, fd);

//     // Ecriture des tables dans le fichier
//     ecrireTableDansFichier(fd, addressTable, numAddressEntries, holeTable, numHoleEntries);

//     // Fermeture du fichier
//     close(fd);

//     return 0;
// }