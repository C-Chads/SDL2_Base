

/*
	put all your video game stuff here!
*/
Mix_Chunk* popwav;
unsigned int timer = 60;
img bubble;

int offsetx = 50;
int offsety = 50;

static void gameInit(){
	popwav = loadSample("assets/Sound Effects/Equipment Hit 1 Random Whacks and Hits 1.wav");
	bubble = loadImage("bubble1.png");
}

static void gameStep(){
	unsigned short btns;
	if(timer) timer--;
	pollevents();
	btns = get_gamerbuttons();
	if(timer == 0)if(btns & 0x40) {playSample(popwav);timer = 60;}
	if(btns & 0x80){
		unsigned int x = 0;
		unsigned int y = 0;
		
		for(y=0;y<height;y++)
		for(x=0;x<width;x++)
		writePixel(0x0000FF,x,y);
	} else {
		unsigned int x = 0;
		unsigned int y = 0;
		
		for(y=0;y<height;y++)
		for(x=0;x<width;x++)
		writePixel(0xff0000,x,y);		
	}
	drawImage(bubble,offsetx,offsety,2,2);
	if(btns & 0x1) offsety-=10;
	if(btns & 0x2) offsety+=10;
	if(btns & 0x4) offsetx-=10;
	if(btns & 0x8) offsetx+=10;
}
static void gameClose(){
	
}
