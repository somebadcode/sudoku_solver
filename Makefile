CC?=cc
CFLAGS_COMMON=-Wall -Wextra -pedantic -pedantic-errors -fPIE --std=c11
CFLAGS_DEBUG=-O0 -g3 $(CLFAGS_COMMON)
CFLAGS_OPTIMAL=-O2 -D_FORTIFY_SOURCE=2 -mtune=native -flto -pipe -Werror=format-security -Werror=implicit-function-declaration -fstack-protector-strong
CFLAGS=$(CFLAGS_OPTIMAL) $(CFLAGS_COMMON)
LDFLAGS=$(CFLAGS) -Wl,-z,relro,-z,now,-z,defs -pie
SRCDIR=src
BUILDDIR=build
BINDIR=bin
OBJECTS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
TARGET=$(BINDIR)/sudoku

# GCC CC supports stack clash protection so append it and wait for Clang to support it.
ifeq ($(CC), cc)
	CFLAGS+= -fstack-clash-protection
endif

.PHONY: default all clean debug
.PRECIOUS: $(TARGET) $(OBJECTS)

default: $(TARGET)
all: clean default

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(BUILDDIR) $(OBJECTS) $(BINDIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILDDIR):
	@mkdir --parent $(BUILDDIR)

$(BINDIR):
	@mkdir --parent $(BINDIR)

debug: CFLAGS=$(CFLAGS_DEBUG) $(CFLAGS_COMMON)
debug: all

clean:
	@rm --recursive --force $(BUILDDIR) $(BINDIR)
