#define CSV_MAX_INPUT_SIZE 4095

#define FOREACH_CSVERROR(CSVERROR)                                                                 \
	CSVERROR(CSV_SUCCESS)                                                                      \
	CSVERROR(CSV_ERROR_FREAD_SIZE_MISMATCH)                                                    \
	CSVERROR(CSV_ERROR_FSEEK)                                                                  \
	CSVERROR(CSV_ERROR_INPUT_EMPTY)                                                            \
	CSVERROR(CSV_ERROR_INPUT_TOO_LARGE)                                                        \
	CSVERROR(CSV_ERROR_INPUT_TRAILING_DATA)                                                    \
	CSVERROR(CSV_ERROR_INTEGER_CONVERSION)                                                     \
	CSVERROR(CSV_ERROR_INTEGER_OUT_OF_RANGE)                                                   \
	CSVERROR(CSV_ERROR_MEMORY_ALLOCATION)                                                      \
	CSVERROR(CSV_ERROR_UNEXPECTED_EOF)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum { FOREACH_CSVERROR(GENERATE_ENUM) } csv_error;

const char *get_csv_error(csv_error err);
csv_error   parse_csv(FILE *fp, int board[9][9], unsigned long *position, int coords[2]);
