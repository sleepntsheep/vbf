LIBS = -lncurses

all: vbf

vbf: *.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
