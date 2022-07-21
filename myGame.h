

/*
	put all your video game stuff here!
*/
Mix_Chunk* popwav;
unsigned int timer = 500;

static void gameInit(){
	popwav = loadSample("assets/Sound Effects/Equipment Hit 1 Random Whacks and Hits 1.wav");
}

static void gameStep(){
	unsigned short btns;
	if(timer) timer--;
	pollevents();
	btns = get_gamerbuttons();
	if(timer == 0)if(btns & 0x40) playSample(popwav);
}
static void gameClose(){
	
}
