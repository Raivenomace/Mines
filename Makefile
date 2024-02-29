mines: mines.c
	gcc mines.c gfx.o -lX11 -lm -o mines

clean:
	rm mines

