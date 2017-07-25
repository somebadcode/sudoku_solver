#include <stdio.h>
#include "sudoku.h"

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

static bool sudokuGetFreeCell(int board[][9], int *row, int *col) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (!board[i][j]) {
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    return false;
}

static bool sudokuIsValid(int board[][9]) {
    int row, col, boxrow, boxcol;
    short colmask, rowmask, boxmask;
    for (row = 0; row < 9; row++) {
        rowmask = 0;
        for (col = 0; col < 9; col++) {

            /* Scan rows */
            if (board[row][col] == 0) { continue; }
            if (((1 << board[row][col]) & rowmask)) {
                return false;
            } else {
                rowmask = rowmask | (1 << board[row][col]);
            }

            /* If we're on the first row, validate columns while we're here. */
            if (row == 0) {
                colmask = 0;
                for (row = 0; row < 9; row++) {
                    if (board[row][col] == 0) { continue; }
                    if (((1 << board[row][col]) & colmask)) {
                        return false;
                    } else {
                        colmask = colmask | (1 << board[row][col]);
                    }
                }
                colmask = 0;
                row = 0;
            } /* End of column validation */

            /* Validate each box */
            if (!(row % 3) && !(col % 3)) {
                boxmask = 0;
                for (boxrow = 0; boxrow < 3; boxrow++) {
                    for (boxcol = 0; boxcol < 3; boxcol++) {
                        if (board[row+boxrow][col+boxcol] == 0) { continue; }
                        if ((( 1 << board[row+boxrow][col+boxcol]) & boxmask)) {
                            return false;
                        } else { 
                            boxmask = boxmask | (1 << board[row+boxrow][col+boxcol]);
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

    if (!sudokuGetFreeCell(board, &row, &col)) {
        return sudokuIsValid(board);
    } else {
        for (trial = 1; trial <= 9; trial++) {
            board[row][col] = trial;
            if(sudokuIsValid(board) == true) {
                if(!sudokuSolve(board)) {
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
