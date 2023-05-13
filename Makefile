CC := cc
CFLAGS := $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib) -lm

SRCDIR := src
TESTDIR := test
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/app
TESTTARGET := $(BINDIR)/test

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

TESTSOURCES := $(wildcard $(SRCDIR)/*.c $(TESTDIR)/*.c)
TESTOBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(filter-out $(SRCDIR)/main.c, $(TESTSOURCES)))

.PHONY: all clean build test

all: build

build: clean $(TARGET)

test: clean $(TESTTARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TESTTARGET): $(TESTOBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
