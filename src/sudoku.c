#include <stdio.h>
#include <stdbool.h>

#include "sudoku.h"

bool sudokuSolve(int board[][9]);
bool sudokuPrintBoard(int board[][9]);
static bool isValid(int board[][9]);
static bool isCellValid(int board[][9], int row, int col);

bool sudokuPrintBoard(int board[][9]) {
    int i;

    printf("╔═╤═╤═╦═╤═╤═╦═╤═╤═╗\n");
    for (i = 0; i < 9; i++) {
        if (i && !(i % 3)) {
            printf("╠═╪═╪═╬═╪═╪═╬═╪═╪═╣\n");
        }
        printf("║%d│%d│%d║%d│%d│%d║%d│%d│%d║\n", board[i][0],
                                                 board[i][1],
                                                 board[i][2],
                                                 board[i][3],
                                                 board[i][4],
                                                 board[i][5],
                                                 board[i][6],
                                                 board[i][7],
                                                 board[i][8]);
    }
    printf("╚═╧═╧═╩═╧═╧═╩═╧═╧═╝\n");
    return true;
}

static bool getFreeCell(int board[][9], int *row, int *col) {
    int i, j;
    for (i = 9; i--;) {
        for (j = 9; j--;) {
            if (!board[i][j]) {
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    return false;
}

static bool isCellValid(int board[][9], int row, int col) {
    int i, j;
    int rowbitfield = 0;
    int colbitfield = 0;
    int boxbitfield = 0;

    for (i = 9; i--;) {
        if (board[row][i]) {
            if (((1 << board[row][i]) & rowbitfield)) {
                return false;
            } else {
                rowbitfield = rowbitfield | (1 << board[row][i]);
            }
        }
        if (board[i][col]) {
            if (((1 << board[i][col]) & colbitfield)) {
                return false;
            } else {
                colbitfield = colbitfield | (1 << board[i][col]);
            }
        }
    }
    row = row - (row % 3);
    col = col - (col % 3);
    for (i = 3; i--;) {
        for (j = 3; j--;) {
            if (board[row+i][col+j]) {
                if ((( 1 << board[row+i][col+j]) & boxbitfield)) {
                    return false;
                } else {
                    boxbitfield = boxbitfield | (1 << board[row+i][col+j]);
                }
            }
        }
    }
    return true;
}

static bool isValid(int board[][9]) {
    int row, col, boxrow, boxcol;
    int colbitfield, rowbitfield, boxbitfield;
    for (row = 9; row--;) {
        rowbitfield = 0;
        for (col = 9; col--;) {

            /* Validate rows */
            if (!board[row][col]) { continue; }
            if (((1 << board[row][col]) & rowbitfield)) {
                return false;
            } else {
                rowbitfield = rowbitfield | (1 << board[row][col]);
            }

            /* If we're on the first row, validate columns while we're here. */
            if (row == 0) {
                colbitfield = 0;
                for (row = 9; row--;) {
                    if (!board[row][col]) { continue; }
                    if (((1 << board[row][col]) & colbitfield)) {
                        return false;
                    } else {
                        colbitfield = colbitfield | (1 << board[row][col]);
                    }
                }
                row = 0;
            } /* End of column validation */

            /* Validate each box */
            if (!(row % 3) && !(col % 3)) {
                boxbitfield = 0;
                for (boxrow = 3; boxrow--;) {
                    for (boxcol = 3; boxcol--;) {
                        if (!board[row+boxrow][col+boxcol]) { continue; }
                        if ((( 1 << board[row+boxrow][col+boxcol]) & boxbitfield)) {
                            return false;
                        } else {
                            boxbitfield = boxbitfield | (1 << board[row+boxrow][col+boxcol]);
                        }
                    }
                }
            } /* End of box validation */
        }
    }
    return true;
}

bool sudokuSolve(int board[][9]) {
    int trial, row, col;

    if (!getFreeCell(board, &row, &col)) {
        return isValid(board);
    } else {
        for (trial = 1; trial <= 9; trial++) {
            board[row][col] = trial;
            if (isCellValid(board, row, col)) {
                if (!sudokuSolve(board)) {
                    board[row][col] = 0;
                } else {
                    return true;
                }
            } else {
                board[row][col] = 0;
            }
        }
    }
    return false;
}
