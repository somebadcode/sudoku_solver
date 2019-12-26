#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "parse_csv.h"
#include "sudoku.h"

int main(int argc, char *argv[])
{

	parsed_args pargs;
	pargs.output_format = BOX;
	parse_args(&pargs, argc, argv);

	FILE *fp;
	int   board[9][9];

	if (pargs.count < 1) {
		fprintf(stderr, "Expected at least one input!\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < pargs.count; i++) {
		if (strcmp(pargs.args[i], "-") == 0) {
			fp = stdin;
		} else {
			fp = fopen(pargs.args[i], "r");
			if (fp == NULL) {
				perror("Unable to open file");
				free(pargs.args);
				return 1;
			}
		}

		unsigned long position;
		int	      coords[2];
		csv_error     csverr = parse_csv(fp, board, &position, coords);
		if (csverr != CSV_SUCCESS) {
			if (fp != stderr) {
				fclose(fp);
			}
			fprintf(stderr, "error while parsing CSV: %s\n", get_csv_error(csverr));
			free(pargs.args);
			return 1;
		}

		if (fp != stderr) {
			fclose(fp);
		}

		if (sudokuSolve(board)) {
			char *strboard = NULL;
			if (pargs.output_format == CSV) {
				strboard = sudokuGetBoardCSV(board);
			} else if (pargs.output_format == BOX) {
				strboard = sudokuGetBoardBox(board);
			}
			if (strboard == NULL) {
				perror("failed to generate board for output");
				free(pargs.args);
				return 1;
			}
			printf("# Board: %i\n%s", i + 1, strboard);
			free(strboard);
		} else {
			fprintf(stderr, "Unable to solve!\n");
			free(pargs.args);
			return 1;
		}
	}
	free(pargs.args);
	return EXIT_SUCCESS;
}
