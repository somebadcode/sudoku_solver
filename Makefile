CC?=cc
CFLAGS_COMMON=-Wall -Wextra -pedantic -pedantic-errors --std=c11
CFLAGS_DEBUG=-O0 -g3 $(CLFAGS_COMMON)
CFLAGS_OPTIMAL=-O3 -mtune=native -flto
CFLAGS=$(CFLAGS_OPTIMAL) $(CFLAGS_COMMON)
SRCDIR=src
BUILDDIR=build
BINDIR=bin
OBJECTS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
TARGET=sudoku

.PHONY: default all clean directories debug
.PRECIOUS: $(TARGET) $(OBJECTS)

default: directories $(TARGET)
all: default

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o ${BINDIR}/$@

directories: $(BUILDDIR) $(BINDIR)

$(BUILDDIR):
	@mkdir --parent ${BUILDDIR}

$(BINDIR):
	@mkdir --parent ${BINDIR}

debug: CFLAGS=$(CFLAGS_DEBUG) $(CFLAGS_COMMON)
debug: all

clean:
	@rm --recursive --force $(BUILDDIR) $(BINDIR)
