// # Author: HAYAT Rahim && MTARFI Souhail

#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct data_t data_t;
struct data_t
{
    int hole; // 0 = address, 1 = hole
    off_t data_position;
    int data_size;
}; // address_t ou hole_t

// ---------- Table d'adresses ----------

// Créer une table d'adresses
void creerTableAdresse(data_t **table, int numEntries, int isHole);

// Supprimer une table d'adresses
void supprimerTableAdresse(data_t **table);

// ---------- Entrées de la table d'adresses ----------

// Lire une entrée de la table d'adresses
void lireEntreeAdresse(data_t *table, int entry);

// Écrire une entrée dans la table d'adresses
void ecrireEntreeAdresse(data_t *table, int entry, off_t position, int size);

// Afficher toutes les entrées de la table d'adresses
void affichierTableAdresse(data_t *table, int numEntries, int fd);

// Ajouter une entrée à la table d'adresses
void ajouterEntreeAdresse(data_t *table, int *numEntries, off_t position, int size);

// Supprimer une entrée de la table d'adresses
void supprimerEntreeAdresse(data_t *table, int *numEntries, int entry);

// ---------- Fichier & Tables ----------

// Lire une chaîne de caractères à partir d'une entrée de la table d'adresses
void lireChaine(int fd, data_t *table, int entry, char *buffer, int bufferSize);

// Écrire une chaîne de caractères dans une entrée de la table d'adresses
void ecrireChaine(int fd, data_t *table, int entry, char *buffer, int bufferSize);

// Créer un fichier
int creerFichier(char *fileName);

// Écrire les tables dans un fichier
void ecrireTableDansFichier(int fd, data_t *addressTable, int numAddressEntries, data_t *holeTable, int numHoleEntries);

// Lire les tables d'un fichier
void lireTableDepuisFichier(int fd, data_t **addressTable, int *numAddressEntries, data_t **holeTable, int *numHoleEntries);

#endif // __TABLE_ADRESSE_H__
