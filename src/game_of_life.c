#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25

void read_field(int field[HEIGHT][WIDTH]);
int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);
void update_field(int field[HEIGHT][WIDTH]);
void render(int field[HEIGHT][WIDTH]);

void read_field(int field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char c = getchar();

            if (c == '\n') {
                c = getchar();
            }

            if (c == '*' || c == '1') {
                field[i][j] = 1;
            } else {
                field[i][j] = 0;
            }
        }
    }
}

void read_field(int field[HEIGHT][WIDTH]) { (void)field; }

/* ========================================================================= */
/*            ЧАСТЬ 2: ЛОГИКА И МАТЕМАТИКА (ВЕТКА feature/logic) */
/* ========================================================================= */

int count_neighbors(int field[HEIGHT][WIDTH], int y, int x) {
    (void)field;
    (void)y;
    (void)x;
    return 0;
}

void update_field(int field[HEIGHT][WIDTH]) { (void)field; }

/* ========================================================================= */
/*           ЧАСТЬ 3: ГРАФИКА И ИГРОВОЙ ЦИКЛ (ВЕТКА feature/ui)   */
/* ========================================================================= */

void render(int field[HEIGHT][WIDTH]) { (void)field; }

int main(void) { return 0; }