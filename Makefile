


#SDL_LIBS= -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer
default:
	cc -O3 main.c -o main.out -lm -lSDL2 -lSDL2_mixer -Wall -std=c99 -Wno-unused
clean:
	rm -f *.out
