#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku.h"

static char boardBox[] = {"╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n"
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
			  "╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n"};

static char boardCSV[] = {" , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"
			  " , , , , , , , , \n"};

static bool getFreeCell(int board[9][9], int *row, int *col);
static bool isCellValid(int board[9][9], int row, int col);
static bool isValid(int board[9][9]);

char *sudokuGetBoardBox(int board[9][9])
{
	char *box = malloc(sizeof(boardBox));
	if (box == NULL) {
		return NULL;
	}

	strcpy(box, boardBox);

	for (int i = 9; i--;) {
		box[111 + 170 * i + 5]	= (char)(board[i][0] + (int)'0');
		box[111 + 170 * i + 11] = (char)(board[i][1] + (int)'0');
		box[111 + 170 * i + 17] = (char)(board[i][2] + (int)'0');
		box[111 + 170 * i + 23] = (char)(board[i][3] + (int)'0');
		box[111 + 170 * i + 29] = (char)(board[i][4] + (int)'0');
		box[111 + 170 * i + 35] = (char)(board[i][5] + (int)'0');
		box[111 + 170 * i + 41] = (char)(board[i][6] + (int)'0');
		box[111 + 170 * i + 47] = (char)(board[i][7] + (int)'0');
		box[111 + 170 * i + 53] = (char)(board[i][8] + (int)'0');
	}
	return box;
}

char *sudokuGetBoardCSV(int board[9][9])
{
	char *box = malloc(sizeof(boardCSV));
	if (box == NULL) {
		return NULL;
	}

	strcpy(box, boardCSV);

	for (int i = 9; i--;) {
		box[0 + i * 18]	    = (char)(board[i][0] + (int)'0');
		box[1 + i * 18 + 1] = (char)(board[i][1] + (int)'0');
		box[2 + i * 18 + 2] = (char)(board[i][2] + (int)'0');
		box[3 + i * 18 + 3] = (char)(board[i][3] + (int)'0');
		box[4 + i * 18 + 4] = (char)(board[i][4] + (int)'0');
		box[5 + i * 18 + 5] = (char)(board[i][5] + (int)'0');
		box[6 + i * 18 + 6] = (char)(board[i][6] + (int)'0');
		box[7 + i * 18 + 7] = (char)(board[i][7] + (int)'0');
		box[8 + i * 18 + 8] = (char)(board[i][8] + (int)'0');
	}
	return box;
}

static bool getFreeCell(int board[9][9], int *row, int *col)
{
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

static bool isCellValid(int board[9][9], int row, int col)
{
	int i, j;
	int rowmask = 0;
	int colmask = 0;
	int boxmask = 0;

	/* Validate row and column in one sweep */
	for (i = 9; i--;) {
		if ((((1 << board[row][i]) & ~1) & rowmask) ||
		    ((1 << board[i][col]) & ~1) & colmask) {
			return false;
		}
		rowmask |= ((1 << board[row][i]) & ~1);
		colmask |= ((1 << board[i][col]) & ~1);
	} /* End of row & col validation */

	/* Validate box */
	row = row - (row % 3);
	col = col - (col % 3);
	for (i = 3; i--;) {
		for (j = 3; j--;) {
			if ((((1 << board[row + i][col + j]) & ~1) & boxmask)) {
				return false;
			}
			boxmask |= ((1 << board[row + i][col + j]) & ~1);
		}
	} /* End of box validation */

	return true;
}

static bool isValid(int board[9][9])
{
	int row, col, boxrow, boxcol;
	int colmask, rowmask, boxmask;
	for (row = 9; row--;) {
		rowmask = 0;
		for (col = 9; col--;) {

			/* Validate rows */
			if (!board[row][col]) {
				continue;
			}
			if (((1 << board[row][col]) & rowmask)) {
				return false;
			} else {
				rowmask = rowmask | (1 << board[row][col]);
			}

			/* If we're on the first row, validate columns while
			 * we're here. */
			if (row == 0) {
				colmask = 0;
				for (row = 9; row--;) {
					if (!board[row][col]) {
						continue;
					}
					if (((1 << board[row][col]) &
					     colmask)) {
						return false;
					} else {
						colmask =
						    colmask |
						    (1 << board[row][col]);
					}
				}
				row = 0;
			} /* End of column validation */

			/* Validate each box */
			if (!(row % 3) && !(col % 3)) {
				boxmask = 0;
				for (boxrow = 3; boxrow--;) {
					for (boxcol = 3; boxcol--;) {
						if (!board[row + boxrow]
							  [col + boxcol]) {
							continue;
						}
						if (((1
						      << board[row + boxrow]
							      [col + boxcol]) &
						     boxmask)) {
							return false;
						} else {
							boxmask =
							    boxmask |
							    (1
							     << board[row +
								      boxrow]
								     [col +
								      boxcol]);
						}
					}
				}
			} /* End of box validation */
		}
	}
	return true;
}

bool sudokuSolve(int board[9][9])
{
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
