#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "Sudoku.h"

static time_t timer;

void peak()
{
    time_t diff = clock() - timer;
    printf("Time passed: %.2fms\n", ((float) diff) / CLOCKS_PER_SEC * 1000);
    timer = clock();
}

int main()
{
    int wikipedia_sudoku[81] = {
            5, 3, 0,    0, 7, 0,    0, 0, 0,
            6, 0, 0,    1, 9, 5,    0, 0, 0,
            0, 9, 8,    0, 0, 0,    0, 6, 0,

            8, 0, 0,    0, 6, 0,    0, 0, 3,
            4, 0, 0,    8, 0, 3,    0, 0, 1,
            7, 0, 0,    0, 2, 0,    0, 0, 6,

            0, 6, 0,    0, 0, 0,    2, 8, 0,
            0, 0, 0,    4, 1, 9,    0, 0, 5,
            0, 0, 0,    0, 8, 0,    0, 7, 9
    };
    int easy[81] = {
            0, 0, 1,    5, 0, 6,    2, 0, 0,
            0, 5, 0,    0, 4, 2,    0, 9, 0,
            0, 0, 3,    7, 1, 0,    8, 0, 0,

            7, 0, 6,    0, 2, 0,    5, 0, 0,
            0, 2, 0,    0, 0, 0,    0, 3, 0,
            0, 0, 5,    0, 8, 0,    7, 0, 4,

            0, 0, 9,    0, 5, 7,    4, 0, 0,
            0, 1, 0,    3, 6, 0,    0, 7, 0,
            0, 0, 2,    1, 0, 8,    6, 0, 0
    };
    int medium[81] = {
            0, 0, 7,    0, 5, 0,    3, 9, 0,
            0, 0, 0,    0, 4, 2,    0, 7, 0,
            5, 2, 0,    9, 0, 0,    0, 0, 1,

            9, 0, 2,    0, 0, 0,    0, 6, 0,
            0, 1, 5,    6, 2, 4,    9, 3, 0,
            0, 6, 0,    0, 0, 0,    1, 0, 7,

            1, 0, 0,    0, 0, 5,    0, 8, 3,
            0, 3, 0,    4, 1, 0,    0, 0, 0,
            0, 5, 9,    0, 3, 0,    2, 0, 0
    };
    int hard[81] = {
            8, 0, 0,    0, 4, 0,    3, 0, 0,
            0, 4, 0,    7, 0, 0,    0, 8, 0,
            0, 0, 9,    0, 0, 8,    4, 7, 0,

            0, 0, 5,    3, 0, 0,    0, 2, 0,
            9, 0, 2,    5, 0, 4,    6, 0, 3,
            0, 8, 0,    0, 0, 9,    5, 0, 0,

            0, 9, 1,    8, 0, 0,    2, 0, 0,
            0, 2, 0,    0, 0, 1,    0, 3, 0,
            0, 0, 8,    0, 2, 0,    0, 0, 5
    };
    int very_hard[81] = {
            0, 0, 1,    8, 0, 5,    0, 0, 0,
            0, 0, 9,    0, 1, 0,    0, 5, 3,
            2, 5, 0,    0, 0, 0,    0, 6, 0,

            3, 2, 0,    0, 0, 0,    9, 0, 1,
            0, 1, 0,    7, 0, 2,    0, 3, 0,
            5, 0, 4,    0, 0, 0,    0, 2, 6,

            0, 4, 0,    0, 0, 0,    0, 9, 7,
            8, 7, 0,    0, 5, 0,    6, 0, 0,
            0, 0, 0,    1, 0, 7,    5, 0, 0
    };

    timer = clock();
    sudoku(wikipedia_sudoku);
    peak();
    sudoku(easy);
    peak();
    sudoku(medium);
    peak();
    sudoku(hard);
    peak();
    sudoku(very_hard);
    peak();
    print_sudoku(wikipedia_sudoku);
    print_sudoku(easy);
    print_sudoku(medium);
    print_sudoku(hard);
    print_sudoku(very_hard);
    return EXIT_SUCCESS;
}
