#TODO: grading environment not UNIX.
# 	   Makefile only for development.

EXE = ConnectBlocks
SRC = main.cc

.PHONY: clean

# Compiler
CC = g++

# Compiler Options
CFLAGS = -g -std=c++11 -I utils -I object -Wall
# Linker Options
LDFLAG = -lGL -lm -lGLU -lGLEW -lglfw

default: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDFLAG)

clean:
	rm -f $(EXE)
