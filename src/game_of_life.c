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

// Подсчёт живых соседей вокруг клетки (y, x) с учётом замкнутости тора
int count_neighbors(int field[HEIGHT][WIDTH], int y, int x) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            // Саму центральную клетку не считаем
            if (dy == 0 && dx == 0) {
                // continue;  ПОМЕНЯТЬ
            }
            // Формула тора: правый край соединён с левым, верх с низом
            int ny = (y + dy + HEIGHT) % HEIGHT;
            int nx = (x + dx + WIDTH) % WIDTH;
            count += field[ny][nx];
        }
    }
    return count;
}

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