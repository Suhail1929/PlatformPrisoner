// author: Souhail MTARFI & Hayat RAHIM

/*
 * Enum representing the map
 */

enum Map
{
    ID_DELETE = 0,
    ID_BLOCK = 1,
    ID_LADDER = 2,
    ID_TRAP = 3,
    ID_EXIT = 4,
    ID_START = 5,
    ID_ROBOT = 6,
    ID_PROBE = 7,
    ID_LIFE = 8,
    ID_BOMB = 9,
    // ID_PLAYER = 10,
    ID_PURPLE_GATE = 40,
    ID_GREEN_GATE = 41,
    ID_BLUE_GATE = 42,
    ID_YELLOW_GATE = 43,
    ID_PURPLE_KEY = 50,
    ID_GREEN_KEY = 51,
    ID_BLUE_KEY = 52,
    ID_YELLOW_KEY = 53,
    ID_PURPLE_DOOR1 = 61,
    ID_PURPLE_DOOR2 = 62,
    ID_PURPLE_DOOR3 = 63,
    ID_PURPLE_DOOR4 = 64,
    ID_PURPLE_DOOR5 = 65,
    ID_PURPLE_DOOR6 = 66,
    ID_PURPLE_DOOR7 = 67,
    ID_PURPLE_DOOR8 = 68,
    ID_PURPLE_DOOR9 = 69,
    ID_GREEN_DOOR1 = 71,
    ID_GREEN_DOOR2 = 72,
    ID_GREEN_DOOR3 = 73,
    ID_GREEN_DOOR4 = 74,
    ID_GREEN_DOOR5 = 75,
    ID_GREEN_DOOR6 = 76,
    ID_GREEN_DOOR7 = 77,
    ID_GREEN_DOOR8 = 78,
    ID_GREEN_DOOR9 = 79,
    ID_BLUE_DOOR1 = 81,
    ID_BLUE_DOOR2 = 82,
    ID_BLUE_DOOR3 = 83,
    ID_BLUE_DOOR4 = 84,
    ID_BLUE_DOOR5 = 85,
    ID_BLUE_DOOR6 = 86,
    ID_BLUE_DOOR7 = 87,
    ID_BLUE_DOOR8 = 88,
    ID_BLUE_DOOR9 = 89,
    ID_YELLOW_DOOR1 = 91,
    ID_YELLOW_DOOR2 = 92,
    ID_YELLOW_DOOR3 = 93,
    ID_YELLOW_DOOR4 = 94,
    ID_YELLOW_DOOR5 = 95,
    ID_YELLOW_DOOR6 = 96,
    ID_YELLOW_DOOR7 = 97,
    ID_YELLOW_DOOR8 = 98,
    ID_YELLOW_DOOR9 = 99
};

int getEntityDetail(interface_t *interface, int tab[20][60], int posX, int posY, int *bloc_width, int *bloc_height, int *nb_door);
