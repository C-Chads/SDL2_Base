#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "header_only_libs/stringutil.h"
#define SDL_MAIN_HANDLED
#ifdef __TINYC__
#define STBI_NO_SIMD
#define SDL_DISABLE_IMMINTRIN_H 1
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "header_only_libs/stb_image.h"
#include "header_only_libs/3dMath.h"
#include "header_only_libs/chadphys.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
static SDL_Window *sdl_win = NULL;
static SDL_GLContext* sdl_glcontext = NULL;
static int shouldquit = 0;
static int width = 640, height = 480, display_scale = 1;
static int mousex = 0, mousey = 0, mousexrel=0, mouseyrel=0;

static int modint(int a, int b){
	return (a%b+b)%b;
}

void onClick(int btn, int state);

static int lockCursor(int state){
	return SDL_SetRelativeMouseMode(state);
}

static void pollevents(){
	SDL_Event ev;
	while(SDL_PollEvent(&ev)){
		if(ev.type == SDL_QUIT) {shouldquit = 0xFFff; /*Magic value for quit.*/}
		else if(ev.type == SDL_MOUSEMOTION){
			mousex = ev.motion.x;
			mousey = ev.motion.y;
			mousexrel = ev.motion.xrel;
			mouseyrel = ev.motion.yrel;
		} else if (ev.type == SDL_MOUSEBUTTONDOWN){
			if(ev.button.button == SDL_BUTTON_LEFT){
				onClick(0,1);
			} else if(ev.button.button == SDL_BUTTON_RIGHT){
				onClick(1,1);
			}else if(ev.button.button == SDL_BUTTON_MIDDLE){
				onClick(2,1);
			}
		} else if (ev.type == SDL_MOUSEBUTTONDOWN){
			if(ev.button.button == SDL_BUTTON_LEFT){
				onClick(0,0);
			} else if(ev.button.button == SDL_BUTTON_RIGHT){
				onClick(1,0);
			}else if(ev.button.button == SDL_BUTTON_MIDDLE){
				onClick(2,0);
			}
		}
	}
}

/*
static void mouse_update(){
	unsigned a = SDL_GetMouseState(&mousex, &mousey);
	mouse1 = ((a & SDL_BUTTON_LMASK) != 0);
	mouse2 = ((a & SDL_BUTTON_RMASK) != 0);
	mouse3 = ((a & SDL_BUTTON_MMASK) != 0);
}
*/
#define MAX_SAMPLES 8192
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

GLuint loadRGBTexture(unsigned char* buf, unsigned int w, unsigned int h) {
	GLuint t = 0;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	return t;
}

GLuint loadRGBATexture(unsigned char* buf, unsigned int w, unsigned int h) {
	GLuint t = 0;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	return t;
}

GLuint createModelDisplayList(
	// HUGE important note! these depend on the math library using
	// f_ as float and not double!
	// Remember that!
	vec3* points, uint npoints, vec3* colors, vec3* normals, vec3* texcoords
	) {
	GLuint ret = 0;
	if (!points)
		return 0;
	ret = glGenLists(1);
	glNewList(ret, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (uint i = 0; i < npoints; i++) {
		if (colors) {
			glColor3f(colors[i].d[0], colors[i].d[1], colors[i].d[2]);
		}
		if (texcoords)
			glTexCoord2f(texcoords[i].d[0], texcoords[i].d[1]);
		if (normals)
			glNormal3f(normals[i].d[0], normals[i].d[1], normals[i].d[2]);
		glVertex3f(points[i].d[0], points[i].d[1], points[i].d[2]);
	}
	glEnd();
	glEndList();
	return ret;
}

static void playSample(Mix_Chunk* samp){
	Mix_PlayChannel(-1, samp, 0);
}



static float randf(){
	return ((float)rand()/(float)RAND_MAX);
}



#include "myGameGL.h"

int main(int argc, char** argv){
	{int i = 1;for(i = 1;i < argc; i++){
		if(strprefix("-w", argv[i-1])){
			width = strtoul(argv[i], 0,0);
		} else if(strprefix("-h", argv[i-1])){
			height = strtoul(argv[i], 0,0);
		}
	}}

	if(width == 0 || height == 0 || width > 32767 || height > 32767){
		printf("\r\n<INVALID WIDTH OR HEIGHT>\r\n");
		exit(1);
	}

	
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
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	if(!sdl_win)
	{
		printf("SDL2 window creation failed.\n"
			"SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	sdl_glcontext = SDL_GL_CreateContext(sdl_win);
	if(!sdl_glcontext){
		printf("SDL2 GL Context creation failed.\n"
		"SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	if (Mix_OpenAudio(44100,AUDIO_S16SYS, 2, 512) < 0){
	  fprintf(stderr,"\r\nSDL2 mixer audio opening failed!\r\n"
	  "SDL_Error: %s\r\n", SDL_GetError());
	  exit(-1);
	}
	if(Mix_AllocateChannels(32) < 0){
	  fprintf(stderr,"\r\nSDL2 mixer channel allocation failed!\r\n"
	  "SDL_Error: %s\r\n", SDL_GetError());
	  exit(-1);		
	}
	gameInit();
	while(!shouldquit){
		pollevents();
		gameStep();
		SDL_GL_SwapWindow(sdl_win);
	}
	gameClose();
	Mix_CloseAudio();
   	SDL_DestroyWindow(sdl_win);
    SDL_Quit();
}
