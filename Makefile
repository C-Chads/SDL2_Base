


#SDL_LIBS= -lmingw32 -lSDL2main -lSDL2
default:
	cc -O3 main.c -o main.out -lm -lSDL2 -Wall -std=c89 -Wno-unused
c-runner:
	cc -O3 runner.c -o main.out -lm -lSDL2 -Wall -std=c89 -Wno-unused
	cc -O3 runner_tester.c -o main_tester.out -lm -lSDL2 -Wall -std=c89 -Wno-unused
clean:
	rm -f *.out
