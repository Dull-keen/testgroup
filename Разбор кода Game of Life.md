# Подробный разбор программы «Игра „Жизнь“» (game_of_life.c)

Ниже представлен пошаговый разбор программы по мере её выполнения, с листингом каждой функции и детальным описанием того, что именно происходит в коде.

## 1. Точка входа в программу

```c
int main(void) {
    int field[HEIGHT][WIDTH] = {{0}};
    int result = 0;
    if (!ReadField(field)) {
        fprintf(stderr, "Ошибка: ожидалось поле 80x25 из символов . O или 0 1.\n");
        result = 1;
    } else if (!StartTerminal(field)) {
        fprintf(stderr, "Ошибка: не удалось открыть управляющий терминал.\n");
        result = 1;
    }
    return result;
}
```

Здесь начинается выполнение:
* Создается двумерный массив `field` размером 25 строк (`HEIGHT`) на 80 столбцов (`WIDTH`), и все его элементы инициализируются нулями.
* Вызывается функция чтения состояния поля `ReadField`. Если она возвращает `0` (ошибка валидации), программа выводит сообщение в поток ошибок `stderr` и готовится завершиться с кодом `1`.
* Если поле прочитано успешно, вызывается `StartTerminal`. Если терминал открыть не удается, также выводится ошибка.
* Программа возвращает `result` (`0` при успехе, `1` при ошибке).

---

## 2. Чтение и валидация данных

```c
int ReadField(int field[HEIGHT][WIDTH]) {
    int valid = 1;
    int row = 0;
    while (row < HEIGHT && valid) {
        int col = 0;
        while (col < WIDTH && valid) {
            char symbol;
            if (scanf(" %c", &symbol) != 1) {
                valid = 0;
            } else {
                valid = ReadCell(symbol, &field[row][col]);
            }
            col++;
        }
        row++;
    }
    if (valid) valid = HasOnlyWhitespaces();
    return valid;
}
```

Эта функция заполняет матрицу начальным состоянием:
* Запускаются вложенные циклы `while` для прохода по 25 строкам и 80 столбцам. Использование `while` вместо `for` часто встречается в строгих стандартах кодирования (например, в Школе 21).
* Функция `scanf(" %c", &symbol)` считывает следующий непробельный символ из потока стандартного ввода (`stdin`). Пробел перед `%c` позволяет игнорировать любые символы форматирования (пробелы, табы, переносы строк).
* Если символ успешно считан, он передается в `ReadCell` для расшифровки.
* Если чтение всей матрицы прошло без ошибок (`valid == 1`), вызывается `HasOnlyWhitespaces()` для проверки конца файла.

```c
int ReadCell(char symbol, int* cell) {
    int valid = 1;
    *cell = 0;
    if (symbol == 'O' || symbol == '1') {
        *cell = 1;
    } else if (symbol != '.' && symbol != '0') {
        valid = 0;
    }
    return valid;
}
```

* Функция получает символ и указатель на ячейку массива.
* По умолчанию ячейке присваивается состояние «мертва» (`0`).
* Если символ — это `'O'` (буква О) или `'1'`, в ячейку записывается `1` («жива»).
* Если символ не равен `'.'` и не равен `'0'`, это означает наличие посторонних символов (например, букв) — функция возвращает `0` (невалидно).

```c
int HasOnlyWhitespaces(void) {
    int valid = 1;
    int symbol = fgetc(stdin);
    while (symbol != EOF && valid) {
        if (symbol != ' ' && symbol != '\t' && symbol != '\n' && symbol != '\r' && symbol != '\v' &&
            symbol != '\f') {
            valid = 0;
        }
        symbol = fgetc(stdin);
    }
    return valid;
}
```

* После того как поле 80x25 заполнено, эта функция читает оставшийся ввод по одному символу через `fgetc` вплоть до конца файла (`EOF`).
* Если попадается хотя бы один видимый символ (не пробел, не табуляция, не перенос строки и т.д.), файл считается невалидным (содержит мусор в конце).

---

## 3. Переход в графический режим

```c
int StartTerminal(int field[HEIGHT][WIDTH]) {
    int started = 0;
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        if (initscr() != NULL) {
            RunGame(field);
            endwin();
            started = 1;
        }
    }
    return started;
}
```

Поскольку программа уже прочитала карту поля из `stdin` (например, через перенаправление файла `< init.txt`), обычный `stdin` теперь пуст (достигнут EOF).
* `freopen("/dev/tty", "r", stdin)` — критически важный шаг, который переподключает стандартный ввод на клавиатуру текущего терминала пользователя, чтобы считывать нажатия клавиш в игре.
* Если это удалось, `initscr()` инициализирует библиотеку `ncurses` (очищает экран, выделяет память).
* Запускается главный цикл игры `RunGame`.
* По завершении игры `endwin()` возвращает настройки терминала в нормальный вид (иначе после выхода из программы терминал будет сломан).

---

## 4. Игровой цикл и управление

```c
void RunGame(int field[HEIGHT][WIDTH]) {
    int running = 1;
    int delay = 240;
    cbreak();
    noecho();
    curs_set(0);
    while (running) {
        Render(field);
        timeout(delay);
        running = ProcessKey(getch(), &delay);
        if (running) {
            UpdateField(field);
        }
    }
}
```

Это «сердце» программы, работающее постоянно, пока пользователь не нажмет выход:
* Устанавливается задержка кадра `delay = 240` миллисекунд.
* Настройки `ncurses`:
    * `cbreak()` — отключает буферизацию терминала (реакция на нажатия происходит мгновенно, без ожидания Enter).
    * `noecho()` — отключает вывод нажимаемых клавиш на экран.
    * `curs_set(0)` — делает курсор (мигающий квадрат или линию) невидимым.
* В цикле `while` происходит: отрисовка (`Render`), установка таймаута ожидания клавиши, чтение нажатия `getch()`, его обработка (`ProcessKey`) и вычисление новой жизни (`UpdateField`). Функция `timeout()` делает `getch()` неблокирующим: если за `delay` мс клавиша не нажата, `getch()` вернет ошибку, и игра просто обновит кадр.

```c
int ProcessKey(int key, int* delay) {
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
```

* Если нажат пробел (`' '`), возвращается `0`, что останавливает цикл в `RunGame`.
* Клавиши `A` ускоряют игру, уменьшая задержку `delay` (пока она больше `MIN_DELAY = 40`), клавиши `Z` — замедляют, увеличивая задержку (пока она меньше `MAX_DELAY = 1000`).

```c
void Render(int field[HEIGHT][WIDTH]) {
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
```

* `erase()` очищает внутренний буфер экрана `ncurses`.
* Двойной цикл проходит по всему массиву: через тернарный оператор определяется символ (`'O'` для `1`, `'.'` для `0`).
* `mvaddch(row, col, cell)` помещает символ в буфер на конкретные координаты (строка, столбец).
* `refresh()` физически выводит все изменения из буфера на экран пользователя за один проход.

---

## 5. Обновление клеток (Математика и логика)

```c
void UpdateField(int field[HEIGHT][WIDTH]) {
    int next[HEIGHT][WIDTH];
    int row = 0;
    while (row < HEIGHT) {
        int col = 0;
        while (col < WIDTH) {
            next[row][col] = NextCellState(field, row, col);
            col++;
        }
        row++;
    }
    CopyField(next, field);
}
```

* Нельзя обновлять клетки прямо в оригинальном массиве, иначе новые состояния сразу повлияют на вычисления соседних клеток (а они должны обновляться синхронно).
* Поэтому создается временный массив `next`, куда записываются будущие состояния.
* После расчёта всего поля вызывается `CopyField`, чтобы перенести новые значения обратно в `field`.

```c
int NextCellState(int field[HEIGHT][WIDTH], int row, int col) {
    int neighbors = CountNeighbors(field, row, col);
    int state = field[row][col];
    if (neighbors < 2 || neighbors > 3) {
        state = 0;
    } else if (neighbors == 3) {
        state = 1;
    }
    return state;
}
```

Это непосредственная реализация правил Джона Конвея:
* Получает количество живых соседей вокруг клетки.
* Если соседей меньше 2 (одиночество) или больше 3 (перенаселение), клетка умирает (`0`).
* Если соседей ровно 3, в пустой клетке зарождается жизнь (`1`), а живая продолжает жить.
* При 2 соседях живая клетка остается живой (состояние не меняется).

```c
int CountNeighbors(int field[HEIGHT][WIDTH], int row, int col) {
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
```

* Считает живых соседей в квадрате 3x3 вокруг целевой клетки (от -1 до +1 по обеим осям).
* Математика `(row + HEIGHT + row_offset) % HEIGHT` реализует «замкнутость» (тороидальность) игрового поля: если выйти за левую границу (`col_offset = -1` при `col = 0`), то благодаря прибавлению ширины и взятию остатка от деления мы переместимся на самый правый край экрана.
* В цикле прибавляется состояние самой центральной клетки (если она жива, к `count` добавится 1). Чтобы не считать саму клетку своим же соседом, её значение отнимается в самом конце: `count -= field[row][col]`.

```c
void CopyField(int source[HEIGHT][WIDTH], int destination[HEIGHT][WIDTH]) {
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
```

* Простая утилитарная функция. Проходит по всем элементам временной матрицы `source` и переписывает их значения в основную матрицу `destination`, подготавливая её к новому кадру игры.