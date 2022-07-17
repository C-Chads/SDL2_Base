#include <stdio.h>
#include <unistd.h>

unsigned char red[4] = {0,255,0,0};
unsigned char green[4] = {0,0,255,0};
unsigned char blue[4] = {0,0,0,255};
unsigned char white[4] = {0,0,0,255};
unsigned char inputdata[2];
int main(){
	int i = 0;
	perror("[T]INITIATED TESTING SEQUENCE!!!\r\n");
	sleep(1);
	while(1){
		/*Constantly emit an output*/
		perror("[T]About to send PrN\r\n");
		write(1,"PrN",4);
		/*while(!read(0,inputdata,1));
		while(!read(0,inputdata+1,1));*/
		perror("[T]sending G\r\n");
		write(1,"G",1);/*Send Graphics*/
		perror("[T]sending Graphics...\r\n");
		for(i = 0; i < 640 * 480; i++){
			/*TODO: based on buttons pressed, draw different colors.*/
			write(1,blue,4);
		}
	}
}
