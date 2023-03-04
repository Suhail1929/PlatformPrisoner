#ifndef _WINDOW_
#define _WINDOW_

typedef struct
{
    int posx, posy;
    int width, height;
    WINDOW *box;
    WINDOW *inner;
} window_t;

window_t *window_create(int posX, int posY, int width, int height, char *title, bool scroll);
void window_delete(window_t **window);
int window_printw(window_t *window, const char *fmt, ...);
int window_printw_col(window_t *window, unsigned int color, const char *fmt, ...);
int window_mvprintw(window_t *window, int posY, int posX, const char *fmt, ...);
int window_mvprintw_col(window_t *window, int posY, int posX, unsigned int color, const char *fmt, ...);
int window_refresh(window_t *window);
int window_erase(window_t *window);
bool window_getcoordinates(window_t *window, int posx, int posy, int *x, int *y);
int window_color(window_t *window, unsigned int color);
int window_mvaddch(window_t *window, int posY, int posX, chtype c);
int window_mvaddch_col(window_t *window, int posY, int posX, unsigned int color, chtype c);

#endif
