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

// Расчёт нового поколения на основе текущего
void update_field(int field[HEIGHT][WIDTH]) {
    int next[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int n = count_neighbors(field, i, j);
            // Правило 1: живая выживает при 2 или 3 соседях
            if (field[i][j] == 1 && (n == 2 || n == 3)) {
                next[i][j] = 1;
                // Правило 2: мёртвая зарождается ровно при 3 соседях
            } else if (field[i][j] == 0 && n == 3) {
                next[i][j] = 1;
                // Правило 3: во всех остальных случаях умирает
            } else {
                next[i][j] = 0;
            }
        }
    }

    // Копируем рассчитанное поколение обратно в исходное поле
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = next[i][j];
        }
    }
}

/*
 * ДЛЯ САМОПРОВЕРКИ:
 */
#ifdef TEST_PART2
int main(void) {
    int field[HEIGHT][WIDTH] = {0};
    // Создаём осциллятор «мигалка» (Blinker): 3 клетки в ряд
    field[10][10] = 1;
    field[10][11] = 1;
    field[10][12] = 1;

    printf("Поколение 0: горизонтальная линия\n");
    update_field(field);
    printf("Поколение 1: должна стать вертикальной линией:\n");
    printf("field[9][11]=%d (ожидается 1)\n", field[9][11]);
    printf("field[10][11]=%d (ожидается 1)\n", field[10][11]);
    printf("field[11][11]=%d (ожидается 1)\n", field[11][11]);
    printf("field[10][10]=%d (ожидается 0)\n", field[10][10]);
    return 0;
}
#endif

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