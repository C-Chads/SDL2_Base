

/*
	put all your video game stuff here!
*/
#include <math.h>
#include <time.h>
#include <stdlib.h>
Mix_Chunk* popwav;
unsigned sfx_tmr = 60;
img bubble1;
img bubble2;

typedef struct{
	float x;
	float y;
	float vx;
	float vy;
}particle;

#define MAX_PARTICLES (1024)
particle particles[MAX_PARTICLES];

#define MAX_DIR_SPEED 5
#define AIR_FRICTION_MULT 0.99
float gx = 0;
float gy = -0.2;



static void gameInit(){
	popwav = loadSample("assets/Sound Effects/Equipment Hit 1 Random Whacks and Hits 1.wav");
	bubble1 = loadImage("bubble1.png");
	bubble2 = loadImage("bubble2.png");
	srand(time(NULL));
	{long i;for(i=0;i<MAX_PARTICLES;i++){
				particles[i].x=randf() * ((float)(width+100)-50.0);
				particles[i].y=randf() * (height+2*bubble1.h);
				particles[i].vy=randf()*7.0 - 3.5;
				particles[i].vx=randf()*2.0 -1.0;
	}}
}

static int hfunc_sin(int y){
	return 10.0f * cos((float)y/(float)height * 19);
}

static void gameStep(){
	unsigned short btns;
	if(sfx_tmr) sfx_tmr--;
	pollevents();
	btns = get_gamerbuttons();
	if(sfx_tmr == 0) 
		if(btns & 0x40) 
			{playSample(popwav);sfx_tmr = 60;}
	{unsigned int x = 0;unsigned int y = 0;
		for(y=0;y<height;y++)for(x=0;x<width;x++)
			writePixel(0x2222AA,x,y);
	}
	{long i;for(i=0;i<MAX_PARTICLES;i++){
				particles[i].x+=particles[i].vx;
				particles[i].y+=particles[i].vy;
				particles[i].vy+=gy+randf()*0.3;
				particles[i].vy*=AIR_FRICTION_MULT;
				particles[i].vx+=gx+randf()*0.5 - 0.25;
				particles[i].vx*=AIR_FRICTION_MULT;
				if(particles[i].vx > MAX_DIR_SPEED) particles[i].vx = MAX_DIR_SPEED;
				if(particles[i].vx < (-1*MAX_DIR_SPEED)) particles[i].vx = -MAX_DIR_SPEED;
				if(particles[i].vy > MAX_DIR_SPEED) particles[i].vy = MAX_DIR_SPEED;
				if(particles[i].vy < (-1*MAX_DIR_SPEED)) particles[i].vy = -MAX_DIR_SPEED;
				if(particles[i].y < -bubble1.h){
					/*re-init particle*/
					particles[i].x=randf() * ((float)(width+100)-50.0);
					
					particles[i].y=height + bubble1.h+10;
					particles[i].vy=randf()*7 - 3.5;
					particles[i].vx=randf()*2 - 1;
				}
					drawImage(bubble2,particles[i].x,particles[i].y,1,1);
	}}
	
}
static void gameClose(){
	
}
