#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25

/* Часть 1: Ввод данных */
void read_field(int field[HEIGHT][WIDTH]);

/* Часть 2: Логика игры */
int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);
void update_field(int field[HEIGHT][WIDTH]);

/* Часть 3: Отображение и главный цикл  */
void render(int field[HEIGHT][WIDTH]);

/* ========================================================================= */
/*               ЧАСТЬ 1: ВВОД ДАННЫХ (ВЕТКА feature/input)     */
/* ========================================================================= */

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