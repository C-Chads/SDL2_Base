#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "header_only_libs/stringutil.h"
#define SDL_MAIN_HANDLED
#ifdef __TINYC__
#define STBI_NO_SIMD
#define SDL_DISABLE_IMMINTRIN_H 1
#endif

#include "header_only_libs/font8x8_basic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
static SDL_Window *sdl_win = NULL;
static SDL_Renderer *sdl_rend = NULL;
static SDL_Texture *sdl_tex = NULL;
static SDL_AudioSpec sdl_spec = {0};
#define ABUF_LEN 0x100000
static int audio_left = 0;
static unsigned char abuf[ABUF_LEN];
static unsigned int *SDL_targ = NULL; /*Default to all black.*/
static unsigned int width = 640;
static unsigned int height = 480;
static unsigned int display_scale = 1;
static unsigned char FG_color = 15;
int shouldquit = 0;
static unsigned int vga_palette[256] = {
#include "header_only_libs/vga_pal.h"
};
/*
audio callback.
*/
static void sdl_audio_callback(void *udata, Uint8 *stream, int len){
	(void)udata;
	SDL_memset(stream, 0, len);
	if(audio_left == 0){return;}
	len = (len < audio_left) ? len : audio_left;
	SDL_MixAudio(stream, abuf + (ABUF_LEN - audio_left), len, SDL_MIX_MAXVOLUME);
	audio_left -= len;
}

static void pollevents(){
	SDL_Event ev;
	while(SDL_PollEvent(&ev)){
		if(ev.type == SDL_QUIT) shouldquit = 0xFFff; /*Magic value for quit.*/
	}
}

static void renderchar(unsigned char* bitmap, unsigned int p) {
	unsigned int x, y, _x, _y;
	unsigned int set;
	_x = p%(width/8);
	_y = p/(width/8);
	_x *= 8;
	_y *= 8;
	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {
			set = bitmap[x] & (1 << y);
			if (set) SDL_targ[(_x+y) + (_y+x) * (width)] = vga_palette[FG_color];
		}
	}
}

static void writePixel(unsigned int value, unsigned long x, unsigned long y){
	if((x < width) && (y < height)) SDL_targ[y * width + x] = value;
}

static unsigned short get_gamerbuttons(){
	unsigned short retval = 0;
	const unsigned char *state;
	SDL_PumpEvents();
	state = SDL_GetKeyboardState(NULL);
	retval |= 0x1 * (state[SDL_SCANCODE_UP]!=0);
	retval |= 0x2 * (state[SDL_SCANCODE_DOWN]!=0);
	retval |= 0x4 * (state[SDL_SCANCODE_LEFT]!=0);
	retval |= 0x8 * (state[SDL_SCANCODE_RIGHT]!=0);
	retval |= 0x10 * (state[SDL_SCANCODE_RETURN]!=0);
	retval |= 0x20 * (state[SDL_SCANCODE_RSHIFT]!=0);
	retval |= 0x40 * (state[SDL_SCANCODE_Z]!=0);
	retval |= 0x80 * (state[SDL_SCANCODE_X]!=0);
	retval |= 0x100 * (state[SDL_SCANCODE_C]!=0);
	retval |= 0x200 * (state[SDL_SCANCODE_A]!=0);
	retval |= 0x400 * (state[SDL_SCANCODE_S]!=0);
	retval |= 0x800 * (state[SDL_SCANCODE_D]!=0);
	return retval;
}

#define MAX_SAMPLES 4096
Mix_Chunk* samples[MAX_SAMPLES];
unsigned long nsamples=0;

static Mix_Chunk* loadSample(char* name){
	Mix_Chunk* v = NULL;
	if(nsamples == MAX_SAMPLES) {exit(1);return NULL;}
	
	v = Mix_LoadWAV(name);
	if(v == NULL) 
	{exit(1);return NULL;}
	samples[nsamples++] = v;
	return v;
}

static void playSample(Mix_Chunk* samp){
	Mix_PlayChannel(-1, samp, 0);
}

#include "myGame.h"


SDL_Rect screenrect;
SDL_Rect screenrect2;

int main(int argc, char** argv){
	{int i = 1;for(i = 1;i < argc; i++){
		if(strprefix("-w", argv[i-1])){
			width = strtoul(argv[i], 0,0);
		} else if(strprefix("-h", argv[i-1])){
			height = strtoul(argv[i], 0,0);
		}else if(strprefix("-s", argv[i-1])){
			display_scale = strtoul(argv[i], 0,0);
		}
	}}

	if(width == 0 || height == 0 || width > 32767 || height > 32767){
		printf("\r\n<INVALID WIDTH OR HEIGHT>\r\n");
		exit(1);
	}
	SDL_targ = calloc(1, width * height * 4);
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL2 could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
	sdl_win = SDL_CreateWindow("Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width * display_scale, 
		height * display_scale,
		SDL_WINDOW_SHOWN
	);
	if(!sdl_win)
	{
		printf("SDL2 window creation failed.\n"
			"SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	sdl_rend = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
	if(!sdl_rend){
		printf("SDL2 renderer creation failed.\n"
			"SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	sdl_tex = SDL_CreateTexture(sdl_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 
		width,
		height
	);
	if(!sdl_tex){
		printf("SDL2 texture creation failed.\n"
			"SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	if ( Mix_OpenAudio(44100,AUDIO_S16SYS, 2, 512) < 0){
	  fprintf(stderr,"\r\nSDL2 mixer audio opening failed!\r\n"
	  "SDL_Error: %s\r\n", SDL_GetError());
	  exit(-1);
	}
	if(Mix_AllocateChannels(32) < 0){
	  fprintf(stderr,"\r\nSDL2 mixer channel allocation failed!\r\n"
	  "SDL_Error: %s\r\n", SDL_GetError());
	  exit(-1);		
	}
	screenrect.x = 0;
	screenrect.y = 0;
	screenrect.w = width;
	screenrect.h = height;
	screenrect2 = screenrect;
	screenrect2.w *= display_scale;
	screenrect2.h *= display_scale;
	gameInit();
	while(!shouldquit){
		pollevents();
		/*
			TODO: game logic
		*/
		gameStep();
		{

			SDL_UpdateTexture(
				sdl_tex,
				NULL,
				SDL_targ, 
				width * 4
			);
			SDL_RenderCopy(
				sdl_rend, 
				sdl_tex,
				&screenrect,
				&screenrect2
			);
			SDL_RenderPresent(sdl_rend);
		}
	}
	gameClose();
	SDL_DestroyTexture(sdl_tex);
	SDL_DestroyRenderer(sdl_rend);
	{unsigned long i;
		for(i=0;i<nsamples;i++){
		Mix_FreeChunk(samples[i]);
	}}
	Mix_CloseAudio();
   	SDL_DestroyWindow(sdl_win);
    SDL_Quit();
    return 0;
}
