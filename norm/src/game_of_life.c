#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define MIN_DELAY 40
#define MAX_DELAY 1000
#define DELAY_STEP 40

/* Часть 1: Ввод данных */
int read_field(int field[HEIGHT][WIDTH]);
int read_cell(char symbol, int* cell);
int has_only_whitespace(void);

/* Часть 2: Логика игры */
int count_neighbors(int field[HEIGHT][WIDTH], int row, int col);
int next_cell_state(int field[HEIGHT][WIDTH], int row, int col);
void copy_field(int source[HEIGHT][WIDTH], int destination[HEIGHT][WIDTH]);
void update_field(int field[HEIGHT][WIDTH]);

/* Часть 3: Отображение и главный цикл */
void render(int field[HEIGHT][WIDTH]);
int process_key(int key, int* delay);
void run_game(int field[HEIGHT][WIDTH]);
int start_terminal(int field[HEIGHT][WIDTH]);

/* ========================================================================= */
/*               ЧАСТЬ 1: ВВОД ДАННЫХ (ВЕТКА feature/input)                  */
/* ========================================================================= */

int read_cell(char symbol, int* cell) {
    int valid = 1;
    *cell = 0;
    if (symbol == 'O' || symbol == '1') {
        *cell = 1;
    } else if (symbol != '.' && symbol != '0') {
        valid = 0;
    }
    return valid;
}

int has_only_whitespace(void) {
    int valid = 1;
    int symbol = fgetc(stdin);
    while (symbol != EOF && valid) {
        if (symbol != ' ' && symbol != '\t' && symbol != '\n' &&
            symbol != '\r' && symbol != '\v' && symbol != '\f') {
            valid = 0;
        }
        symbol = fgetc(stdin);
    }
    return valid;
}

int read_field(int field[HEIGHT][WIDTH]) {
    int valid = 1;
    int row = 0;
    while (row < HEIGHT && valid) {
        int col = 0;
        while (col < WIDTH && valid) {
            char symbol;
            if (scanf(" %c", &symbol) != 1) {
                valid = 0;
            } else {
                valid = read_cell(symbol, &field[row][col]);
            }
            col++;
        }
        row++;
    }
    if (valid) valid = has_only_whitespace();
    return valid;
}

/* ========================================================================= */
/*            ЧАСТЬ 2: ЛОГИКА И МАТЕМАТИКА (ВЕТКА feature/logic)             */
/* ========================================================================= */

int count_neighbors(int field[HEIGHT][WIDTH], int row, int col) {
    int count = 0;
    int row_offset = -1;
    while (row_offset <= 1) {
        int col_offset = -1;
        while (col_offset <= 1) {
            int neighbor_row = (row + HEIGHT + row_offset) % HEIGHT;
            int neighbor_col = (col + WIDTH + col_offset) % WIDTH;
            count += field[neighbor_row][neighbor_col];
            col_offset++;
        }
        row_offset++;
    }
    count -= field[row][col];
    return count;
}

int next_cell_state(int field[HEIGHT][WIDTH], int row, int col) {
    int neighbors = count_neighbors(field, row, col);
    int state = field[row][col];
    if (neighbors < 2 || neighbors > 3) {
        state = 0;
    } else if (neighbors == 3) {
        state = 1;
    }
    return state;
}

void copy_field(int source[HEIGHT][WIDTH], int destination[HEIGHT][WIDTH]) {
    int row = 0;
    while (row < HEIGHT) {
        int col = 0;
        while (col < WIDTH) {
            destination[row][col] = source[row][col];
            col++;
        }
        row++;
    }
}

void update_field(int field[HEIGHT][WIDTH]) {
    int next[HEIGHT][WIDTH];
    int row = 0;
    while (row < HEIGHT) {
        int col = 0;
        while (col < WIDTH) {
            next[row][col] = next_cell_state(field, row, col);
            col++;
        }
        row++;
    }
    copy_field(next, field);
}

/* ========================================================================= */
/*           ЧАСТЬ 3: ГРАФИКА И ИГРОВОЙ ЦИКЛ (ВЕТКА feature/ui)              */
/* ========================================================================= */

void render(int field[HEIGHT][WIDTH]) {
    int row = 0;
    erase();
    while (row < HEIGHT) {
        int col = 0;
        while (col < WIDTH) {
            char cell = field[row][col] ? 'O' : '.';
            mvaddch(row, col, cell);
            col++;
        }
        row++;
    }
    refresh();
}

int process_key(int key, int* delay) {
    int running = 1;
    if (key == ' ') {
        running = 0;
    } else if (key == 'a' || key == 'A') {
        if (*delay > MIN_DELAY) *delay -= DELAY_STEP;
    } else if (key == 'z' || key == 'Z') {
        if (*delay < MAX_DELAY) *delay += DELAY_STEP;
    }
    return running;
}

void run_game(int field[HEIGHT][WIDTH]) {
    int running = 1;
    int delay = 240;
    cbreak();
    noecho();
    curs_set(0);
    while (running) {
        render(field);
        timeout(delay);
        running = process_key(getch(), &delay);
        if (running) {
            update_field(field);
        }
    }
}

int start_terminal(int field[HEIGHT][WIDTH]) {
    int started = 0;
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        if (initscr() != NULL) {
            run_game(field);
            endwin();
            started = 1;
        }
    }
    return started;
}

int main(void) {
    int field[HEIGHT][WIDTH] = {{0}};
    int result = 0;
    if (!read_field(field)) {
        fprintf(stderr, "Ошибка: ожидалось поле 80x25 из символов . O или 0 1.\n");
        result = 1;
    } else if (!start_terminal(field)) {
        fprintf(stderr, "Ошибка: не удалось открыть управляющий терминал.\n");
        result = 1;
    }
    return result;
}
