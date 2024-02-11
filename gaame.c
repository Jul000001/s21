#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define STR 25
#define COL 80
#define FIELD_WIDTH 81
#define FIELD_HEIGHT 26

void input_matrix(int matr[STR][COL]);
void update_matrix(int matr1[STR][COL], int matr2[STR][COL]);
int count_neighbors(int matr1[STR][COL], int i, int j);
int decision(int neighbors, int condition);
void replace(int matr2[STR][COL], int matr1[STR][COL]);
int check(int matr1[STR][COL], int matr2[STR][COL]);
int change_speed(char control_button, int *flag, int time_mili_sec);
int count(int matr[STR][COL]);
void first_Pattern(int cur_State[STR][COL]);

int main() {
    int matr1[STR][COL], matr2[STR][COL], time_mili_sec = 500, stop = 0;
     char manual_input;
     printf("Желаете ли вы ввести данные вручную? (y/n): ");
    scanf(" %c", &manual_input);
    
    if (manual_input == 'y' || manual_input == 'Y') {
        while (getchar() != '\n'); // Очистка буфера ввода
        printf("Введите данные вручную:\n");
        input_matrix(matr1);
    } else {
        first_Pattern(matr1);
    }
    if (freopen("/dev/tty", "r", stdin)) initscr();
    nodelay(stdscr, true);

    while (stop != 1) {
        char control_button = getch();
        if (count(matr1) == 0) stop = 1;
        time_mili_sec = change_speed(control_button, &stop, time_mili_sec);
        usleep(time_mili_sec * 1000);
        clear();
        update_matrix(matr1, matr2);
        if (check(matr1, matr2) == 2000) stop = 1;
        replace(matr2, matr1);
    }

    endwin();
    return 0;
}

void input_matrix(int matr[STR][COL]) {
    for (int i = 0; i < STR; i++)
        for (int j = 0; j < COL; j++) scanf("%d", &matr[i][j]);
}

void update_matrix(int matr1[STR][COL], int matr2[STR][COL]) {
    char buffer[FIELD_HEIGHT][FIELD_WIDTH + 1]; // +1 для символа конца строки '\0'
    
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (i == 0 || j == 0 || i == FIELD_HEIGHT - 1 || j == FIELD_WIDTH - 1) {
                buffer[i][j] = '#'; // Границы поля
            } else {
                matr2[i][j] = decision(count_neighbors(matr1, i, j), matr1[i][j]);
                if (matr2[i][j] == 1)
                    buffer[i][j] = 'X'; // Живые клетки
                else
                    buffer[i][j] = '.'; // Пустые клетки
            }
        }
        buffer[i][FIELD_WIDTH] = '\0'; // Добавляем символ конца строки
    }

    // Выводим буфер на экран
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        printw("%s\n", buffer[i]);
    }
}

int count_neighbors(int matr1[STR][COL], int i, int j) {
    int sum = 0, di[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dj[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int k = 0; k < 8; k++) sum += matr1[(i + di[k] + STR) % STR][(j + dj[k] + COL) % COL];
    return sum;
}

int decision(int neighbors, int condition) {
    return (neighbors == 3 || (neighbors == 2 && condition == 1)) ? 1 : 0;
}

void replace(int matr2[STR][COL], int matr1[STR][COL]) {
    for (int i = 0; i < STR; i++)
        for (int j = 0; j < COL; j++) matr1[i][j] = matr2[i][j];
}

int check(int matr1[STR][COL], int matr2[STR][COL]) {
    int ans = 0;
    for (int i = 0; i < STR; i++)
        for (int j = 0; j < COL; j++) if (matr1[i][j] == matr2[i][j]) ans++;
    return ans;
}

int change_speed(char control_button, int *flag, int time_mili_sec) {
    switch (control_button) {
        case '1': time_mili_sec = 900; break;
        case '2': time_mili_sec = 400; break;
        case '3': time_mili_sec = 70; break;
        case 'q': *flag = 1; break;
    }
    return time_mili_sec;
}

int count(int matr[STR][COL]) {
    int sum = 0;
    for (int i = 0; i < STR; i++)
        for (int j = 0; j < COL; j++) sum += matr[i][j];
    return sum;
}

void first_Pattern(int cur_State[STR][COL]) {
    int flag = 1;
    FILE *fp;
    do {
        char *patt = "pattern";
        char *ext = ".txt";
        char num;

        printf("Введите номер паттерна (1-5):\n");
        scanf("%c", &num);
        char file_Name[13];
        snprintf(file_Name, sizeof file_Name, "%s%c%s", patt, num, ext);
        if ((fp = fopen(file_Name, "r")) != NULL) {
            flag = 0;
        }
        printf("\033[0d\033[2J");
    } while (flag);
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < COL; j++) {
            fscanf(fp, "%d", &cur_State[i][j]);
        }
    }
    fclose(fp);
}
