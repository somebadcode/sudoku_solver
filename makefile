CC=gcc
CFLAGS_COMMON=-Wall -pedantic -pedantic-errors --std=c11
CFLAGS_DEBUG=-O0 -g3 $(CLFAGS_COMMON)
CFLAGS_OPTIMAL=-O3 -mtune=native -Werror -fprefetch-loop-arrays
CFLAGS=$(CFLAGS_OPTIMAL) $(CFLAGS_COMMON)
SRCDIR=src
INCLUDESDIR=src/includes
BUILDDIR=build
INCLUDES=-I$(INCLUDESDIR)
OBJECTS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
TARGET=sudoku

MKDIR_P=mkdir -p

.PHONY: default all clean directories debug
.PRECIOUS: $(TARGET) $(OBJECTS)

default: directories $(TARGET)
all: default

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

directories: ${BUILDDIR}

${BUILDDIR}:
	${MKDIR_P} ${BUILDDIR}

debug: CFLAGS=$(CFLAGS_DEBUG) $(CFLAGS_COMMON)
debug: all

clean:
	-rm -rf $(BUILDDIR)
	-rm -f $(TARGET)
