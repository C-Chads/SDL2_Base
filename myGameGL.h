
#include "header_only_libs/chade_min.h"
#include "header_only_libs/tobjparse.h"
#include <stdio.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif
double time_passed = 0.0;
Mix_Chunk* popwav;
unsigned sfx_tmr = 0;

static unsigned short get_gamerbuttons(){
	unsigned short retval = 0;
	const unsigned char *state;
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

void onClick(int btn, int state){
	if(btn == 0 && state == 1)
		printf("Click %d %d\r\n", mousex, mousey);
	if(btn == 1 && state == 1)
			printf("Right Click %d %d\r\n", mousex, mousey);
}

void gameInit(){
	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_NORMALIZE);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glFrustum( -1.0, 1.0, -h, h, 5.0, 60.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	popwav = loadSample("assets/Sound Effects/Equipment Hit 1 Random Whacks and Hits 1.wav");
}
void gameStep(){
	unsigned short btns;
	if(sfx_tmr) sfx_tmr--;
	pollevents();
	btns = get_gamerbuttons();
	if(sfx_tmr == 0)
		if(btns & 0x40)
			{printf("should play!\r\n");playSample(popwav);sfx_tmr = 60;}

	if(btns & 0x80)
		shouldquit = 0xFFff;

	/*draw	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	//glRotatef(time_passed*50, time_passed*34, time_passed*19, 1);
	glRotatef(time_passed*50, 0,0, 1);
	glBegin(GL_TRIANGLES);
	glColor3f(0.2, 0.2, 1.0); // BLUE!
	// glColor3f(1.0, 0.2, 0.2); //RED!
	glVertex3f(-0.8, -0.8, 0.2);

	glColor3f(0.2, 1.0, 0.2); // GREEN!
	// glColor3f(1.0, 0.2, 0.2); //RED!
	glVertex3f(0.8, -0.8, 0.2);

	glColor3f(1.0, 0.2, 0.2); // RED!
	glVertex3f(0, 1.2, 0.2);
	glEnd();
	glPopMatrix();
	time_passed += 0.01666666;
}
void gameClose(){

}
