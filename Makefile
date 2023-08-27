CC := gcc
CFLAGS := $(shell pkg-config --cflags raylib) -g -fopenmp #-Wall -Wextra -Wpedantic -std=c99 -O3
LDFLAGS := $(shell pkg-config --libs raylib) -lm -fopenmp

SRCDIR := src
TESTDIR := test
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/connect4
TESTTARGET := $(BINDIR)/test

RELEASE_CFLAGS := $(CFLAGS) -O3 -Wall -DNDEBUG

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

TESTSOURCES := $(wildcard $(TESTDIR)/*.c)
TESTOBJECTS := $(patsubst $(TESTDIR)/%.c, $(BUILDDIR)/%.o, $(TESTSOURCES))

.PHONY: all clean build test release

all: build

build: $(TARGET)

test: CFLAGS += -DTEST
test: $(TESTTARGET)

release: CFLAGS = $(RELEASE_CFLAGS)
release: build

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TESTTARGET): $(TESTOBJECTS) $(filter-out $(BUILDDIR)/main.o $(BUILDDIR)/gameUI.o, $(OBJECTS))
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
