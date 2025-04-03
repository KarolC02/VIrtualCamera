SRC_DIR = src 
BUILD_DIR = build/debug
CC = g++ 
SRC_FILES = $(wildcard src/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -I/opt/homebrew/Cellar/sdl2/2.32.4/include
LIBRARY_PATHS = -L/opt/homebrew/lib
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g
LINKER_FLAGS = -lSDL2

all:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)
