#include <stdlib.h>
#include <ncurses.h>
#include "window.h"

window_t *window_create(int posX, int posY, int width, int height, char *title, bool scroll)
{
    window_t *result;

    // Structure allocation
    if ((result = malloc(sizeof(window_t))) == NULL)
    {
        fprintf(stderr, "Error allocation window\n");
        exit(EXIT_FAILURE);
    }

    // Filling fields
    result->posx = posX;
    result->posy = posY;
    result->height = height;
    result->width = width;

    // Create main window with the box
    result->box = newwin(height, width, posY, posX);
    box(result->box, 0, 0);
    wprintw(result->box, title);
    wrefresh(result->box);

    // Create inner window
    result->inner = subwin(result->box, height - 2, width - 2, posY + 1, posX + 1);
    scrollok(result->inner, scroll);
    wrefresh(result->inner);

    return result;
}

void window_delete(window_t **window)
{
    delwin((*window)->inner);
    delwin((*window)->box);
    free(*window);
    *window = NULL;
}

int window_printw(window_t *window, const char *fmt, ...)
{
    va_list lstPar;
    int result;

    va_start(lstPar, fmt);
    result = vw_printw(window->inner, fmt, lstPar);
    va_end(lstPar);

    return result;
}

int window_printw_col(window_t *window, unsigned int color, const char *fmt, ...)
{
    va_list lstPar;
    int result;

    va_start(lstPar, fmt);
    wattron(window->inner, COLOR_PAIR(color));
    result = vw_printw(window->inner, fmt, lstPar);
    wattroff(window->inner, COLOR_PAIR(color));
    va_end(lstPar);

    return result;
}

int window_refresh(window_t *window)
{
    return wrefresh(window->inner);
}

int window_mvprintw(window_t *window, int posY, int posX, const char *fmt, ...)
{
    va_list lstPar;
    int result;

    va_start(lstPar, fmt);
    wmove(window->inner, posY, posX);
    result = vw_printw(window->inner, fmt, lstPar);
    va_end(lstPar);

    return result;
}

int window_mvprintw_col(window_t *window, int posY, int posX, unsigned int color, const char *fmt, ...)
{
    va_list lstPar;
    int result;

    va_start(lstPar, fmt);
    wmove(window->inner, posY, posX);
    wattron(window->inner, COLOR_PAIR(color));
    result = vw_printw(window->inner, fmt, lstPar);
    wattroff(window->inner, COLOR_PAIR(color));
    va_end(lstPar);

    return result;
}

int window_erase(window_t *window)
{
    return werase(window->inner);
}

bool window_getcoordinates(window_t *window, int posx, int posy, int *x, int *y)
{
    bool result = FALSE;

    if (((posx >= window->posx + 1) && (posx < window->width + window->posx - 1)) &&
        ((posy >= window->posy + 1) && (posy < window->height + window->posy - 1)))
    {
        result = TRUE;
        *x = posx - window->posx - 1;
        *y = posy - window->posy - 1;
    }

    return result;
}

int window_color(window_t *window, unsigned int color)
{
    return wattron(window->inner, COLOR_PAIR(color));
}

int window_mvaddch(window_t *window, int posY, int posX, chtype c)
{
    wmove(window->inner, posY, posX);
    return waddch(window->inner, c);
}

int window_mvaddch_col(window_t *window, int posY, int posX, unsigned int color, chtype c)
{
    int result;

    wattron(window->inner, COLOR_PAIR(color));
    wmove(window->inner, posY, posX);
    result = waddch(window->inner, c);
    wattroff(window->inner, COLOR_PAIR(color));

    return result;
}