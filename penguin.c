#include <ncurses.h>
#include <stdlib.h>

#define PENG_H 4

// Penguin faces 
const char *penguin[4][PENG_H] = {
    { // Up
        "  _  ",
        "|(v)|",
        "     ",
        "     "
    },
    { // Down
        "  _  ",
        " ('v)",
        "//-=-\\",  // Corrected backslash
        "(\\_=_/)"
    },
    { // Left
        "  _  ",
        " (v) ",
        " /V\\ ",   // Left-side arm/wing
        "(_)> "
    },
    { // Right
        "   _  ",
        "  (v) ",
        " //V\\ ",  // Right-side arm/wing
        "  <(_)"
    }
};

// Draw dynamic terminal border
void draw_border(int rows, int cols) {
    mvhline(0, 0, '_', cols);
    mvhline(rows - 1, 0, '_', cols);
    mvvline(0, 0, '|', rows);
    mvvline(0, cols - 1, '|', rows);
    mvaddch(0, 0, '+');
    mvaddch(0, cols - 1, '+');
    mvaddch(rows - 1, 0, '+');
    mvaddch(rows - 1, cols - 1, '+');
}

int main(void) {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(50); // waits 50ms for input

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int x = 10, y = 5;
    int ch;
    int frame = 0; // start facing Right (Up arrow)

    while (1) {
        getmaxyx(stdscr, max_y, max_x);
        clear();

        attron(COLOR_PAIR(1));
        draw_border(max_y, max_x);
        attroff(COLOR_PAIR(1));

        // Clamp penguin inside borders
        if (x < 1) x = 1;
        if (y < 1) y = 1;
        if (x > max_x - 6) x = max_x - 6;
        if (y > max_y - PENG_H - 1) y = max_y - PENG_H - 1;

        // Draw penguin
        for (int i = 0; i < PENG_H; i++) {
            mvprintw(y + i, x, penguin[frame][i]);
        }

        refresh();

        ch = getch();
        if (ch == KEY_UP) {
            y--;
            frame = 0; // Up arrow = Right
        }
        else if (ch == KEY_DOWN) {
            y++;
            frame = 1; // Down arrow = Left
        }
        else if (ch == KEY_LEFT) {
            x--;
            frame = 2; // Left arrow = Down
        }
        else if (ch == KEY_RIGHT) {
            x++;
            frame = 3; // Right arrow = Up
        }
        else if (ch == 'q' || ch == 'Q') {
            break;
        }
        // If no key pressed, penguin stops
    }

    endwin();
    return 0;
}

