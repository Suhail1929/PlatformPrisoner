#include "../Window/window.h"
#include "../Couleur/couleur.h"
#include "../Tool/Block.h"
#include "../Tool/Start.h"
#include "../Tool/Exit.h"
#include "../Tool/Gate.h"
#include "../Tool/Key.h"
#include "../Tool/Ladder.h"
#include "../Tool/Life.h"
#include "../Tool/Probe.h"
#include "../Tool/Robot.h"
#include "../Tool/Bomb.h"
#include "../Tool/Door.h"
#include "../Tool/Player.h"
#include "../Tool/Trap.h"
#include "../Level/niveau.h"
#include "../Interface/interface.h"
#include "objet.h"

Objet_t *initObjet(int largeur, int hauteur, int couleur, int id_type)
{
    Objet_t *objet = malloc(sizeof(Objet_t));
    objet->largeur = largeur;
    objet->hauteur = hauteur;
    objet->couleur = couleur;
    objet->id_type = id_type;

    switch (id_type)
    {
    case PLAYER:
        objet->type.player = initPlayer("Chuck_Norris", 100, 1, largeur, hauteur);
        break;
    case ROBOT:
        objet->type.robot = initRobot(largeur, hauteur, 100);
        break;
    case BLOCK:
        objet->type.block = initBlock(largeur, hauteur);
        break;
    case LADDER:
        objet->type.ladder = initLadder(largeur, hauteur);
        break;
    case TRAP:
        objet->type.trap = initTrap(largeur, hauteur, 10);
        break;
    case GATE:
        objet->type.gate = initGate(largeur, hauteur);
        break;
    case KEY:
        objet->type.key = initKey(largeur, hauteur);
        break;
    case DOOR:
        objet->type.door = initDoor(largeur, hauteur);
        break;
    case EXIT:
        objet->type.exit = initExit(largeur, hauteur);
        break;
    case START:
        objet->type.start = initStart(largeur, hauteur);
        break;
    case PROBE:
        objet->type.probe = initProbe(largeur, hauteur);
        break;
    case LIFE:
        objet->type.life = initLife(largeur, hauteur, 10);
        break;
    case BOMB:
        objet->type.bomb = initBomb(largeur, hauteur, 30);
        break;
    default:
        break;
    }

    return objet;
}
void drawObjet(Objet_t objet, window_t *window, int x, int y)
{
    switch (objet.id_type)
    {
    case BLOCK:
        drawBlock(window, x, y, CYAN);
        break;
    case LADDER:
        drawLadder(window, x, y, YELLOW);
        break;
    case TRAP:
        drawTrap(window, x, y, CYAN);
        break;
    case GATE:
        drawGate(window, x, y, objet.couleur);
        break;
    case KEY:
        drawKey(window, x, y, objet.couleur);
        break;
    case DOOR:
        drawDoor(window, x, y, objet.couleur);
        break;
    case EXIT:
        drawExit(window, x, y, objet.couleur);
        break;
    case START:
        drawStart(window, x, y, objet.couleur);
        break;
    case ROBOT:
        drawRobot(window, x, y, WHITE);
        break;
    case PROBE:
        drawProbe(window, x, y, WHITE);
        break;
    case LIFE:
        drawLife(window, x, y, objet.couleur);
        break;
    case BOMB:
        drawBomb(window, x, y, WHITE);
        break;
    default:
        break;
    }
}

void deleteObjet(Objet_t *objet)
{
    free(objet);
}