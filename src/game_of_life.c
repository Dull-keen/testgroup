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

#ifdef TEST_PART1
int main(void) {
    int field[HEIGHT][WIDTH];
    read_field(field);
    printf("Проверка считывания поля (первые 5 строк):\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", field[i][j] ? '*' : '.');
        }
        printf("\n");
    }
    return 0;
}
#endif

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

extern void read_field(int field[HEIGHT][WIDTH]);
extern void update_field(int field[HEIGHT][WIDTH]);

// Отрисовка поля в терминале
void render(int field[HEIGHT][WIDTH]) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    refresh();
}

int main(void) {
    int field[HEIGHT][WIDTH];
    int delay = 100;
    int running = 1;

    // Шаг 1: считываем начальное поле
    read_field(field);

    // Шаг 2: переключаем stdin обратно на физическую клавиатуру терминала
    freopen("/dev/tty", "r", stdin);

    // Шаг 3: настройка ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // Шаг 4: игровой цикл
    while (running) {
        render(field);

        timeout(delay);
        int ch = getch();

        if (ch == 'a' || ch == 'A') {
            if (delay > 20) {
                delay -= 20;  // Увеличение скорости
            }
        } else if (ch == 'z' || ch == 'Z') {
            if (delay < 400) {
                delay += 20;  // Уменьшение скорости
            }
        } else if (ch == ' ') {
            running = 0;  // Выход по пробелу
        }

        // Шаг 5: пересчёт поля
        update_field(field);
    }

    // Шаг 6: освобождаем ресурсы терминала
    endwin();
    return 0;
}
