# Sudoku Solver
A simple back tracking recursive brute force solver for Sudoku.

## Description
This little piece of software solves Sudoku using a back tracking recursive
brute force method. It takes simple CSV formatted input and attempts to solve
it, one file at a time, one board per file. The CSV input must not have headers
and must only contain 0-9, commas(`,`) and line feeds(`\n`) where `,` and `\n`
are delimiters. Each row on a Sudoku board can be one row in the CSV input for
readability or they can all be on a single line only being delimited by `,`.
Trailing data that's not whitespace will result in an error even though there's
a valid board. There is an input size limit that's way beyond a sanely
formatted board.

## Building

### Build
To build, just run `make` inside the repository. The final executable binary
will be in `bin/`.

### Compilers and compliance
* GCC C compiler 9.2.1 or later
* Clang 9.0.0 or later

Keep in mind that I focus on keeping it C11 compliant.

## Contribute
Feel free to contribute or come with suggestions that can improve the code but
I will try to keep it simple enough that others can learn from this just like
I created this to teach myself C.

The code should be formatted the same way through the whole project and since
no particular style is everybody's favorite I've chosen to pick a style similar
to the Linux kernel source code. To make this whole thing easier and consistent,
I've picked clang-format to do the job (`make format`).
