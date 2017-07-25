CC=gcc
CFLAGS_COMMON=-Wall -Werror -pedantic -pedantic-errors --std=c11
CFLAGS_DEBUG=-O0 -g3 $(CLFAGS_COMMON)
CFLAGS_OPTIMAL=-O3 -mtune=native -fprefetch-loop-arrays
CFLAGS=$(CFLAGS_OPTIMAL) $(CFLAGS_COMMON)
SRCDIR=src
INCLUDESDIR=src/includes
BUILDDIR=build
INCLUDES=-I$(INCLUDESDIR)
OBJECTS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
TARGET=sudoku

.PHONY: default all clean directories debug

default: $(TARGET)
all: default

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

debug: CFLAGS=$(CFLAGS_DEBUG) $(CFLAGS_COMMON)
debug: default

clean:
	-rm -f $(BUILDDIR)/*
	-rm -f $(TARGET)
