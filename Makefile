cc  := cc
src := *.c backend/*.c
lib := -lSDL2 -lSDL2_ttf
exe := build/vbf
cflags := -DHAVE_SDL2=1

vbf: $(src)
	$(cc) $^ $(cflags) -o $(exe) $(lib)
