typedef enum { CSV, BOX } board_format;

typedef struct {
	char **	     args;
	int	     count;
	board_format output_format;
} parsed_args;

void parse_args(parsed_args *pargs, int argc, char *argv[]);
