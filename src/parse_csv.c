#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_csv.h"

#define MAX_INPUT_SIZE 4095

extern int errno;

static const char *ERR_CONVERSION_ERROR = "invalid token, expected an integer but got something else";
static const char *ERR_ERANGE = "integer out of range, expected 0 <= i <= 9";
static const char *ERR_INPUT_EMPTY = "input is empty";
static const char *ERR_INPUT_TOO_LARGE = "input is too large (size > 4095)";
static const char *ERR_MEMORY = "memory allocation failed, unable to parse CSV";
static const char *ERR_TRAILING_DATA = "unexpected trailing data in file, please make sure that there's no data after the table";
static const char *ERR_UNEXPECTED_END = "unexpected end of CSV";

static inline bool isonlyspace(char *str) {
    while(*str != '\0') {
        if (!isspace((unsigned char)*str)) {
            return false;
        }
        str++;
    }
    return true;
}

const char *parse_csv(FILE *fp, int board[9][9], unsigned long *position, int coords[2]) {
    char *buffer;

    int err = fseek(fp, 0L, SEEK_END);
    if (err != 0) {
        return strerror(1);
    }

    // Get offset and rewind.
    size_t size = (size_t)ftell(fp);
    rewind(fp);

    if (size > MAX_INPUT_SIZE) return ERR_INPUT_TOO_LARGE;
    if (size == 0) return ERR_INPUT_EMPTY;

    buffer = calloc(1, size + 1);
    if (buffer == NULL) return ERR_MEMORY;

    fread(buffer, 1, size, fp);

    char *token = buffer;
    *position = 0;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            coords[0] = x; coords[1] = y;
            if (token == NULL) {
                free(buffer);
                return ERR_UNEXPECTED_END;
            }

            // Find delimiter.
            char *end = strpbrk(token, ",\n");
            if (end != NULL) {
                *end = '\0';
            }

            // If the token is empty, let's make it 0.
            if (strlen(token) == 0) {
                board[y][x] = 0;
                continue;
            }

            // Convert token to long int, if possible.
            char *endptr;
            long int num = strtol(token, &endptr, 10);
            if (!isonlyspace(endptr)) {
                free(buffer);
                return ERR_CONVERSION_ERROR;
            }

            // Make sure it's within the valid range.
            if (num < 0 || num > 9) {
                free(buffer);
                return ERR_ERANGE;
            }

            // Store the number.
            board[y][x] = (int)num;

            // Advance position up to the end of the token so that we can use
            // it after parsing the last field to see if we reached the end
            // of the input or if there's more trailing data.
            *position = (unsigned long)(token - buffer) + (unsigned long)strlen(token);

            // Advance the token but make it NULL if end is NULL.
            token = end;
            if (token != NULL) {
                token++;
            }
        }
    }

    // Return an error if there's any trailing data since we don't want to let
    // the user assume that the input is correct. This error can be ignored by
    // the caller.
    if (!isonlyspace(buffer + *position + 1)) {
        free(buffer);
        return ERR_TRAILING_DATA;
    }
    free(buffer);
    return NULL;
}
