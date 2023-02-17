#include "../Window/window.h"
#include "Robot.h"
Robot_t *initRobot(int largeur, int hauteur, int health)
{
    Robot_t *robot = malloc(sizeof(Robot_t));
    robot->health = health;
    robot->largeur = largeur;
    robot->hauteur = hauteur;
    return robot;
}

void drawRobot(window_t *window, int x, int y, int color)
{
    window_mvaddch_col(window, y, x, color, ' ' | ACS_ULCORNER);
    window_mvaddch_col(window, y, x + 1, color, ' ' | ACS_BTEE);
    window_mvaddch_col(window, y, x + 2, color, ' ' | ACS_URCORNER);
    window_mvaddch_col(window, y + 1, x, color, ' ' | ACS_LLCORNER);
    window_mvaddch_col(window, y + 1, x + 1, color, ' ' | ACS_TTEE);
    window_mvaddch_col(window, y + 1, x + 2, color, ' ' | ACS_LRCORNER);
    window_mvaddch_col(window, y + 2, x, color, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y + 2, x + 1, color, ' ' | ACS_PLUS);
    window_mvaddch_col(window, y + 2, x + 2, color, ' ' | ACS_HLINE);
    window_mvaddch_col(window, y + 3, x, color, ' ' | ACS_ULCORNER);
    window_mvaddch_col(window, y + 3, x + 1, color, ' ' | ACS_BTEE);
    window_mvaddch_col(window, y + 3, x + 2, color, ' ' | ACS_URCORNER);
}