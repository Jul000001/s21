#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

#define fori(i, n) for(int i = 0; i < n; i++)
#define fori2(x, w, y, h) fori(x, w) fori(y, h)
#define foriq(i, n) for(int i = -1; i < n; i++)
#define foriq2(x, w, y, h) foriq(x, w) foriq(y, h)

void input(int **map);
void speed(int *pause);
void render_map(int **map);
int status_quo(int **map, int x, int y);
void calculate(int **map);
void clearScreen();

int main() {
    int** map = malloc(HEIGHT * sizeof(int*)); // выделение памяти на указатели строк матрицы
    fori(i, HEIGHT) map[i] = malloc(WIDTH * sizeof(int)); // выделение памяти на матрицу
    int delta_time;
    input(map); // ввод данных
    speed(&delta_time); // ввод данных
    render_map(map); // отрисовка экрана
    fori(i, 100000) { // цикл игры в 50_000 * delta_time милисекунд
        calculate(map); // высчитывание параметров клеток
        clearScreen(); // очистка экрана
        render_map(map); // отрисовка экрана
        usleep(delta_time); // сон игры на delta_time милисекунд
    }
    fori(i, HEIGHT) free(map[i]); // очистка памяти матрицы
    free(map); // очистка памяти указателей на строки матрицы
    return 0;
}

void clearScreen() { printf("\33[0d\33[2J"); }

void input(int **map) {
    printf("\nВведите входные значения матрицы 25х80 (Состояния клетки: 1 - функционирует, 0 - отсутствует)\n");
    fori2(x, HEIGHT, y, WIDTH) scanf("%d", &map[x][y]);
}

void speed(int *delta) {
    int speed;
    printf("\nВведите скорость игры (Режимы: 1 - медленно, 2 - средне, 3 - быстро)\n");
    if (scanf("%d", &speed) == 1) {
        if(speed == 1) {
            *delta = 1000000;
        } else if(speed == 2) {
            *delta = 100000;
        } else if(speed == 2) {
            *delta = 10000;
        } else {
            printf("Введено не корректное значение\n");
        }
    } else {
        printf("Введено не корректное значение\n");
    }
}

void render_map(int **map) {
    fori(x, HEIGHT) {
        fori(y, WIDTH) (map[x][y] == 1) ? printf("O") : printf(".");
        printf("\n");
    }
    printf("\n");
}

int status_quo(int **map, int x, int y) {
    int count_figure = 0; // счётчик рядом стоящих фигур
    foriq2(i, 2, j, 2) if(!(i == 0 && j == 0)) if(!(i == 0 && j == 0)) count_figure += map[(x + HEIGHT + i) % HEIGHT][(y + WIDTH + j) % WIDTH];
    return count_figure; // сумма всех клеток в квадрате 3х3 с вычетом самой клетки
}

void calculate(int **map) {
    int map_new[HEIGHT][WIDTH];
    fori2(x, HEIGHT,y, WIDTH) {
        int count_figure = status_quo(map, x, y);
        if (count_figure == 3) {
            map_new[x][y] = 1;
        } else if (count_figure == 2 && map[x][y] == 1) {
            map_new[x][y] = 1;
        } else {
            map_new[x][y] = 0;
        }
    }
    fori2(x, HEIGHT, y, WIDTH) map[x][y] = map_new[x][y];
}