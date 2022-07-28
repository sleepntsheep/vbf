LIBS = -lncurses

all: vbf

vbf: *.c backend/*.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
