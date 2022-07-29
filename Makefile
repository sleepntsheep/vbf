LIBS = -lncurses -lSDL2 -lSDL2_ttf
CFLAGS = -g -Wall 

all: vbf

vbf: *.c backend/*.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
