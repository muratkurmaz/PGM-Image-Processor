CC := gcc
CFLAGS := -std=gnu11 -Wall -Wextra -O2 -g -Iinclude
LDFLAGS :=

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
COMMON_OBJ := $(BUILD_DIR)/pgm.o

PROGRAMS := pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble
BINS := $(addprefix $(BIN_DIR)/,$(PROGRAMS))

.PHONY: all clean test dirs

all: dirs $(BINS)

dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(COMMON_OBJ): $(SRC_DIR)/pgm.c include/pgm.h | dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c include/pgm.h | dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(BUILD_DIR)/%.o $(COMMON_OBJ) | dirs
	$(CC) $^ $(LDFLAGS) -o $@

test: all
	bash tests/run_tests.sh

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) tests/output
