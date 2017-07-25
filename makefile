CC?=gcc
CFLAGS_COMMON=-Wall -pedantic -pedantic-errors --std=c11
CFLAGS_DEBUG=-O0 -g3 $(CLFAGS_COMMON)
CFLAGS_OPTIMAL=-O3 -mtune=native -Werror -fprefetch-loop-arrays
CFLAGS=$(CFLAGS_OPTIMAL) $(CFLAGS_COMMON)
SRCDIR=src
INCLUDESDIR=src/includes
BUILDDIR=build
INCLUDES=-I$(INCLUDESDIR)
MKDIR_P=mkdir -p
OBJECTS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
TARGET=sudoku

ifeq ($(CC), clang)
	CFLAGS:=$(filter-out -fprefetch-loop-arrays,$(CFLAGS))
endif

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
