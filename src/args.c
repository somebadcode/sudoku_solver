#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

static bool has_prefix(const char *prefix, const char *str);
static void set_output_format(parsed_args *pargs, char *val);
static void string_tolower(char *str);

void parse_args(parsed_args *pargs, int argc, char *argv[])
{
	pargs->args = calloc((unsigned long)argc, sizeof(char *));
	pargs->count = 0;

	if (pargs->args == NULL) {
		fprintf(stderr, "failed to allocate memeory: %s\n",
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
		if (has_prefix("--", argv[i])) {
			if (strlen(argv[i]) == 2) {
				for (i++; i < argc; i++) {
					pargs->args[pargs->count] = argv[i];
					pargs->count++;
				}
				break;
			}
			if (strcmp("--output", argv[i]) == 0) {
				set_output_format(pargs, argv[i]);
			} else if (has_prefix("--output=", argv[i])) {
				set_output_format(pargs,
						  strstr(argv[i], "=") + 1);
			}
		} else if (has_prefix("-", argv[i]) && strlen(argv[i]) > 1) {
			int x = i;
			for (size_t y = 1; argv[x][y]; y++) {
				if (argv[x][y] == 'o') {
					set_output_format(pargs, argv[++i]);
				} else {
					fprintf(stderr,
						"unrecognized flag '%c'\n",
						argv[i][y]);
					exit(EXIT_FAILURE);
				}
			}
		} else {
			pargs->args[pargs->count] = argv[i];
			pargs->count++;
		}
	}
	return;
}

bool has_prefix(const char *prefix, const char *str)
{
	return strncmp(prefix, str, strlen(prefix)) == 0;
}

void string_tolower(char *str)
{
	for (int i = 0; str[i]; i++) {
		str[i] = tolower(str[i]);
	}
}

void set_output_format(parsed_args *pargs, char *val)
{
	string_tolower(val);
	if (strcmp("csv", val) == 0) {
		pargs->output_format = CSV;
	} else if (strcmp("box", val) == 0) {
		pargs->output_format = BOX;
	} else {
		fprintf(stderr, "invalid output format '%.20s'\n", val);
		exit(EXIT_FAILURE);
	}
}
