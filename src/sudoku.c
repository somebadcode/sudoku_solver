#include <stdio.h>
#include <stdbool.h>
#include <jansson.h>

#include "sudoku.h"

char boardplan[] = { "╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                     "║   │   │   ║   │   │   ║   │   │   ║\n"
                     "╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n" };

bool sudokuSolve(int board[][9]);
bool sudokuPrintBoard(int board[][9]);
json_t* sudokuGetBoardJson(int board[][9]);
static bool isValid(int board[][9]);
static bool isCellValid(int board[][9], int row, int col);

bool sudokuPrintBoard(int board[][9]) {
    int i;

    for (i = 9; i--;) {
        boardplan[111+170*i+5] = (char)(board[i][0] + (int)'0');
        boardplan[111+170*i+11] = (char)(board[i][1] + (int)'0');
        boardplan[111+170*i+17] = (char)(board[i][2] + (int)'0');
        boardplan[111+170*i+23] = (char)(board[i][3] + (int)'0');
        boardplan[111+170*i+29] = (char)(board[i][4] + (int)'0');
        boardplan[111+170*i+35] = (char)(board[i][5] + (int)'0');
        boardplan[111+170*i+41] = (char)(board[i][6] + (int)'0');
        boardplan[111+170*i+47] = (char)(board[i][7] + (int)'0');
        boardplan[111+170*i+53] = (char)(board[i][8] + (int)'0');
    }
	puts(boardplan);

    return true;
}

json_t* sudokuGetBoardJson(int board[][9]) {
    json_t *json_arrA;
    json_t *json_arrB;
    int i;

    json_arrA = json_array();

    for (i = 0; i < 9; i++) {
        json_arrB = json_array();
        json_array_append_new(json_arrB, json_integer(board[i][0]));
        json_array_append_new(json_arrB, json_integer(board[i][1]));
        json_array_append_new(json_arrB, json_integer(board[i][2]));
        json_array_append_new(json_arrB, json_integer(board[i][3]));
        json_array_append_new(json_arrB, json_integer(board[i][4]));
        json_array_append_new(json_arrB, json_integer(board[i][5]));
        json_array_append_new(json_arrB, json_integer(board[i][6]));
        json_array_append_new(json_arrB, json_integer(board[i][7]));
        json_array_append_new(json_arrB, json_integer(board[i][8]));
        json_array_append_new(json_arrA, json_arrB);
    }

    return json_arrA;
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
    int rowmask = 0;
    int colmask = 0;
    int boxmask = 0;

    /* Validate row and column in one sweep */
    for (i = 9; i--;) {
        if ((((1 << board[row][i]) &~1) & rowmask) \
          || ((1 << board[i][col]) &~1) & colmask) {
            return false;
        }
        rowmask |= ((1 << board[row][i]) &~ 1);
        colmask |= ((1 << board[i][col]) &~ 1);
    } /* End of row & col validation */

    /* Validate box */
    row = row - (row % 3);
    col = col - (col % 3);
    for (i = 3; i--;) {
        for (j = 3; j--;) {
            if (((( 1 << board[row+i][col+j]) &~ 1) & boxmask)) {
                return false;
            }
            boxmask |= ((1 << board[row+i][col+j]) &~ 1);
        }
    } /* End of box validation */

    return true;
}

static bool isValid(int board[][9]) {
    int row, col, boxrow, boxcol;
    int colmask, rowmask, boxmask;
    for (row = 9; row--;) {
        rowmask = 0;
        for (col = 9; col--;) {

            /* Validate rows */
            if (!board[row][col]) { continue; }
            if (((1 << board[row][col]) & rowmask)) {
                return false;
            } else {
                rowmask = rowmask | (1 << board[row][col]);
            }

            /* If we're on the first row, validate columns while we're here. */
            if (row == 0) {
                colmask = 0;
                for (row = 9; row--;) {
                    if (!board[row][col]) { continue; }
                    if (((1 << board[row][col]) & colmask)) {
                        return false;
                    } else {
                        colmask = colmask | (1 << board[row][col]);
                    }
                }
                row = 0;
            } /* End of column validation */

            /* Validate each box */
            if (!(row % 3) && !(col % 3)) {
                boxmask = 0;
                for (boxrow = 3; boxrow--;) {
                    for (boxcol = 3; boxcol--;) {
                        if (!board[row+boxrow][col+boxcol]) { continue; }
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
