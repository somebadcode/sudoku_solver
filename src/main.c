#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "sudoku.h"

int defaultboard[][9] = { { 8, 9, 0,   0, 0, 6,   0, 5, 3 },
                          { 0, 0, 1,   4, 3, 0,   6, 0, 0 },
                          { 6, 0, 3,   7, 0, 9,   0, 0, 2 },
                          { 3, 8, 2,   0, 0, 0,   0, 0, 0 },
                          { 0, 7, 0,   0, 0, 0,   0, 4, 0 },
                          { 0, 0, 0,   0, 0, 0,   9, 2, 1 },
                          { 4, 0, 0,   6, 0, 1,   2, 0, 7 },
                          { 0, 0, 8,   0, 4, 7,   5, 0, 0 },
                          { 7, 2, 0,   3, 0, 0,   0, 8, 4 } };

int main(int argc, char *argv[]) {

    if (sudokuSolve(defaultboard)) {
        if (sudokuPrintBoard(defaultboard)) {
            return EXIT_SUCCESS;
        } else {
            printf("Unable print solved board!\n");
            return 2;
        }
    } else {
        printf("Unable to solve!\n");
        return 1;
    }

    return EXIT_SUCCESS;
}


