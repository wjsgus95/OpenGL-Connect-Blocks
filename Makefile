EXE = ConnectBlocks
SRC = main.cc

.PHONY: clean

# Compiler
CC = g++

# Compiler Options
CFLAGS = -g -O3 -std=c++11 -I utils -I object -I /usr/include/freetype2 -Wall
# Linker Options
LDFLAG = -lGL -lm -lGLU -lGLEW -lglfw -lassimp -lfreetype

default: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDFLAG)

clean:
	rm -f $(EXE)
