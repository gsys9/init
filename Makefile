.PHONY: all clean help

CXX?=g++ -std=c++11
CC?=gcc
CC_FLAGS=-c -g -I include/
CXX_FLAGS=-fpermissive -c -g -I include/

EXEC=init
CXX_SOURCES:=$(wildcard src/*.cc)
C_SOURCES:=$(wildcard src/*.c)
CXX_OBJ:=$(patsubst src/%.cc,build/%.o,$(CXX_SOURCES))
C_OBJ:=$(patsubst src/%.c,build/%.o,$(C_SOURCES))

all: build build/$(EXEC)

build:
	mkdir -p build

build/$(EXEC): $(CXX_OBJ) $(C_OBJ)
	$(CXX) $^ -o $@

$(CXX_OBJ): build/%.o : src/%.cc
	$(CXX) $(CXX_FLAGS) $< -o $@

$(C_OBJ): build/%.o : src/%.c
	$(CC) $(CC_FLAGS) $< -o $@


clean:
	rm -rf build/*.o build/$(EXEC)

help:
	@echo "Usage: make {all|clean|help}" 1>&2 && false