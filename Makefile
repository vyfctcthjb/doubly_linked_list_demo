# Portable Makefile (macOS/Linux)

CXX      ?= clang++
CXXFLAGS ?= -std=c++20 -O2 -g -Wall -Wextra -Wpedantic
SAN      ?= -fsanitize=address,undefined
INC      := -Iinclude
BIN_DIR  := build

DEMO1    := $(BIN_DIR)/doubly_linked_list_demo
GENERIC  := $(BIN_DIR)/generic_demo

.PHONY: all run run-generic test clean

all: $(DEMO1) $(GENERIC)

$(BIN_DIR):
	@mkdir -p $@

$(DEMO1): main.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SAN) $(INC) $< -o $@

$(GENERIC): generic_main.cpp include/doubly_linked_list.hpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SAN) $(INC) generic_main.cpp -o $@

run: $(DEMO1)
	$(DEMO1)

run-generic: $(GENERIC)
	$(GENERIC)

test: $(DEMO1)
	tests/smoke.sh

clean:
	rm -rf $(BIN_DIR)
