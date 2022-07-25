


#SDL_LIBS= -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer
default: main main2
main:
	cc -O3 main.c -o main.out -lm -lSDL2 -lSDL2_mixer -Wall -std=c99 -Wno-unused
main2:
	cc -O3 main2.c -o main2.out -lm -lSDL2 -lGL -lSDL2_mixer -Wall -std=c99 -Wno-unused
clean:
	rm -f *.out *.exe
