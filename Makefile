CC := gcc
CFLAGS := $(shell pkg-config --cflags raylib) -g #-Wall -Wextra -Wpedantic -std=c99 -O3
LDFLAGS := $(shell pkg-config --libs raylib) -lm

SRCDIR := src
TESTDIR := test
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/app
TESTTARGET := $(BINDIR)/test

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

TESTSOURCES := $(wildcard $(TESTDIR)/*.c)
TESTOBJECTS := $(patsubst $(TESTDIR)/%.c, $(BUILDDIR)/%.o, $(TESTSOURCES))

.PHONY: all clean build test

all: build

build: $(TARGET)

test: CFLAGS += -DTEST
test: $(TESTTARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TESTTARGET): $(TESTOBJECTS) $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))
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
