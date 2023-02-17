// # Author: HAYAT Rahim && MTARFI Souhail

#ifndef NIVEAU_H
#define NIVEAU_H

#define LARGEUR 60
#define HAUTEUR 20
typedef struct niveau
{
    off_t niveau_position; // je sais pas si sera utile , on verra
    int objet[HAUTEUR][LARGEUR];
    int id;
    char *nom;
    char *description;
    int *id_adresse;
    int is_deleted; // boolean pour savoir si le niveau est supprimé ou non
    // à modifier

} niveau_t;

niveau_t *niveau_init();
off_t niveau_add(niveau_t *niveau, char *file);
void display_niveau(niveau_t *niveau);
void niveau_remove(off_t niveau_pos, char *file);

#endif
