#ifndef ROBOT_H
#define ROBOT_H



typedef struct Robot
{
    int health;
    int largeur;
    int hauteur;
} Robot_t;

Robot_t *initRobot( int largeur, int hauteur,int health);
void drawRobot(window_t *window, int x, int y, int color);

#endif