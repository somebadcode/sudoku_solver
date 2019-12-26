#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_csv.h"
#include "sudoku.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    int board[9][9];

    if (argc > 2) {
        fprintf(stderr, "Expected one argument! Please enter name of CSV file that has valid Sudoku board.\n");
        return 1;
    }

    if ((argc == 2 && strcmp(argv[1], "-") == 0) || argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            perror("Unable to open file");
            return 1;
        }
    }

    unsigned long position;
    int coords[2];
    char const *errstring = parse_csv(fp, board, &position, coords);
    if (errstring != NULL) {
        if (fp != stderr) {
            fclose(fp);
        }
        fprintf(stderr, "%s\n", errstring);
        return 1;
    }

    if (fp != stderr) {
      fclose(fp);
    }


    if (sudokuSolve(board)) {
        char *strboard = sudokuGetBoardCSV(board);
        if (strboard == NULL) {
            perror("failed to generate board for output");
            return 1;
        }
        printf("%s", strboard);
        free(strboard);
    } else {
        fprintf(stderr, "Unable to solve!\n");
        return 1;
    }
    return EXIT_SUCCESS;
}

