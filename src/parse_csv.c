#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_csv.h"

static const char *csv_error_string[] = {
    FOREACH_CSVERROR(GENERATE_STRING)
};

const char *get_csv_error(csv_error err) {
    return csv_error_string[err];
}

static inline bool isonlyspace(char *str) {
    while(*str != '\0') {
        if (!isspace((unsigned char)*str)) {
            return false;
        }
        str++;
    }
    return true;
}

csv_error parse_csv(FILE *fp, int board[9][9], unsigned long *position, int coords[2]) {
    char *buffer = NULL;
    csv_error outcome = CSV_SUCCESS;

    int err = fseek(fp, 0L, SEEK_END);
    if (err != 0) {
        outcome = CSV_ERROR_SEEK;
        goto cleanup;
    }

    // Get offset and rewind.
    size_t size = (size_t)ftell(fp);
    rewind(fp);

    if (size > CSV_MAX_INPUT_SIZE) {
        outcome = CSV_ERROR_INPUT_LARGE;
        goto cleanup;
    } else if (size == 0) {
        outcome = CSV_ERROR_INPUT_EMPTY;
        goto cleanup;
    }

    buffer = calloc(1, size + 1);
    if (buffer == NULL) {
        outcome = CSV_ERROR_MEMORY;
        goto cleanup;
    }

    size_t readsz = fread(buffer, 1, size, fp);
    if (readsz != size) {
       outcome = CSV_ERROR_READ_SIZE;
       goto cleanup;
    }

    char *token = buffer;
    *position = 0;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            coords[0] = x; coords[1] = y;
            if (token == NULL) {
                outcome = CSV_ERROR_END;
                goto cleanup;
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
                outcome = CSV_ERROR_CONVERSION;
                goto cleanup;
            }

            // Make sure it's within the valid range.
            if (num < 0 || num > 9) {
                outcome = CSV_ERROR_RANGE;
                goto cleanup;
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
        outcome = CSV_ERROR_TRAILING_DATA;
    }

    // Clean up and return outcome;
    cleanup:
        free(buffer);
        return outcome;
}
