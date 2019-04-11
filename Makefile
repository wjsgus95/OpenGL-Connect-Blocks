#TODO: grading environment not UNIX.
# 	   Makefile only for development.

EXE = ConnectBlocks
SRC = main.cc

.PHONY: clean

# Compiler
CC = g++

# Compiler Options
CFLAGS = -I utils -I object
# Linker Options
LDFLAG = -lGL -lm -lGLU -lGLEW -lglfw

default: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDFLAG)

clean:
	rm -f $(EXE)
