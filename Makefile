CC := cc
CFLAGS := $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib)

SRCS := main.c
OBJS := $(SRCS:.c=.o)
BIN_DIR := ./bin
TARGET := $(BIN_DIR)/app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BIN_DIR) $(OBJS)
